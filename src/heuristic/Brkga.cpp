#include "../../include/heuristic/Brkga.hpp"
#include <algorithm>
#include <limits>
#include <random>

GsapDecoder::GsapDecoder(const SequenceGraph &graph, const std::string &query)
    : graph_(graph), query_(query) {}

double GsapDecoder::score(const BRKGA::Chromosome &chromosome, int v,
                          int i) const {
  return chromosome[v] + (graph_.get_label(v) == query_[i] ? kLabelBonus : 0.0);
}

std::vector<int>
GsapDecoder::build_path(const BRKGA::Chromosome &chromosome) const {
  const int v_size = graph_.get_vertices_size();
  const int q_len = static_cast<int>(query_.length());

  std::vector<int> path;
  if (v_size == 0 || q_len == 0) {
    return path;
  }

  std::vector<bool> visited(v_size, false);

  int start = 0;
  double best_score = score(chromosome, 0, 0);
  for (int v = 1; v < v_size; v++) {
    const double s = score(chromosome, v, 0);
    if (s > best_score) {
      start = v;
      best_score = s;
    }
  }

  path.reserve(q_len);
  path.push_back(start);
  visited[start] = true;

  for (int i = 1; i < q_len; i++) {
    int best_next = -1;
    double best_next_score = 0.0;

    for (int neighbor : graph_.get_neighbors(path.back())) {
      if (visited[neighbor]) {
        continue;
      }
      const double s = score(chromosome, neighbor, i);
      if (best_next == -1 || s > best_next_score) {
        best_next = neighbor;
        best_next_score = s;
      }
    }

    if (best_next == -1) {
      return path;
    }

    path.push_back(best_next);
    visited[best_next] = true;
  }

  return path;
}

double GsapDecoder::decode(BRKGA::Chromosome &chromosome, bool rewrite) {
  (void)rewrite;

  const std::vector<int> path = build_path(chromosome);
  const int q_len = static_cast<int>(query_.length());
  const int p_len = static_cast<int>(path.size());

  if (p_len != q_len) {
    return static_cast<double>(q_len + (q_len - p_len) + 1);
  }

  int distance = 0;
  for (int i = 0; i < q_len; i++) {
    if (graph_.get_label(path[i]) != query_[i]) {
      distance++;
    }
  }

  return static_cast<double>(distance);
}
BrkgaAnswer BrkgaSolver::solve(const SequenceGraph &graph,
                               const std::string &query, int max_generations) {
  const int v_size = graph.get_vertices_size();
  const int q_len = static_cast<int>(query.length());

  if (q_len == 0) {
    return {std::vector<int>(), 0};
  }
  if (v_size < 2) {
    return {std::vector<int>(), std::numeric_limits<int>::max()};
  }

  GsapDecoder decoder(graph, query);

  // Standard BRKGA-MP-IPR hyper-parameters.
  BRKGA::BrkgaParams params;
  params.population_size = 100;
  params.elite_percentage = 0.20;
  params.mutants_percentage = 0.10;
  params.num_elite_parents = 1;
  params.total_parents = 2;
  params.bias_type = BRKGA::BiasFunctionType::CONSTANT;
  params.num_independent_populations = 1;

  params.alpha_block_size = 1.0;
  params.pr_percentage = 1.0;

  const unsigned chromosome_size = static_cast<unsigned>(v_size);
  const unsigned seed = std::random_device{}();
  const unsigned max_threads = 1;

  BRKGA::BRKGA_MP_IPR<GsapDecoder> brkga_engine(decoder, BRKGA::Sense::MINIMIZE,
                                                seed, chromosome_size, params,
                                                max_threads);

  const int generations_to_run = std::max(max_generations, 1);

  for (int gen = 0; gen < generations_to_run; gen++) {
    brkga_engine.evolve(1);

    if (brkga_engine.getBestFitness() <= 0.0) {
      break;
    }
  }

  const BRKGA::Chromosome &best_chromosome = brkga_engine.getBestChromosome();
  std::vector<int> best_path = decoder.build_path(best_chromosome);

  int distance;
  if (static_cast<int>(best_path.size()) == q_len) {
    distance = 0;
    for (int i = 0; i < q_len; i++) {
      if (graph.get_label(best_path[i]) != query[i]) {
        distance++;
      }
    }
  } else {
    best_path.clear();
    distance = std::numeric_limits<int>::max();
  }

  return {best_path, distance};
}
