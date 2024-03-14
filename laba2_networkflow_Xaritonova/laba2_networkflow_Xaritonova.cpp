#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max(); // Using infinity

// Function to find the maximum flow in the graph using the Ford-Fulkerson method
int Flow(vector<vector<int>>& graph, int source, int outlet, int n) {
    vector<vector<int>> residualGraph(graph); // Residual network
    vector<vector<int>> backwardGraph(n, vector<int>(n, 0)); // Backward edges graph
    int max_flow = 0; // Initializing maximum flow
    int total_flow = 0; // Total flow

    // Finding the maximum flow
    int new_flow;
    while (true) {
        vector<int> parent(n); // For storing the path from source to outlet
        // Lambda function search implements path finding using breadth-first search
        auto search = [&](int source, int east, vector<int>& parent) {
            // Initializing the path array
            fill(parent.begin(), parent.end(), -1);
            // Setting the parent of the source as -2
            parent[source] = -2;
            // Queue for breadth-first search, containing pairs of vertex-flow
            queue<pair<int, int>> q;
            // Adding the source to the queue with maximum flow INF
            q.push({ source, INF });
            while (!q.empty()) {
                int current = q.front().first;
                int flow = q.front().second;
                q.pop();
                // Iterating over all adjacent vertices
                for (int next = 0; next < n; ++next) {
                    // If the vertex is not visited yet and there is an edge with non-zero capacity
                    if (parent[next] == -1 && residualGraph[current][next] > 0) {
                        // Calculating the new flow through this edge
                        int new_flow = min(flow, residualGraph[current][next]);
                        // Setting the current vertex as the parent for the next vertex
                        parent[next] = current;
                        // If we reach the outlet, return the new flow
                        if (next == east) {
                            return new_flow;
                        }
                        // Adding the next vertex to the queue with the new flow
                        q.push({ next, new_flow });
                    }
                }
            }
            return 0;
            };
        // If search returns 0, it means an augmenting path couldn't be found, so we finish the search
        if ((new_flow = search(source, outlet, parent)) == 0) {
            break;
        }
        // Increasing the maximum flow and total flow by the new flow
        max_flow += new_flow;
        total_flow += new_flow;
        int current = outlet;
        // Saving the path from source to outlet
        vector<int> path;
        while (current != source) {
            path.push_back(current);
            int prev = parent[current];
            // Updating the residual network
            residualGraph[prev][current] -= new_flow;
            residualGraph[current][prev] += new_flow;
            // Updating the backward edges graph
            backwardGraph[current][prev] += new_flow;
            current = prev;
        }
        path.push_back(source);

        // Printing the selected path
        cout << "Selected path: ";
        for (int i = path.size() - 1; i > 0; --i) {
            cout << path[i] + 1 << " -> ";
        }
        cout << path[0] + 1 << endl;
        // Printing the minimum element that we add up
        cout << "Minimum element to add: " << new_flow << endl;
        // Printing the current total flow
        cout << "Current total flow: " << total_flow << endl;
        // Printing the final state of the residual graph
        cout << "Current state of the residual graph:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << residualGraph[i][j] << " ";
            }
            cout << endl;
        }
        // Printing the final state of the backward edges graph
        cout << "\nCurrent state of the backward edges graph:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << backwardGraph[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\n/---------------------------------------------------------------------/\n";
    }
    return max_flow;
}

int main() {
    int n; // Number of vertices in the graph
    // Repeat asking for the number of vertices until a valid input is received
    while (true) {
        cout << "Enter the number of vertices in the graph: "; cin >> n;
        // Validate input for the number of vertices
        if (n <= 0) {
            cout << "Invalid number of vertices. Try again." << endl;
        }
        else {
            break;
        }
    }
    int source, outlet; // Source and outlet
    // Repeat asking for the source and outlet until valid inputs are received
    while (true) {
        cout << "Enter the source and outlet: "; cin >> source >> outlet;
        // Validate input for the source and outlet
        if (source <= 0 || source > n || outlet <= 0 || outlet > n) {
            cout << "Invalid source or outlet. Try again." << endl;
        }
        else {
            break;
        }
    }
    vector<vector<int>> graph(n, vector<int>(n)); // Adjacency matrix of the graph
    // Repeat asking for the adjacency matrix until a valid input is received
    while (true) {
        cout << "Enter the adjacency matrix of the graph:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> graph[i][j];
                // Validate input for non-negative weights
                if (graph[i][j] < 0) {
                    cout << "Invalid weight. Please enter non-negative weights." << endl;
                    break;
                }
            }
        }
        break; // Break the loop if the input is valid
    }
    cout << "\n/---------------------------------------------------------------------/\n";
    int max_flow = Flow(graph, source - 1, outlet - 1, n); // Find the maximum flow
    cout << "The maximum flow from source " << source << " to outlet " << outlet << " is: " << max_flow << endl;

    return 0;
}