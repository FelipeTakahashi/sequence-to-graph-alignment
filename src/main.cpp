#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/heuristic/BeamSearch.hpp"
#include "../include/heuristic/Brkga.hpp"
#include "../include/heuristic/Greedy.hpp"
#include "../include/utils/Parser.hpp"

std::string get_arg(int argc, char *argv[], const std::string &arg_name) {
  for (int i = 1; i < argc - 1; ++i) {
    if (arg_name == argv[i]) {
      return argv[i + 1];
    }
  }
  return "";
}

int main(int argc, char *argv[]) {
  std::string instance_path = get_arg(argc, argv, "--instance");
  std::string approach = get_arg(argc, argv, "--approach");
  std::string out_path = get_arg(argc, argv, "--out");

  if (instance_path.empty() || approach.empty() || out_path.empty()) {
    std::cerr << "Usage: " << argv[0]
              << " --instance <path> --approach <greedy|beam|brkga> "
                 "--out <path>\n";
    return 1;
  }

  try {
    Instance instance = InstanceParser::parse(instance_path);
    int distance = -1;
    std::vector<int> path;

    if (approach == "greedy") {
      GreedyAnswer ans = Greedy::solve(instance.graph, instance.query);
      distance = ans.distance;
      path = ans.path;
    } else if (approach == "beam") {
      int beam_width = 3;
      BeamSearchAnswer ans =
          BeamSearch::solve(instance.graph, instance.query, beam_width);
      distance = ans.distance;
      path = ans.path;
    } else if (approach == "brkga") {
      BrkgaAnswer ans = BrkgaSolver::solve(instance.graph, instance.query, 100);
      distance = ans.distance;
      path = ans.path;
    } else {
      std::cerr << "Error: Unknown approach '" << approach << "'\n";
      return 1;
    }

    std::ofstream out_file(out_path);
    if (!out_file.is_open()) {
      std::cerr << "Error: Could not open output file: " << out_path << "\n";
      return 1;
    }

    out_file << "Instance: " << instance_path << "\n";
    out_file << "Approach: " << approach << "\n";
    out_file << "Distance: " << distance << "\n";
    out_file << "Path: ";

    if (path.empty()) {
      out_file << "NONE (No valid path found)";
    } else {
      for (int v : path) {
        out_file << v << " ";
      }
    }
    out_file << "\n";
    out_file.close();

  } catch (const std::exception &e) {
    std::cerr << "Fatal Error during execution: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
