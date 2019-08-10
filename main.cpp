//
//  create by untea 01.06.2019
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

class Graph
{
private:

    vector<vector<int>> adjacency_list;

    vector<vector<int>> bfs(int start_vertex_number)
    {
        // Returns a vector representing pathes to all other vertexes
        vector<bool> visited(vertex_number, false);
        visited[start_vertex_number] = true;

        queue<int> q;
        q.push(start_vertex_number);

        vector<vector<int>> pathes(vertex_number);
        pathes[start_vertex_number].push_back(start_vertex_number + 1);

        while(!q.empty())
        {
            int from = q.front();
            q.pop();

            for (auto to : adjacency_list[from])
                if (!visited[to])
                {
                    q.push(to);
                    visited[to] = true;
                    // Path to current vertex is path to previous vertex plus this vertex
                    pathes[to] = pathes[from];
                    pathes[to].push_back(to + 1);
                }
        }
        return pathes;
    }

    void delete_connection(int from, int to)
    {
        for (int i = 0; i < adjacency_list[from].size(); i++)
            if (adjacency_list[from][i] == to)
            {
                adjacency_list[from].erase(adjacency_list[from].begin() + i);
                break;
            }
    }

public:
    int vertex_number;
    int *GraphGrid = new int;

    Graph(int vertex_number, vector<pair<int, int>> &edges)
    {
        this->vertex_number = vertex_number;
        adjacency_list.resize(vertex_number);

        for (auto &edge : edges)
        {
            bool present = false;
            for (int i = 0; i < adjacency_list[edge.first].size(); i++)
                if (adjacency_list[edge.first][i] == edge.second)
                {
                    present = true;
                    *GraphGrid = adjacency_list[edge.first][i];
                    break;
                }
            if (!present)
                adjacency_list[edge.first].push_back(edge.second);

            present = false;
            for (int i = 0; i < adjacency_list[edge.second].size(); i++)
                if (adjacency_list[edge.second][i] == edge.first)
                {
                    present = true;
                    break;
                }
            if (!present)
                adjacency_list[edge.second].push_back(edge.first);
        }

        // I will sort the adjacencies so the graph does not depend on input order
        for (int i = 0; i < vertex_number; i++)
            sort(adjacency_list[i].begin(), adjacency_list[i].end());

    }

    Graph()
    {
        vertex_number = 0;
    }

    void print_graph()
    {
        cout << "Adjacency list:" << endl;
        for (int i = 0; i < adjacency_list.size(); i++)
        {
            cout << i + 1 << ": ";
            for(int j = 0; j < adjacency_list[i].size(); j++)
                cout << adjacency_list[i][j] + 1 << ' ';
            cout << endl;
        }
    }

    void print_first_graph_element()
    {
        cout << "First(v): ";
        for (int i = 0; i < adjacency_list.size(); i++)
        {
            for(int j = 0; j < adjacency_list[i].size(); j++)
            {
                cout << i << " " << j << endl;
                break;
            }
            break;
        }
        auto it = adjacency_list.begin();
        cout << "Address: " << &it << endl;
    }

    void VERTEX()
    {
        cout << "Which vertex you want to know the neighbors: ";
        int huj = 0;
        cin >> huj;
        cout << "Specify neighbor index: ";
        int hgf = 0;
        cin >> hgf;
        if (huj == 0 || hgf == 0)
        {
            cout << "wrong input";
            return;
        }
        for (int i = 0; i < adjacency_list.size(); i++)
        {
            i = huj - 1;
            cout << i + 1 << ": ";
            if (i > adjacency_list.size())
            {
                cout << "wrong input";
                break;
            }
            for(int j = 0; j < adjacency_list[i].size(); j++)
            {
                j = hgf - 1;
                if (j > adjacency_list[i].size())
                {
                    cout << "wrong input";
                    break;
                }
                cout << "Neighbors vertexes:";
                cout << adjacency_list[i][j] + 1 << ' ' << endl;
                break;
            }
            break;
        }
    }

    void next_vertex_in_adj()
    {
            cout << "Which vertex you want to know the neighbors: ";
            int huj = 0;
            cin >> huj;
            if (huj == 0) {
                cout << "wrong input";
                return;
            }
            if (huj > adjacency_list.size()) {
                cout << "wrong input";
                return;
            }
            for (int i = 0; i < adjacency_list.size(); i++) {
                i = huj - 1;
                cout << "Neighbors vertexes: " << endl;
                cout << i + 1 << ": ";
                for (int j = 0; j < adjacency_list[i].size(); j++) {
                    cout << adjacency_list[i][j] + 1 << ' ';
                }
                break;
            }
            cout << endl;
    }

    friend std::ostream& operator<< (std::ostream &out, const Graph &point);

