import networkx as nx
import sys

network = str(sys.argv[1])

G = nx.read_edgelist(network + ".edges")

degree_sequence = sorted((d for n, d in G.degree()), reverse=True)
dmax = max(degree_sequence)
print("Max deg: " + str(dmax))

