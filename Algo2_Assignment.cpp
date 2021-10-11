#include<iostream>
#include<ostream>
#include<iterator>
#include<vector>
#include<list>
#include<ctime>
#include<fstream>
#include<algorithm>

using namespace std;

vector<vector<int>> build_random_graph(int,double);
vector<int> bfs(vector<vector<int>>&,int);
void add_edge(vector<vector<int>>&,int,int);
int is_Isolated(vector<vector<int>>);
int connectivity(vector<vector<int>>);
int diameter(vector<vector<int>>);
void attributes_Simulation();


vector<vector<int>> build_random_graph(int v, double p)
{
    vector<vector<int>> adj_list(v);//Vector of vectors - the outer vector represents the vertices , the inner vector contains the neighbors of each vector
    for (int i = 0; i < v; i++)
    {
        for (int j = i + 1; j < v; j++)
        {
            /*Randomize a number betwenn 0-1 and check if its equal/smaller the our p , if it is we add an edge*/
            if ((double)rand() / RAND_MAX <= p)
                add_edge(adj_list, i, j);
        }
    }
    return adj_list;
}

vector<int> bfs(vector<vector<int>>& adj_list, int start_vertex)
{
    vector<int> distances(adj_list.size(), -1);//set the distances to -1 (infnite)
    vector<bool> visited(adj_list.size(), false);//indicate if a vertex is visited or not 
    list<int> queue;

    visited[start_vertex] = true;
    distances[start_vertex] = 0;

    queue.push_back(start_vertex);
    vector<vector<int>>::iterator i;

    while (!queue.empty())
    {
        start_vertex = queue.front();
        queue.pop_front();
        /*Run through all the neighbors of the current vertex*/
        for (auto i = adj_list[start_vertex].begin(); i != adj_list[start_vertex].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                distances[*i] = distances[start_vertex] + 1;// set the current distance to the current vertex
                queue.push_back(*i);
            }
        }
    }
    return (distances);// return an vector containing the distances from the startVertex to the other vertcies
}


void add_edge(vector<vector<int>>& adj_list, int vertex_a, int vertex_b)
{
    adj_list[vertex_a].push_back(vertex_b);//push vertex b to the vector containing vertex a neighbors
    adj_list[vertex_b].push_back(vertex_a);//push vertex a to the neighbors vector of vertex b aswell - because the graph is undirected
}


int is_Isolated(vector<vector<int>> adj_list)
{
    /* Run through all vertices to check if there's a vertex with an empty neighbors vector,*/
    for (int i = 0; i < adj_list.size(); i++)
    {
        if (adj_list[i].empty())
            return 1;//Found a isolated vertex
    }
    return 0;//No isolated vertex
}



int connectivity(vector<vector<int>> adj_list)
{
    /*Run bfs once which returns a distance array , if there's distance -1 it mean the graph is not connected*/
    vector<int> distanceArray = bfs(adj_list, 0);
    for (int i = 0; i < distanceArray.size(); i++)
    {
        if (distanceArray[i] == -1)
            return 0;//Graph not connected
    }
    return 1;//Graph connected
}


int diameter(vector<vector<int>> adj_list)
{
    vector<int> maxDistances(adj_list.size()); // a vector containing the biggest distance for each bfs run
    vector<int> tempDistances(adj_list.size());// a vector containing the distances for the current bfs run
    tempDistances = bfs(adj_list, 0);
    maxDistances[0] = *max_element(tempDistances.begin(), tempDistances.end());
    if (maxDistances[0] == -1)//The graph is not connected so the diameter is -1 (infinite) and there's no need to search further
    	return -1;
    for (int i = 1; i < adj_list.size(); i++)
    {
    	tempDistances = bfs(adj_list,i);
    	maxDistances[i] = *max_element(tempDistances.begin(), tempDistances.end());
    }
    int maxValue = *max_element(maxDistances.begin(),maxDistances.end());
    return maxValue;//MaxValue==The diameter of the graph
}

void attributes_Simulation()
{
    int count_Succes;

    double probabilities_Input1[] = {0.0011,0.0023,0.0042,0.0052,0.0061,0.0073,0.0092,0.0121,0.0249,0.0569};//0.0069077552
    double probabilities_Attribute1[10] = { 0 };
    for (int i = 0; i < 10; i++)
    {
        count_Succes = 0;
        for (int j = 0; j < 500; j++)
        {
            vector<vector<int>> g = build_random_graph(1000, probabilities_Input1[i]);
            if (connectivity(g) == 1)
                count_Succes++;
        }
        probabilities_Attribute1[i] = count_Succes / 500.00000;
    }

   double probabilities_Input2[] = { 0.065,0.075,0.085,0.095,0.10,0.122,0.1245,0.158,0.181,0.224 };
   double probabilities_Attribute2[10] = { 0 };
   for (int i = 0; i < 10; i++)
   {
       count_Succes = 0;
       for (int j = 0; j < 500; j++)
       {
           vector<vector<int>> g = build_random_graph(1000, probabilities_Input2[i]);
           int diam = diameter(g);
           if (diam <= 2 && diam!=-1)
               count_Succes++;
       }
       probabilities_Attribute2[i] = count_Succes / 500.00000;
   }


    double probabilities_Input3[] = { 0.0011,0.0023,0.0042,0.0052,0.0061,0.0073,0.0092,0.0121,0.0249,0.0569 };//0.0069077552
    double probabilities_Attribute3[10] = { 0 };
    for (int i = 0; i < 10; i++)
    {
        count_Succes = 0;
        for (int j = 0; j < 500; j++)
        {
            vector<vector<int>> g = build_random_graph(1000, probabilities_Input3[i]);
            if (is_Isolated(g) == 1)
                count_Succes++;
        }
        probabilities_Attribute3[i] = count_Succes / 500.00000;
    }


    ofstream myFile1, myFile2, myFile3;
    myFile1.open("Attribute1.csv");
    myFile2.open("Attribute2.csv");
    myFile3.open("Attribute3.csv");
    for (int i = 0; i < 10; i++)
    {
        myFile1 << "P=[" << probabilities_Input1[i] << "]" << ", " << probabilities_Attribute1[i] << endl;
        myFile2 << "P=[" << probabilities_Input2[i] << "]" << ", " << probabilities_Attribute2[i] << endl;
        myFile3 << "P=[" << probabilities_Input3[i] << "]" << ", " << probabilities_Attribute3[i] << endl;
    }
    myFile1.close();
    myFile2.close();
    myFile3.close();
}

int main()
{
    srand(time(NULL));
    attributes_Simulation();
}