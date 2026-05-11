// Parallel BFS and DFS using OpenMP
// HPC Practical Program

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

const int MAX = 100;

vector<int> graph[MAX];
bool visited[MAX];

// ---------------- PARALLEL BFS ----------------
void parallelBFS(int start)
{
    queue<int> q;

    q.push(start);
    visited[start] = true;

    cout << "\nParallel BFS Traversal: ";

    while (!q.empty())
    {
        vector<int> currentLevel;

        // Store current level nodes
        while (!q.empty())
        {
            currentLevel.push_back(q.front());
            q.pop();
        }

        // Parallel processing
        #pragma omp parallel for
        for (int i = 0; i < (int)currentLevel.size(); i++)
        {
            int node = currentLevel[i];

            // Print safely
            #pragma omp critical
            {
                cout << node << " ";
            }

            // Visit neighbors
            for (int j = 0; j < (int)graph[node].size(); j++)
            {
                int neighbor = graph[node][j];

                #pragma omp critical
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    cout << endl;
}

// ---------------- PARALLEL DFS ----------------
void parallelDFS(int node)
{
    visited[node] = true;

    // Print safely
    #pragma omp critical
    {
        cout << node << " ";
    }

    // Parallel traversal of neighbors
    #pragma omp parallel for
    for (int i = 0; i < (int)graph[node].size(); i++)
    {
        int neighbor = graph[node][i];

        bool shouldVisit = false;

        #pragma omp critical
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                shouldVisit = true;
            }
        }

        if (shouldVisit)
        {
            parallelDFS(neighbor);
        }
    }
}

// ---------------- MAIN FUNCTION ----------------
int main()
{
    int n, e;

    cout << "===== Parallel BFS and DFS using OpenMP =====";

    cout << "\n\nEnter number of nodes: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> e;

    cout << "\nEnter edges (u v):\n";

    // Input graph
    for (int i = 0; i < e; i++)
    {
        int u, v;
        cin >> u >> v;

        // Undirected graph
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int start;

    cout << "\nEnter starting node: ";
    cin >> start;

    // Display graph
    cout << "\nGraph Representation:\n";

    for (int i = 0; i < n; i++)
    {
        cout << i << " --> ";

        for (int j = 0; j < (int)graph[i].size(); j++)
        {
            cout << graph[i][j] << " ";
        }

        cout << endl;
    }

    // ---------------- BFS ----------------
    for (int i = 0; i < MAX; i++)
        visited[i] = false;

    parallelBFS(start);

    // ---------------- DFS ----------------
    for (int i = 0; i < MAX; i++)
        visited[i] = false;

    cout << "\nParallel DFS Traversal: ";
    parallelDFS(start);

    cout << endl;

    return 0;
}


// commands to run : 1.g++ BFS_DFS.cpp -fopenmp -o BFS_DFS   
            //       2.BFS_DFS.exe

// OUTPUT
/*Enter number of nodes: 6 
Enter number of edges: 6
Enter edges (u v):
0 1
0 2
1 3
1 4
2 5
4 5
Enter starting node: 0 
Graph Representation:
0 --> 1 2 
1 --> 0 3 4
2 --> 0 5
3 --> 1 
4 --> 1 5
5 --> 2 4
Parallel BFS Traversal: 0 2 1 4 3 5
Parallel DFS Traversal: 0 2 5 4 1 3

*/