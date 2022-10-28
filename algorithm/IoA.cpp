#include "other_function.h"
#include "heuristic.h"
#include "graph.h"
#include<algorithm>
#include<math.h>
#include<iostream> 
#include<cstdlib>
#include<unordered_map>
#include<string>
#include<vector>
#include<random>
#include<fstream>

using namespace std;

int main(int argc, char *argv[])
{   
    string network_name = argv[1]; // arena, google_plus, astroph
    int algo_type = atoi(argv[2]);
    int k = atoi(argv[3]);

    string network_file = "../network/" + network_name + ".edges";
    
    Graph G;
    G = construct_network(network_file); 

    int obj;
    
    if(algo_type == 1)
    {
        obj = local_improvement(G.neighbors, k);
        cout<<"-----------------------------"<<endl;
        cout<<"-     Local Improvement     -"<<endl;
        cout<<"-----------------------------"<<endl;
    }
    else if(algo_type == 2)
    {
        obj = greedy(G.neighbors, k);
        cout<<"-------------------"<<endl;
        cout<<"-      Greedy     -"<<endl; 
        cout<<"-------------------"<<endl;
    }
    else
    {
        cerr<<"Unknow algorithm type"<<endl;
        return -1;
    }

    cout<<"IoA: "<<obj<<endl;

    return 0;
}
