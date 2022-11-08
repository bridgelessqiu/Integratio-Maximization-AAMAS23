import algo
import numpy as np
import scipy as sp
import networkx as nx
import sys

if __name__ == "__main__":
    # --------------------------- #
    #     Command line inputs     #
    # --------------------------- #
    network_name = str(sys.argv[1])
    k = int(sys.argv[2])
    
    # ---------------------- #
    #      Path to data      #
    # ---------------------- #
    # The path to the network file
    edgelist_path = "../network/random/" + network_name + ".edges"

    print("Network: {}".format(network_name))
    
    # ------------------------------- #
    #        Network preporcessing    #
    # ------------------------------- #
    # Read in the network
    G = nx.read_edgelist(edgelist_path)
    n = G.number_of_nodes() 

    # the ordering of the node in the matrix (THIS IS VERY VERY IMPORTNAT)
    node_order = [str(i) for i in range(n)]
 
    # The adjacency matrix
    A = nx.to_scipy_sparse_matrix(G, nodelist = node_order)

    # double check the ordering
    for u in range(n):
        if(G.degree(str(u)) != A[u].count_nonzero()):
            print("wrong")
            print(G.degree(str(u)), A[u].count_nonzero())
    
    # ------------------------ #
    #       Run the solver     #
    # ------------------------ #
    obj = algo.ip_IOA(A, k)

    file1 = open(network_name + "_results.txt", 'w') 

    file1.write("IOA: " + str(obj) + '\n')

    file1.close()

