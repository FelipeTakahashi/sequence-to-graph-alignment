#!/bin/bash

set -e

BUILD_DIR="build"

print_usage() {
  echo "Usage: $0 [target]"
  echo ""
  echo "Available targets:"
  echo "  graph     : Build and execute test_graph"
  echo "  parser    : Build and execute test_parser"
  echo "  distance  : Build and execute test_distance"
  echo "  greedy    : Build and execute test_greedy"
  echo "  beam      : Build and execute test_beam"
  echo "  brkga     : Build and execute test_brkga"
  echo "  all       : Build all tests and run them via CTest"
  echo ""
}

if [ $# -eq 0 ]; then
  print_usage
  exit 1
fi

TARGET_ARG=$1

if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

cmake ..

execute_single_test() {
  local target_name=$1
  echo ""
  echo "      Building ${target_name}...            "
  echo ""
  cmake --build . --target "${target_name}"

  echo ""
  echo "      Executing ${target_name}...           "
  echo ""
  ./"${target_name}"
}

case "$TARGET_ARG" in
"graph")
  execute_single_test "test_graph"
  ;;
"parser")
  execute_single_test "test_parser"
  ;;
"distance")
  execute_single_test "test_distance"
  ;;
"greedy")
  execute_single_test "test_greedy"
  ;;
"beam")
  execute_single_test "test_beam"
  ;;
"brkga")
  execute_single_test "test_brkga"
  ;;
"all")
  echo ""
  echo "      Building all targets...            "
  echo ""
  cmake --build .

  echo ""
  echo "      Executing all tests via CTest...   "
  echo ""
  ctest --output-on-failure
  ;;
*)
  echo "Error: Unknown target '$TARGET_ARG'"
  print_usage
  exit 1
  ;;
esac
