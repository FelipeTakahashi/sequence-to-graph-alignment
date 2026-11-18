#ifndef GREEDY_HPP
#define GREEDY_HPP

#include "../graph/SequenceGraph.hpp"
#include <limits>

struct GreedyAnswer {
  std::vector<int> path;
  int distance;
};

class Greedy {
public:
  static GreedyAnswer solve(const SequenceGraph &graph,
                            const std::string &query);
};

#endif // GREEDY_HPP
