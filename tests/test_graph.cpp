#include "../include/graph/SequenceGraph.hpp"
#include <cassert>
#include <iostream>

void test_graph() {

  SequenceGraph g;

  int v0 = g.insert_vertex('A');
  int v1 = g.insert_vertex('C');
  int v2 = g.insert_vertex('G');
  int v3 = g.insert_vertex('T');

  g.insert_edge(v0, v1);
  g.insert_edge(v1, v2);
  g.insert_edge(v2, v3);
  g.insert_edge(v2, v1);
  g.insert_edge(v2, v0);

  assert(g.get_vertices_size() == 4);
  assert(g.get_edges_size() == 5);

  assert(g.get_neighbors(v1).size() == 1);
  assert(g.get_neighbors(v2).size() == 3);
  assert(g.get_neighbors(v3).size() == 0);

  std::vector<int> path = {v0, v1, v2, v3};
  std::string spelled_sequence = g.spell_path(path);

  assert(spelled_sequence == "ACGT");

  std::cout << "@SequenceGraph >> test_graph()\n";
}

void test_graph2() {

  SequenceGraph g(4);

  int v0 = g.insert_vertex('A');
  int v1 = g.insert_vertex('C');
  int v2 = g.insert_vertex('G');
  int v3 = g.insert_vertex('T');

  g.insert_edge(v0, v1);
  g.insert_edge(v1, v2);
  g.insert_edge(v2, v3);
  g.insert_edge(v2, v1);
  g.insert_edge(v2, v0);

  assert(g.get_vertices_size() == 4);
  assert(g.get_edges_size() == 5);

  assert(g.get_neighbors(v1).size() == 1);
  assert(g.get_neighbors(v2).size() == 3);
  assert(g.get_neighbors(v3).size() == 0);

  std::vector<int> path = {v0, v1, v2, v3};
  std::string spelled_sequence = g.spell_path(path);

  assert(spelled_sequence == "ACGT");

  std::cout << "@SequenceGraph >> test_graph2()\n";
}

int main() {
  test_graph();
  test_graph2();
  return 0;
}
