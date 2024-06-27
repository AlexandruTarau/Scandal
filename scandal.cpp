#include <bits/stdc++.h>
using namespace std;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    static constexpr int NMAX = (int)1e5 + 5;
    int n, m;

    vector<int> parent, found, low_link, invited;
    vector<bool> in_stack;
    stack<int> nodes_stack;
    int cnt = 0;

    vector<int> adj[2 * NMAX];

    // Negating a friend means that they are not invited to the party
    int neg(int x) {
        return x > n ? x - n : x + n;
    }

    void read_input() {
        ifstream fin("scandal.in");
        fin >> n >> m;

        for (int i = 1, x, y, c; i <= m; i++) {
            fin >> x >> y >> c;

            switch (c) {
                case 0: {
                    // x || y  <=>  !x -> y && !y -> x
                    adj[neg(x)].push_back(y);
                    adj[neg(y)].push_back(x);
                    break;
                }
                case 1: {
                    // !x -> !y || y -> x
                    adj[neg(x)].push_back(neg(y));
                    adj[y].push_back(x);
                    break;
                }
                case 2: {
                    // !y -> !x || x -> y
                    adj[neg(y)].push_back(neg(x));
                    adj[x].push_back(y);
                    break;
                }
                case 3: {
                    // !x || !y  <=>  x -> !y && y -> !x
                    adj[x].push_back(neg(y));
                    adj[y].push_back(neg(x));
                    break;
                }
            }
        }

        fin.close();
    }

    vector<vector<int>> tarjan_scc() {
        // Initialize results
        parent = vector<int>(2 * n + 1, -1);
        found = vector<int>(2 * n + 1, -1);
        low_link = vector<int>(2 * n + 1, -1);
        in_stack = vector<bool>(2 * n + 1, false);

        // Visit all nodes
        vector<vector<int>> all_sccs;
        int timestamp = 0;  // global timestamp
        for (int node = 1; node <= 2 * n; ++node) {
            if (parent[node] == -1) {
                // The parent of the root is the root
                parent[node] = node;

                // Start a DFS traversal for this subtree
                dfs(node, timestamp, all_sccs);
            }
        }

        return all_sccs;
    }

    void dfs(int node, int& timestamp, vector<vector<int>>& all_sccs) {
        // If a new node is visited, increment the timestamp
        found[node] = ++timestamp;      // The timestamp when node was found
        low_link[node] = found[node];   // Node only knows its timestamp
        nodes_stack.push(node);         // Add node to the visiting stack
        in_stack[node] = true;

        // Visit each neighbour
        for (auto neigh : adj[node]) {
            // Check if the neighbour is already visited
            if (parent[neigh] != -1) {
                /* Update low_link[node] with information gained through neigh
                 * note: neigh is in the same SCC with node only if it's in the
                 * visiting stack;
                 * else, neigh is from other SCC, so it should be ignored */
                if (in_stack[neigh]) {
                    low_link[node] = min(low_link[node], found[neigh]);
                }
                continue;
            }

            // Save parent
            parent[neigh] = node;

            // Visit the child subree
            dfs(neigh, timestamp, all_sccs);

            // Update low_link[node] with information gained through neigh
            low_link[node] = min(low_link[node], low_link[neigh]);
        }

        /* Node is root in a SCC if low_link[node] == found[node]
         * (there is no edge from a descendant to an ancestor) */
        if (low_link[node] == found[node]) {
            /* pop all elements above node from stack
             * => extract the SCC where node is root */
            vector<int> scc;
            do {
                auto x = nodes_stack.top();
                nodes_stack.pop();
                in_stack[x] = false;

                scc.push_back(x);

            // Stop when node was popped from the stack
            } while (scc.back() != node);

            // Save SCC
            all_sccs.push_back(scc);
        }
    }

    int get_result() {
        // Find all strongly connected components (SCCs) using Tarjan
        vector<vector<int>> sccs = tarjan_scc();
        invited = vector<int>(2 * n + 1, -1);

        // Iterate through each SCC
        for (auto scc : sccs) {
            // Converting scc vector to a set for faster searching
            unordered_set<int> scc_set(scc.begin(), scc.end());

            // Iterate through each node in the SCC
            for (int node : scc) {
                // If both the node and its negation are in the same scc, return
                if (scc_set.find(neg(node)) != scc_set.end()) {
                    return 0;
                }

                // Else, 'node' can be invited to the party
                if (invited[node] == -1) {
                    invited[node] = 1;
                    invited[neg(node)] = 0;
                }
            }
        }

        // Count the number of people invited
        for (int i = 1; i <= n; i++) {
            if (invited[i] == 1) {
                cnt++;
            }
        }

        return cnt;
    }


    void print_output(const int d) {
        ofstream fout("scandal.out");
        fout << d << '\n';
        if (d) {
            for (int i = 1; i <= n; i++) {
                if (invited[i] == 1) {
                    fout << i << '\n';
                }
            }
        }
        fout.close();
    }
};

int main() {
    auto* task = new (nothrow) Task();
    if (!task) {
        cerr << "new failed: WTF are you doing? Throw your PC!\n";
        return -1;
    }
    task->solve();
    delete task;
    return 0;
}
