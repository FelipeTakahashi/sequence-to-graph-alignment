#ifndef SEQUENCE_GRAPH_HPP
#define SEQUENCE_GRAPH_HPP

#include <string>
#include <vector>

class SequenceGraph {
private:
  int vertices_size;
  int edges_size;

  std::vector<char> labels;
  std::vector<std::vector<int>> adj_list;

public:
  SequenceGraph() : vertices_size(0), edges_size(0) {}
  explicit SequenceGraph(int n) : vertices_size(0), edges_size(0) {
    labels.reserve(n);
    adj_list.resize(n);
  }

  int insert_vertex(char label);
  void insert_edge(int v, int u);

  inline int get_vertices_size() const { return vertices_size; }
  inline int get_edges_size() const { return edges_size; }

  inline char get_label(int v) const { return labels[v]; }

  inline const std::vector<int> &get_neighbors(int v) const {
    return adj_list[v];
  }

  std::string spell_path(const std::vector<int> &path) const;
};

#endif // SEQUENCE_GRAPH_HPP
