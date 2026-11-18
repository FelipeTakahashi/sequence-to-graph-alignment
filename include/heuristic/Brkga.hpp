#ifndef BRKGA_HPP
#define BRKGA_HPP

#ifndef BRKGA_MULTIPLE_INCLUSIONS
#define BRKGA_MULTIPLE_INCLUSIONS
#endif

#include "../brkga_mp_ipr/brkga_mp_ipr.hpp"
#include "../graph/SequenceGraph.hpp"
#include <string>
#include <vector>

struct BrkgaAnswer {
  std::vector<int> path;
  int distance;
};

class GsapDecoder {
public:
  static constexpr double kLabelBonus = 0.5;

  GsapDecoder(const SequenceGraph &graph, const std::string &query);

  double decode(BRKGA::Chromosome &chromosome, bool rewrite);

  std::vector<int> build_path(const BRKGA::Chromosome &chromosome) const;

private:
  double score(const BRKGA::Chromosome &chromosome, int v, int i) const;

  const SequenceGraph &graph_;
  const std::string &query_;
};

class BrkgaSolver {
public:
  static BrkgaAnswer solve(const SequenceGraph &graph, const std::string &query,
                           int max_generations);
};

#endif // BRKGA_HPP
