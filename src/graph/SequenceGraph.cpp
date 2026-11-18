#include "../../include/graph/SequenceGraph.hpp"
#include <stdexcept>

int SequenceGraph::insert_vertex(char label) {
  int v_id = vertices_size++;
  labels.push_back(label);
  adj_list.emplace_back();
  return v_id;
}

void SequenceGraph::insert_edge(int v, int u) {
  adj_list[v].push_back(u);
  edges_size++;
}

std::string SequenceGraph::spell_path(const std::vector<int> &path) const {
  int n = path.size();

  std::string sequence(path.size(), '\0');

  for (size_t i = 0; i < n; i++) {
    sequence[i] = labels[path[i]];
  }

  return sequence;
}
