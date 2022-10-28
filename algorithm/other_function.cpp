#include "other_function.h"
#include "graph.h"
#include<list>
#include<fstream>
#include<limits>
#include<cmath>
#include<vector>
#include<iostream>
#include<string>
#include<unordered_map>

using namespace std;

//-------------------------------
//       Float comparision      -
//-------------------------------
bool float_equal(double d1, double d2)
{
    return ((fabs(d1 - d2) <= std::numeric_limits<double>::epsilon()));
}

// ---------------------------
//     Read in networks      -
//----------------------------
Graph construct_network(string network_file_name)
{
    ifstream network_file(network_file_name); //The file object 

    if(network_file.fail()) // If no such a file can be found
    {
        cerr<<"File: "<<network_file_name<<" cannot be found\n"; 
        Graph emp;
        return emp;
    }
    
    // Characterize a nework
    unordered_map<int, list<int>> neighbor_map; // Format: {node_id : [list of ids of neighbors]}
    unordered_map<string, int> name_id_mapping; // Format: {node_name : node_id}
    
    // The node names of u and v
    string u_name, v_name;
    while(network_file>>u_name>>v_name) // assume that nodes are separated by a single whitespace
    {
        // Assign id to the node u (if it has not been assigned)
        name_id_mapping.insert(unordered_map<string, int>::value_type(u_name, (int)name_id_mapping.size()));
        int u_id = name_id_mapping[u_name];

        // Create spaces for u's neighbors (if it has not been created) 
        list<int> u_neighbors;
        neighbor_map.insert(unordered_map<int, list<int>>::value_type(u_id, u_neighbors));

        // Assign id to the node v (if it has not been assigned)
        name_id_mapping.insert(unordered_map<string, int>::value_type(v_name, (int)name_id_mapping.size()));
        int v_id = name_id_mapping[v_name];

        // Create spaces for v's neighbors (if it has not been created)
        list<int> v_neighbors;
        neighbor_map.insert(unordered_map<int, list<int>>::value_type(v_id, v_neighbors));

        // recored the edge u v
        neighbor_map[u_id].push_back(v_id);
        neighbor_map[v_id].push_back(u_id);
    }

    int n = neighbor_map.size(); // number of nodes
    int m = 0; // number of edges

    vector<std::vector<int>> neighbors(n); // [[neighbors of node 0], [neighbors of node 1], ..., [neighbors of node n-1]]

    // Assign neighbors
    for(auto map_it = neighbor_map.begin(); map_it != neighbor_map.end(); ++map_it)
    {
        int u = map_it->first; // vertex
        list<int> u_neighbors = map_it->second; // list of neighbors
        u_neighbors.sort();
        u_neighbors.unique(); // remove duplicates (if any)

        u_neighbors.remove(u); // Make sure that u is not a neighbor of itself
        
        int degree_u = u_neighbors.size();
        neighbors[u] = vector<int> (degree_u);

        m += degree_u;
        
        int v_index = 0;

        for(auto list_it = u_neighbors.begin(); list_it != u_neighbors.end(); ++list_it)
        {
            neighbors[u][v_index] = *list_it;
            v_index++;
        }
    }

    network_file.close();

    cout<<"---------------------------------\n";
    cout<<"The number of vertices: "<<n<<endl;
    cout<<"---------------------------------\n";
    cout<<"---------------------------------\n";
    cout<<"The number of edges: "<<(m / 2)<<endl;
    cout<<"---------------------------------\n";

    Graph G;
    G.neighbors = neighbors;

    return G; 
}

