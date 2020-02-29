#include <iostream>
#include <vector>
using namespace std;

// data structure to store graph edges
struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

// class to represent a graph object
class Graph
{
public:
    // construct a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges, int N)
    {
        // resize the vector to N elements of type vector<Pair>
        adjList.resize(N);

        // add edges to the directed graph
        for (auto &edge: edges)
        {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));

            // Uncomment below line for undirected graph
            // adjList[dest].push_back(make_pair(src, weight));
        }
    }
};

// print adjacency list representation of graph
void printGraph(Graph const &graph, int N)
{
    for (int i = 0; i < N; i++)
    {
        // print all neighboring vertices of given vertex
        for (Pair v : graph.adjList[i])
            cout << "(" << i << ", " << v.first <<
                 ", " << v.second << ") ";
        cout << endl;
    }
}

// Graph Implementation using STL
int main()
{
    // vector of graph edges as per above diagram.
    // Please note that initialization vector in below format will
    // work fine in C++11, C++14, C++17 but will fail in C++98.
    vector<Edge> edges =
            {
                    // (x, y, w) -> edge from x to y having weight w
                    { 1, 2, 100 }, { 1, 3, 300 }, { 2, 5, 65 },
                    { 3, 2, 50 }, { 3, 4, 150 }, { 3, 5, 25 },
                    { 4, 5, 400 }, { 5, 6, 10 }, { 6, 5, 20 },
                    { 6, 2, 200 }
            };

    // Number of nodes in the graph
    int N = 7;

    // construct graph
    Graph graph(edges, N);

    // print adjacency list representation of graph
    printGraph(graph, N);

    return 0;
}
