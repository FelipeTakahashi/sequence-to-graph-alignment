#include "../include/utils/Parser.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void test_parser() {

  std::string filepath = "parser_test_file.txt";

  std::ofstream out(filepath);
  out << "4 6\n";
  out << "A\n";
  out << "C\n";
  out << "G\n";
  out << "T\n";
  out << "0 1\n";
  out << "1 2\n";
  out << "2 3\n";
  out << "3 2\n";
  out << "3 1\n";
  out << "3 0\n";
  out << "ACGT\n";
  out.close();

  Instance a = InstanceParser::parse(filepath);

  assert(a.graph.get_vertices_size() == 4);
  assert(a.graph.get_edges_size() == 6);
  assert(a.graph.get_label(0) == 'A');
  assert(a.graph.get_label(1) == 'C');
  assert(a.graph.get_label(2) == 'G');
  assert(a.graph.get_label(3) == 'T');

  const auto &neighbors0 = a.graph.get_neighbors(0);
  const auto &neighbors1 = a.graph.get_neighbors(1);
  const auto &neighbors2 = a.graph.get_neighbors(2);
  const auto &neighbors3 = a.graph.get_neighbors(3);

  assert(neighbors0.size() == 1);
  assert(neighbors1.size() == 1);
  assert(neighbors2.size() == 1);
  assert(neighbors3.size() == 3);

  assert(neighbors0[0] == 1);
  assert(neighbors1[0] == 2);
  assert(neighbors2[0] == 3);
  assert(neighbors3[0] == 2);
  assert(neighbors3[1] == 1);
  assert(neighbors3[2] == 0);

  assert(a.query == "ACGT");

  std::filesystem::remove(filepath);

  std::cout << "@Parser >> test_parser()\n";
}

int main() {
  test_parser();
  return 0;
}
