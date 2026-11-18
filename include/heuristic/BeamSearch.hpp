#ifndef BEAM_SEARCH_HPP
#define BEAM_SEARCH_HPP

#include "../graph/SequenceGraph.hpp"
#include <limits>
#include <string>
#include <vector>

struct BeamSearchAnswer {
  std::vector<int> path;
  int distance;
};

class BeamSearch {
public:
  static BeamSearchAnswer solve(const SequenceGraph &graph,
                                const std::string &query, int beam_width);
};

#endif // BEAM_SEARCH_HPP
