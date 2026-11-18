#ifndef INSTANCE_PARSER_HPP
#define INSTANCE_PARSER_HPP

#include "../graph/SequenceGraph.hpp"
#include <string>

struct Instance {
  SequenceGraph graph;
  std::string query;
};

class InstanceParser {
public:
  static Instance parse(const std::string &filepath);
};

#endif // INSTANCE_PARSER_HPP
