#include "../../include/utils/Parser.hpp"
#include <fstream>
#include <stdexcept>

Instance InstanceParser::parse(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Parser.cpp::parse >> cannot open file.\n");
  }

  int v, e;
  if (!(file >> v >> e)) {
    throw std::runtime_error(
        "Parser.cpp::parse >> invalid file format (|V| |E|).\n");
  }

  Instance instance{SequenceGraph(v), ""};

  char l;
  for (int i = 0; i < v; i++) {
    file >> l;
    instance.graph.insert_vertex(l);
  }

  int p, q;
  for (int i = 0; i < e; i++) {
    file >> p >> q;
    instance.graph.insert_edge(p, q);
  }

  file >> instance.query;

  return instance;
}
