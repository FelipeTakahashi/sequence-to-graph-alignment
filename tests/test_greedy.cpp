#include "../include/heuristic/Greedy.hpp"
#include "../include/utils/Parser.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void test_greedy() {
  std::string filepath = "greedy_test.txt";

  std::ofstream out(filepath);
  out << "8 8\n";
  out << "a\nb\nb\nb\nc\na\na\nc\n";
  out << "0 1\n0 7\n1 2\n1 3\n2 4\n2 5\n3 6\n 6 7\n";
  out << "abba";
  out.close();

  Instance instance_greedy = InstanceParser::parse(filepath);

  GreedyAnswer ans =
      Greedy::solve(instance_greedy.graph, instance_greedy.query);

  std::vector<int> valid = {0, 1, 2, 5};

  assert(valid.size() == ans.path.size());
  for (auto i = 0lu; i < valid.size(); i++) {
    assert(valid[i] == ans.path[i]);
  }
  assert(ans.distance == 0);

  std::filesystem::remove(filepath);

  std::cout << "@Greedy >> test_greedy()\n";
}

int main() { test_greedy(); }
