#include "heuristic.h"
#include "graph.h"
#include<list>
#include<queue>
#include<unordered_map>
#include <chrono>
#include <set>
#include<cstdlib>
#include<random>
#include<numeric>
#include<algorithm>
#include<vector>
#include<iostream>

using namespace std;

/* ------------------------------ *
 * The local improvment algorithm *
 * ------------------------------ */

int local_improvement(const vector<vector<int>>& neighbors, int k) // k is the number of type-1 vertices (agents)
{
    vector<int> store_obj;

    int obj = 0; // the objective value
    int n = neighbors.size(); // number of vertices (agents)
    
    // Generate an inital random assignment with k 1's and n-k 2's
    vector<int> assignment (n, 2); // assignment[i] is the type (1 or 2) of agents occupying the node i

    int num_of_type_1 = 0; 

    random_device rd; 
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(0, n-1);

    while(num_of_type_1 < k)
    {
        int v = uni(rng);
        if(assignment[v] != 1)
        {
            assignment[v] = 1;
            num_of_type_1++;
        }
    }

    vector<int> Nd (n, 0); // Nd[i]: the number of different-type neighbors of vertex i
    vector<int> Nd_temp (n, 0); // Temporay vector

    // Extract the initial set of type-1 and type-2 vertices (order doesn't matter)
    vector<int> type_1_set(k);
    vector<int> type_2_set(n - k);

    int ii = 0;
    int jj = 0;
    for(int u = 0; u < n; ++u)
    {
        for(auto v : neighbors[u])
        {
            if(assignment[v] != assignment[u]) Nd[u]++;
        }

        if(assignment[u] == 1)
        {
            type_1_set[ii] = u;  
            ii++;
        }
        else
        {
            type_2_set[jj] = u;
            jj++;
        }
    }
    

    // ------- Start the local improvemenet process ------- //
    bool terminate = false;
    set<int> nodes; // used to store vertices in the closed neighborhood of a pair of vertices

    while(!terminate)
    {
        terminate = true;

        for(int i = 0; i < k; ++i)
        {
            int u = type_1_set[i];
            
            for(int j = 0; j < n-k; ++j)
            {
                nodes.clear();
                int v = type_2_set[j]; 
                
                // Union of two closed neighborhood: the set of vertices are affected by the swap
                nodes.insert(u);
                nodes.insert(v);
                for(auto w : neighbors[u]) nodes.insert(w);
                for(auto w : neighbors[v]) nodes.insert(w);

                // Compute the objective before the swap
                int initial_obj = 0;

                // Compute the objective after the swap
                int new_obj = 0;

                // Update neighbors first
                for(auto w : neighbors[v])
                {
                    if(assignment[v] == assignment[w]) Nd_temp[w] = Nd[w] + 1;
                    else Nd_temp[w] = Nd[w] - 1;
                }

                for(auto w : neighbors[u])
                {
                    if(assignment[u] == assignment[w]) Nd_temp[w] = Nd[w] + 1;
                    else 
                    {
                        Nd_temp[w] = Nd[w] - 1;
                    }

                    if(find(neighbors[v].begin(), neighbors[v].end(), w) != neighbors[v].end()) Nd_temp[w] = Nd[w];
                }

                if(find(neighbors[v].begin(), neighbors[v].end(), u) != neighbors[v].end()) // if u and v are neighbors
                {
                    Nd_temp[u] = neighbors[u].size() - Nd[u] + 1;        
                    Nd_temp[v] = neighbors[v].size() - Nd[v] + 1;        
                }

                else
                {
                    Nd_temp[u] = neighbors[u].size() - Nd[u];
                    Nd_temp[v] = neighbors[v].size() - Nd[v];        
                }

                for(auto w : nodes)
                {
                    if(Nd[w] > 0) initial_obj++;
                    if(Nd_temp[w] > 0) new_obj++;
                }

                // Check if the swap is good
                if(new_obj > initial_obj) // make the swap happen
                {
                    assignment[u] = 2; 
                    assignment[v] = 1; 
                    type_1_set[i] = v;
                    type_2_set[j] = u;

                    for(auto w : nodes)
                    {
                        Nd[w] = Nd_temp[w];    
                    }

                    terminate = false;  
                    break;
                }
            }   
        }
    }
    
    obj = 0;

    for(int u = 0; u < n; ++u)
    {
        if(Nd[u] > 0) 
        {
            obj++;
        }
    }

    return obj;
}

int myrandom (int i) { return std::rand()%i;}

// ---------- The Greedy Approach --------------- //
int greedy(const vector<vector<int>>& neighbors, int k)
{
    int obj = 0;

    int n = neighbors.size(); // number of vertices (agents)
    
    // Generate an assignment based on the greedy solution
    vector<int> assignment (n, 2); // assignment[i] is the type (1 or 2) of agents occupying the node i

    int total_type_1_placed = 0;

    int selected_node;
    int max_diff;
    
    vector<int> Nd (n, 0); // Nd[i]: the number of different-type neighbors of vertex i

    vector<int> vertex_order(n);
    iota(vertex_order.begin(), vertex_order.end(), 0); //[0, 1, ..., n-1]
    random_device rd;
    mt19937 g(rd());
    shuffle(vertex_order.begin(), vertex_order.end(), g);

    while(total_type_1_placed != k)
    {
        max_diff = -n;
        for(auto u : vertex_order)
        {
            if(assignment[u] == 2)
            {
                // First compute the obj before the placement
                int initial_obj = 0;              
                if(Nd[u] > 0) initial_obj++;
                for(auto v : neighbors[u])
                {
                    if(Nd[v] > 0) initial_obj++;
                }
                
                // Obj after change u from type 2 to type 1
                int new_obj = 0;
                if((neighbors[u].size() - Nd[u]) > 0) new_obj++;
                for(auto v : neighbors[u])
                {
                    if(assignment[v] == 2) new_obj++;
                    else
                    {
                        if((Nd[v] - 1) > 0) new_obj++;
                    }
                }

                if((new_obj - initial_obj) > max_diff)
                {
                    max_diff = new_obj - initial_obj;
                    selected_node = u;
                }
            }
        }
        
        // Update the assignment
        assignment[selected_node] = 1;
        Nd[selected_node] = 0;

        for(auto v : neighbors[selected_node])
        {
            if(assignment[v] == assignment[selected_node]) Nd[v]--;
            else
            {
                Nd[v]++;
                Nd[selected_node]++;
            }
        }

        total_type_1_placed++;
    }

    for(int u = 0; u < n; ++u)
    {
        for(auto v : neighbors[u])
        {
            if(assignment[v] != assignment[u])
            {
                obj++;
                break;
            }
        }
    }

    return obj;
}

int random_assignment(const vector<vector<int>>& neighbors, int k)
{
    int obj = 0; // the objective value
    int n = neighbors.size(); // number of vertices (agents)
    
    // Generate an inital random assignment with k 1's and n-k 2's
    vector<int> assignment (n, 2); // assignment[i] is the type (1 or 2) of agents occupying the node i

    int num_of_type_1 = 0; 

    random_device rd; 
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(0, n-1);

    while(num_of_type_1 < k)
    {
        int v = uni(rng);
        if(assignment[v] != 1)
        {
            assignment[v] = 1;
            num_of_type_1++;
        }
    }

    int num;
    for(int u = 0; u < n; ++u)
    {
        num = 0;
        for(auto v : neighbors[u])
        {
            if(assignment[v] != assignment[u]) num++;
        }

        if(num > 0) obj++;
    }

    return obj;
}
