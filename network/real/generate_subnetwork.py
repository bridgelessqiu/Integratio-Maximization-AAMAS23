import networkx as nx
import numpy as np
import sys

network_name = str(sys.argv[1])

G = nx.read_edgelist(network_name + ".edges", data=False)

sublist = [str(i) for i in range(8000)]
G = G.subgraph(sublist)

lcc = max(nx.connected_components(G), key=len)
G = G.subgraph(lcc)
G = nx.convert_node_labels_to_integers(G) 

print(nx.info(G))

path  = network_name + "_subnetwork.edges"

nx.write_edgelist(G, path, data = False)
