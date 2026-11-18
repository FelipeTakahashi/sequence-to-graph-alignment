#include "../include/graph/SequenceGraph.hpp"
#include "../include/utils/HammingDistance.hpp"
#include <cassert>
#include <iostream>
#include <limits>

void test_string_distance() {

  const int inf = std::numeric_limits<int>::max();

  assert(Distance::distance("ACGT", "ACGT") == 0);

  assert(Distance::distance("AAAC", "AAAA") == 1);
  assert(Distance::distance("AACG", "AAAA") == 2);
  assert(Distance::distance("ACGT", "AAAA") == 3);
  assert(Distance::distance("TTTT", "AAAA") == 4);

  assert(Distance::distance("ACG", "ACGT") == inf);
  assert(Distance::distance("ACGT", "ACG") == inf);

  std::cout << "@Distance >> test_string_distance()\n";
}

void test_path_query_distance() {
  const int inf = std::numeric_limits<int>::max();

  SequenceGraph g(4);
  int v0 = g.insert_vertex('0');
  int v1 = g.insert_vertex('1');
  int v2 = g.insert_vertex('0');
  int v3 = g.insert_vertex('1');
  g.insert_edge(v0, v1);
  g.insert_edge(v1, v2);
  g.insert_edge(v2, v3);

  std::vector<int> ans = {v0, v1, v2, v3};
  std::vector<int> fail = {v0, v1, v2};
  std::string spelled_ans = g.spell_path(ans);
  std::string spelled_fail = g.spell_path(fail);

  std::string match = "0101";
  std::string distant = "101";

  assert(Distance::distance(spelled_ans, match) == 0);
  assert(Distance::distance(spelled_ans, distant) == inf);

  assert(Distance::distance(spelled_fail, match) == inf);
  assert(Distance::distance(spelled_fail, distant) == 3);

  std::cout << "@Distance >> test_path_query_distance()\n";
}

int main() {
  test_string_distance();
  test_path_query_distance();
  return 0;
}