    void print_diameters()
    {
        // First I need to find all pathes from a random vertex e.g. from the first one
        auto pathes = bfs(0);
        // Then I am going to find the longest pathes (there can be multiple diameters)
        int max_dist = 0;
        set<int> diameter_vertexes;

        for (auto &path : pathes)
        {
            if (path.size() > max_dist)
            {
                diameter_vertexes.clear();
                diameter_vertexes.insert(path[0] - 1);
                // If the first vertex is a diameter vertex, then
                // I need to append the last vertex as well
                diameter_vertexes.insert(path.back() - 1);
                max_dist = path.size();
            }
            else if (path.size() == max_dist) {
                diameter_vertexes.insert(path[0] - 1);
                diameter_vertexes.insert(path.back() - 1);
            }
        }
        // Now I will run BFS for the second time to find diameter paths
        vector<vector<int>> results;
        int diameter_length = 0;
        for (auto diameter_vertex : diameter_vertexes)
        {
            pathes = bfs(diameter_vertex);
            // Finally extracting the diameter
            // Actually there can be multiple diameters outcoming from one vertex
            vector<vector<int>> diameter_pathes;

            for (auto &path : pathes)
            {
                if (path.size() > diameter_length)
                {
                    results.clear();
                    diameter_pathes.clear();
                    diameter_pathes.push_back(path);
                    diameter_length = path.size();
                }
                else if (path.size() == diameter_length)
                    diameter_pathes.push_back(path);
            }

            // Collecting diameters
            for (auto &diameter_path : diameter_pathes)
                results.push_back(diameter_path);
        }
        // The last thing I need to do is to cut repeating diameters
        // e.g "1 -> 2 -> 3" diameter is the same as "3 -> 2 -> 1"
        for (int i = 0; i < results.size(); i++)
        {
            for (int j = i + 1; j < results.size(); j++)
            {
                auto reversed = results[j];
                reverse(reversed.begin(), reversed.end());
                if (results[i] == reversed || results[i] == results[j])
                {
                    results.erase(results.begin() + j);
                    j--;
                }
            }
        }
        // Printing
        cout << "Graph diameter is " << diameter_length << endl;
        cout << "Vertex pairs that make a diameter:" << endl;
        for (auto &result : results)
            cout << result[0] << ' ' << result.back() << endl;

        cout << "Pathes that make such diameters are: " << endl;

        for (auto &result : results)
        {
            int i;
            for (i = 0; i < result.size() - 1; i++)
                cout << result[i] << " => ";
            cout << result[i] << endl;
        }
    }

    void add_vertex()
    {
        adjacency_list.push_back(vector<int>());
        vertex_number++;
        cout << "Vertex added" << endl;
    }

    void delete_vertex(int vertex_number)
    {
        adjacency_list.erase(adjacency_list.begin() + vertex_number - 1);
        vertex_number--;
        cout << "Vertex deleted" << endl;
    }

    void add_edge(int from, int to)
    {
        from--, to--;
        adjacency_list[from].push_back(to);
        adjacency_list[to].push_back(from);
        cout << "Edge added" << endl;
    }

    void delete_edge(int from, int to)
    {
        // There can be multiple edges connecting two vertexes
        // So by deleting an edge there still might exist a connection
        // between vertexes
        from--, to--;
        delete_connection(from, to);
        delete_connection(to, from);
        cout << "Edge deleted" << endl;
    }

    ~Graph()
    {
        delete GraphGrid;
    }
};

std::ostream& operator<< (std::ostream &out, const Graph &point)
{
    out << Graph();
    return out;
}

Graph load_graph()
{
    int vertex_number = 0;
    string filename;

    cout << "Enter file name: ";
    cin >> filename;

    ifstream input(filename);

    string next = "Dummy";

    while (input >> next)
    {
        if (next == "#")
            break;
        vertex_number++;
    }
    vector<pair<int, int>> edges;

    int from, to;
    while (input >> from >> to)
    {
        from--, to--;
        edges.push_back(make_pair(from, to));
    }

    return Graph(vertex_number, edges);
}

int main()
{
    string command = "None";
    Graph graph = Graph();
    cout << "Type \"help\" to see list of available commands" << endl;
    while (command != "exit") {
        cout << "$ ";
        cin >> command;

        if (command == "load")
            graph = load_graph();

        else if (command == "diameter")
            if (graph.vertex_number != 0) {
                graph.print_diameters();
            } else {
                cout << "No graph is loaded";
            }

        else if (command == "add") {
            string to_add;
            cin >> to_add;
            if (to_add == "vertex")
                graph.add_vertex();
            else if (to_add == "edge") {
                cout << "From: ";
                int from, to;
                cin >> from;
                cout << "To: ";
                cin >> to;
                graph.add_edge(from, to);
            } else
                cout << "Can't add " << to_add << " to graph" << endl;

        }
        else if (command == "delete")
        {
            if (graph.vertex_number < 1)
                cout << "Graph not loaded" << endl;
            else {
                string to_delete;
                cin >> to_delete;
                if (to_delete == "vertex")
                {
                    cout << "Vertex number: ";
                    int vertex_number;
                    cin >> vertex_number;
                    if (vertex_number > graph.vertex_number)
                        cout << "There are too little vertexes";
                    else
                        graph.delete_vertex(vertex_number);
                }
                else if (to_delete == "edge")
                {
                    cout << "From: ";
                    int from, to;
                    cin >> from;
                    cout << "To: ";
                    cin >> to;
                    graph.delete_edge(from, to);
                }
            }
        }
        else if (command == "graph")
            graph.print_graph();
        else if (command == "first")
            graph.print_first_graph_element();
        else if (command == "next")
            graph.next_vertex_in_adj();
        else if (command == "VERTEX")
            graph.VERTEX();
        else if (command == "help")
        {
            cout << "load          - loads a graph from file"                                             << endl;
            cout << "diameter      - displays graph's diameters"                                          << endl;
            cout << "graph         - print graph's adjacency list"                                        << endl;
            cout << "add vertex    - adds a vertex to graph"                                              << endl;
            cout << "add edge      - adds an edge to graph"                                               << endl;
            cout << "delete vertex - deletes a vertex from graph"                                         << endl;
            cout << "delete edge   - deletes an edge from graph"                                          << endl;
            cout << "exit          - exits the program"                                                   << endl;
            cout << "first         - output index of the first vertex of the graph"                       << endl;
            cout << "next          - output neighbors at a given vertex "                                 << endl;
            cout << "VERTEX        - output vertex with index i from the set of vertices adjacent to v. " << endl;
            cout << endl;
        }
        else if (command != "exit")
            cout << "Unknown command: " << command << endl;
    }
    return 0;
}