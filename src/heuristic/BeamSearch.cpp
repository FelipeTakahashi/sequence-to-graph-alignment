#include "../../include/heuristic/BeamSearch.hpp"
#include <algorithm>

struct PartialPath {
  std::vector<int> path;
  std::vector<bool> visited;
  int distance;
  int degree;
};

namespace {

BeamSearchAnswer run_from(const SequenceGraph &graph, const std::string &query,
                          int beam_width, int start) {
  int v_size = graph.get_vertices_size();
  int q_len = query.length();

  auto path_comparator = [](const PartialPath &a, const PartialPath &b) {
    if (a.distance != b.distance) {
      return a.distance < b.distance;
    }
    return a.degree > b.degree;
  };

  std::vector<PartialPath> beam;

  PartialPath initial;
  initial.path.reserve(q_len);
  initial.visited.assign(v_size, false);
  initial.path.push_back(start);
  initial.visited[start] = true;
  initial.distance = (graph.get_label(start) == query[0]) ? 0 : 1;
  initial.degree = graph.get_neighbors(start).size();
  beam.push_back(std::move(initial));

  for (int i = 1; i < q_len; i++) {
    std::vector<PartialPath> next_candidates;

    for (const auto &current_p : beam) {
      int last_vertex = current_p.path.back();
      const auto &neighbors = graph.get_neighbors(last_vertex);

      for (int neighbor : neighbors) {
        if (current_p.visited[neighbor]) {
          continue;
        }

        PartialPath next_p = current_p;

        next_p.path.push_back(neighbor);
        next_p.visited[neighbor] = true;

        if (graph.get_label(neighbor) != query[i]) {
          next_p.distance++;
        }

        next_p.degree = graph.get_neighbors(neighbor).size();

        next_candidates.push_back(std::move(next_p));
      }
    }

    if (next_candidates.empty()) {
      return {std::vector<int>(), std::numeric_limits<int>::max()};
    }

    std::stable_sort(next_candidates.begin(), next_candidates.end(),
                     path_comparator);
    int keep = std::min(beam_width, static_cast<int>(next_candidates.size()));
    beam.assign(std::make_move_iterator(next_candidates.begin()),
                std::make_move_iterator(next_candidates.begin() + keep));
  }

  return {beam[0].path, beam[0].distance};
}

} // namespace

BeamSearchAnswer BeamSearch::solve(const SequenceGraph &graph,
                                   const std::string &query, int beam_width) {
  int v_size = graph.get_vertices_size();
  int q_len = query.length();

  if (q_len == 0 || v_size == 0) {
    return {std::vector<int>(), 0};
  }
  if (beam_width <= 0) {
    return {std::vector<int>(), std::numeric_limits<int>::max()};
  }

  BeamSearchAnswer best = {std::vector<int>(), std::numeric_limits<int>::max()};

  for (int start = 0; start < v_size; start++) {
    BeamSearchAnswer current = run_from(graph, query, beam_width, start);

    if (!current.path.empty() && current.distance < best.distance) {
      best = std::move(current);

      if (best.distance == 0) {
        break;
      }
    }
  }

  return best;
}
