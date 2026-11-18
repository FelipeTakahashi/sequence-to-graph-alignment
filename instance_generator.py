#!/usr/bin/env python3
import argparse
import random
import os
import hashlib
import networkx as nx

def parse_arguments():
    parser = argparse.ArgumentParser(description="Scientific GSAP-G Instance Generator.")
    parser.add_argument("--v", type=int, default=500, help="Number of vertices.")
    parser.add_argument("--q_len", type=int, default=50, help="Length of the query sequence.")
    parser.add_argument("--alphabet", type=int, default=4, help="Alphabet size (e.g., 4 for ACGT).")
    parser.add_argument("--model", type=str, choices=['er', 'ba', 'ws'], default='er',
                        help="Graph model: er (Erdos-Renyi), ba (Barabasi-Albert), ws (Watts-Strogatz).")
    parser.add_argument("--density", type=float, default=0.01,
                        help="(er) Probability for edge creation.")
    parser.add_argument("--m", type=int, default=3,
                        help="(ba) Number of edges to attach from a new node to existing nodes.")
    parser.add_argument("--k", type=int, default=10,
                        help="(ws) Each node is joined with its k nearest neighbors in a ring topology.")
    parser.add_argument("--p", type=float, default=0.1,
                        help="(ws) The probability of rewiring each edge.")
    parser.add_argument("--seed", type=int, default=None, help="Random seed / sample index for unique instances.")
    parser.add_argument("--out_dir", type=str, default="data/in/instances", help="Output directory.")
    return parser.parse_args()

def generate_topology(args):
    if args.model == 'er':
        return nx.erdos_renyi_graph(args.v, args.density, seed=args.seed)
    elif args.model == 'ba':
        if args.m >= args.v:
            raise ValueError("Parameter 'm' must be strictly less than the number of vertices 'v'.")
        return nx.barabasi_albert_graph(args.v, args.m, seed=args.seed)
    elif args.model == 'ws':
        if args.k >= args.v:
            raise ValueError("Parameter 'k' must be strictly less than the number of vertices 'v'.")
        return nx.watts_strogatz_graph(args.v, args.k, args.p, seed=args.seed)

def apply_random_orientation(undirected_graph):
    directed_graph = nx.DiGraph()
    directed_graph.add_nodes_from(undirected_graph.nodes())
    
    for u, v in undirected_graph.edges():
        coin = random.random()
        if coin < 0.45:
            directed_graph.add_edge(u, v)
        elif coin < 0.90:
            directed_graph.add_edge(v, u)
        else:
            directed_graph.add_edge(u, v)
            directed_graph.add_edge(v, u)
    return directed_graph

def generate_labels_and_query(v_size, q_len, alphabet_size):
    alphabet = [chr(97 + i) for i in range(alphabet_size)]
    labels = [random.choice(alphabet) for _ in range(v_size)]
    query = "".join(random.choice(alphabet) for _ in range(q_len))
    return labels, query

def get_instance_hash(args):
    hash_str = f"{args.model}_{args.v}_{args.q_len}_{args.alphabet}"
    if args.model == 'er':
        hash_str += f"_{args.density}"
    elif args.model == 'ba':
        hash_str += f"_{args.m}"
    elif args.model == 'ws':
        hash_str += f"_{args.k}_{args.p}"
    if args.seed is not None:
        hash_str += f"_seed{args.seed}"
        
    return hashlib.md5(hash_str.encode()).hexdigest()[:8]

def export_instance(graph, labels, query, args):
    os.makedirs(args.out_dir, exist_ok=True)
    v_size = graph.number_of_nodes()
    e_size = graph.number_of_edges()
    hash_tag = get_instance_hash(args)
    filename = f"{args.model}_v{v_size}_q{args.q_len}_a{args.alphabet}_{hash_tag}.txt"
    filepath = os.path.join(args.out_dir, filename)
    with open(filepath, 'w') as f:
        f.write(f"{v_size} {e_size}\n")
        for label in labels:
            f.write(f"{label}\n")
        for u, v in graph.edges():
            f.write(f"{u} {v}\n")
        f.write(f"{query}\n")

def main():
    args = parse_arguments()
    if args.seed is not None:
        random.seed(args.seed)
    undirected_graph = generate_topology(args)
    directed_graph = apply_random_orientation(undirected_graph)
    labels, query = generate_labels_and_query(args.v, args.q_len, args.alphabet)
    export_instance(directed_graph, labels, query, args)

if __name__ == "__main__":
    main()
