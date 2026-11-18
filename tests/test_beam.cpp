#include "../include/heuristic/BeamSearch.hpp"
#include "../include/utils/Parser.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void test_beam() {
  std::string filepath = "beam_test.txt";

  std::ofstream out(filepath);
  out << "8 8\n";
  out << "a\nb\nb\nb\nc\na\na\nc\n";
  out << "0 1\n0 7\n1 2\n1 3\n2 4\n2 5\n3 6\n 6 7\n";
  out << "abbc";
  out.close();

  Instance instance_beam = InstanceParser::parse(filepath);

  BeamSearchAnswer ans =
      BeamSearch::solve(instance_beam.graph, instance_beam.query, 3);

  std::cout << "Path: ";
  assert(ans.distance == 0);
  for (auto v : ans.path) {
    std::cout << v << " ";
  }
  std::cout << "\nDistance: " << ans.distance << "\n";

  std::filesystem::remove(filepath);

  std::cout << "@Greedy >> test_beam()\n";
}

int main() {
  test_beam();
  return 0;
}
