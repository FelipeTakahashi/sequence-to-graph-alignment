#include "../../include/utils/HammingDistance.hpp"
#include <limits>

int Distance::distance(const std::string &p, const std::string &q) {
  if (p.length() != q.length()) {
    return std::numeric_limits<int>::max();
  }

  int d = 0;

  for (size_t i = 0; i < q.length(); i++)
    if (p[i] != q[i])
      d++;

  return d;
}
