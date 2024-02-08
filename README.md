
# Integration Maximization for the Agent Placement Problem
Paper: *Assigning Agents to Increase Network-Based Neighborhood Diversity*
<br/>
**Full version** (contains complete proofs of theorems): [https://arxiv.org/pdf/2301.02876.pdf]



## Directory Overview 
1. **algorithm**: contains all the code
2. **network**: contains some example networks

## To compile the code
Required software: **C++11**

Steps:
1. `cd algorithm/`
2. `g++ -std=c++11 -O3 heuristic.cpp heuristic.h other_function.cpp other_function.h graph.h IoA.cpp -o IoA`

To run the code:
`./IoA network_name algorithm_id k`

- network_name: 
    - gnp_1000_10 : gnp graph, n = 1000, avg_deg = 10
    - pl_1000_10 : power-law graph, n = 1000, avg_deg = 10
    - google_plus_subnetwork: a subgraph of google_plus
- algorithm_id: 
    - 1: local improvement
    - 2: greedy
- k: the number of type-1 agents (should not be larger than n/2)

Example 1: run local improvement on google_plus_subnetwork with k = 100
- `./IoA google_plus_subnetwork 1 100`

Example 2: run greedy on gnp_1000_10 with k = 100
- `./IoA gnp_1000_10 2 100`
