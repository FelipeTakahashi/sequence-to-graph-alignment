#!/usr/bin/env python3
import sys
from ortools.linear_solver import pywraplp


def parse_instance(filepath):
    """Parses the GSAP-G instance file format."""
    with open(filepath, "r") as f:
        lines = [line.strip() for line in f if line.strip()]

    v_size, e_size = map(int, lines[0].split())
    labels = lines[1 : v_size + 1]

    edges = []
    edge_start_idx = v_size + 1
    for i in range(e_size):
        u, v = map(int, lines[edge_start_idx + i].split())
        edges.append((u, v))

    query = lines[-1]
    return v_size, edges, labels, query


def solve_exact(filepath):
    v_size, edges, labels, query = parse_instance(filepath)
    q_len = len(query)

    predecessors = {v: [] for v in range(v_size)}
    for u, v in edges:
        predecessors[v].append(u)

    mismatch_indices = {v: [] for v in range(v_size)}
    for v in range(v_size):
        for i in range(q_len):
            if labels[v] != query[i]:
                mismatch_indices[v].append(i)

    solver = pywraplp.Solver.CreateSolver("SCIP")
    if not solver:
        print("SCIP solver not available.")
        return None, []

    y = {}
    for v in range(v_size):
        for i in range(q_len):
            y[v, i] = solver.IntVar(0, 1, f"y_{v}_{i}")

    S = {}
    for v in range(v_size):
        S[v] = solver.IntVar(0, 1, f"S_{v}")

    objective = solver.Objective()
    for v in range(v_size):
        objective.SetCoefficient(S[v], 1)
    objective.SetMinimization()

    for i in range(q_len):
        ct = solver.Constraint(1, 1, f"seq_assign_{i}")
        for v in range(v_size):
            ct.SetCoefficient(y[v, i], 1)

    for v in range(v_size):
        for i in range(q_len - 1):
            ct = solver.Constraint(-solver.infinity(), 0, f"topo_{v}_{i}")
            ct.SetCoefficient(y[v, i + 1], 1)
            for u in predecessors[v]:
                ct.SetCoefficient(y[u, i], -1)

    for v in range(v_size):
        ct = solver.Constraint(0, 1, f"simple_path_{v}")
        for i in range(q_len):
            ct.SetCoefficient(y[v, i], 1)

    for v in range(v_size):
        ct = solver.Constraint(0, solver.infinity(), f"subst_{v}")
        ct.SetCoefficient(S[v], 1)
        for i in mismatch_indices[v]:
            ct.SetCoefficient(y[v, i], -1)

    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL or status == pywraplp.Solver.FEASIBLE:
        distance = int(objective.Value())

        path = [-1] * q_len
        for i in range(q_len):
            for v in range(v_size):
                if y[v, i].solution_value() > 0.5:
                    path[i] = v
                    break
        return distance, path
    else:
        return None, []


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 exact_solver.py <instance_file.txt>")
        sys.exit(1)

    filepath = sys.argv[1]
    dist, path = solve_exact(filepath)

    if dist is not None:
        print(f"Optimal Distance: {dist}")
        print(f"Path: {' '.join(map(str, path))}")
    else:
        print("No valid path exists.")
