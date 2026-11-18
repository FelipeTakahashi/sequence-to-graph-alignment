#include "../include/heuristic/Brkga.hpp"
#include "../include/utils/Parser.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void test_brkga() {
  std::string filepath = "brkga_test.txt";

  std::ofstream out(filepath);
  out << "8 8\n";
  out << "a\nb\nb\nb\nc\na\na\nc\n";
  out << "0 1\n0 7\n1 2\n1 3\n2 4\n2 5\n3 6\n 6 7\n";
  out << "abbc";
  out.close();

  Instance instance_brkga = InstanceParser::parse(filepath);

  BrkgaAnswer ans =
      BrkgaSolver::solve(instance_brkga.graph, instance_brkga.query, 200);

  std::cout << "Path: ";
  for (auto v : ans.path) {
    std::cout << v << " ";
  }
  std::cout << "\nDistance: " << ans.distance << "\n";

  assert(ans.distance == 0);
  assert(ans.path.size() == instance_brkga.query.size());

  std::filesystem::remove(filepath);
  std::cout << "@Brkga >> test_brkga()\n";
}

int main() {
  test_brkga();
  return 0;
}
