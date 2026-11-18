# Sequence-to-Graph Alignment with Simple Path and Label Substitution (SGAP)
This work investigates _Sequence-to-Graph Alignment_, a well-known problem in Bioinformatics. The problem has multiple variants, with each presenting its own restrictions (e.g., edits or substitution; a path or a walk; cyclic or acyclic graphs). For a better understanding of the problem, we suggest reading Jain _et al_. (Jain _et al_., 2019).

Very informally, we state: given a **sequence graph** $G(V,E,\sigma)$ with vertices $v\in V$ labeled over $\Sigma$, and a query sequence $q$, minimize the number of label **substitutions** in the graph such that there exists a simple path $p$ that spells $q$. Do note that this problem is equivalent to minimizing the substitution (Hamming) distance from $p$ to $q$. It is possible to show that this variant is NP-hard through a reduction of the Directed Hamiltonian Path.   

## Content
We present three heuristics ([Greedy](placeholder), [Beam Search](placeholder), [BRKGA](placeholder)) for SGAP.

### Greedy
A multistart heuristic and relies on greedily selecting the neighbor that matches the next symbol of the query, adopting vertex out-degree as a tiebreaker.

### Beam Search
A generalization of the Greedy heuristic, capable of exploring multiple paths simultaneously (limited by $\beta$). 

### BRKGA
Relying on `brkga_mp_ipr` (Andrade _et al_., 2021), the decoder adopts a score function that adds a $\lambda$ value to a vertex key if it matches the next symbol of the query. The heuristic starts at the vertex of maximum global score and expands the path by computing the score of each neighbor, also maximizing score. 

## Implementation
As for the source code, **heuristics** are written in `C++20` and understanding is prioritized over complexity. The code tries, to its best, to adopt Structure of Arrays (SoA) and Object Oriented Programming (OOP). 

## Input
The input file must contain a **sequence graph** and a **query**. As is, the project contains a **parser** that parses:
```
<number of vertices> <number of edges>
<labels>
<edges>
<query>  
```
The parser can be easily modified at [```Parser.hpp```](placeholder) and [```Parser.cpp```](placeholder).

## Compiling and Executing

## Output
The output of a given instance is the path $p$ and the substitution distance $d(p,q)$. 

## Note 
A monograph of this work, with proper explanations, is available in [Portuguese](placeholder).

## References
[1] C. Jain, H. Zhang, Y. Gao, and S. Aluru. On the complexity of sequence to
graph alignment. bioRxiv, 2019.

[2] C. E. Andrade, R. F. Toso, J. F. Gon¸calves, and M. G. C. Resende. The multi
parent biased random-key genetic algorithm with implicit path-relinking and its
real-world applications. European Journal of Operational Research, 289(1):17
30, 2021.
