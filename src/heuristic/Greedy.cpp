#include "../../include/heuristic/Greedy.hpp"

GreedyAnswer Greedy::solve(const SequenceGraph &graph,
                           const std::string &query) {
  int distance = std::numeric_limits<int>::max();
  std::vector<int> path;

  int v_size = graph.get_vertices_size();
  int q_len = query.length();

  if (q_len == 0)
    return {path, 0};

  for (int start = 0; start < v_size; start++) {
    std::vector<int> current_path;
    current_path.reserve(q_len);
    int current_distance = 0;

    std::vector<bool> visited(v_size, false);

    current_path.push_back(start);
    visited[start] = true;

    if (graph.get_label(start) != query[0])
      current_distance++;
    bool invalid = false;

    for (auto i = 1; i < q_len; i++) {
      int current = current_path.back();
      const auto &neighbors = graph.get_neighbors(current);

      bool best_fit = false;
      int best_degree = -1;
      int best_next = -1;

      for (int neighbor : neighbors) {
        if (visited[neighbor])
          continue;

        bool _label = (graph.get_label(neighbor) == query[i]);
        int _degree = graph.get_neighbors(neighbor).size();

        bool better_current = false;

        if (best_next == -1) {
          better_current = true;
        } else if (_label && !best_fit) {
          better_current = true;
        } else if (_label == best_fit) {
          if (_degree > best_degree)
            better_current = true;
        }

        if (better_current) {
          best_fit = _label;
          best_degree = _degree;
          best_next = neighbor;
        }
      }

      if (best_next == -1) {
        invalid = true;
        break;
      }

      current_path.push_back(best_next);
      visited[best_next] = true;

      if (!best_fit) {
        current_distance++;
      }
    }
    if (!invalid && current_path.size() == q_len) {
      if (current_distance < distance) {
        path = current_path;
        distance = current_distance;

        if (distance == 0)
          break;
      }
    }
  }

  return {path, distance};
}
