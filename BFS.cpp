#pragma warning(disable:4996)

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdlib.h>
#include <string.h>
#include <queue>
#include <iostream>
#include "bfs.h"

using namespace std;

nodeM* noduri[maxim];


int get_neighbors(const Grid *grid, Point p, Point neighb[])
{

    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    int i = p.row;
    int j = p.col;
    int noOfNeigh = 0;

    if (grid->mat[i + 1][j] == 0)
    {
        neighb[noOfNeigh].col = j;
        neighb[noOfNeigh].row = i + 1;
        noOfNeigh++;
    }
    if (grid->mat[i - 1][j] == 0)
    {
        neighb[noOfNeigh].col = j;
        neighb[noOfNeigh].row = i - 1;
        noOfNeigh++;
    }
    if (grid->mat[i][j + 1] == 0)
    {
        neighb[noOfNeigh].col = j + 1;
        neighb[noOfNeigh].row = i;
        noOfNeigh++;
    }
    if (grid->mat[i][j - 1] == 0)
    {
        neighb[noOfNeigh].col = j - 1;
        neighb[noOfNeigh].row = i;
        noOfNeigh++;
    }
   
    return noOfNeigh;

}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
   
    int n = graph->nrNodes;
    for (int i = 0; i < n; i++)
    {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = maxim;
        graph->v[i]->parent = NULL;
        if (op != NULL) op->count(3);

    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
  
    std::queue <Node*> queue;
    queue.push(s);

    if (op != NULL) op->count(4);

    while ( !queue.empty() )
    {
        Node* u = queue.front();
        queue.pop();
        if (op != NULL) op->count(2);
        for (int i = 0; i < u->adjSize; i++)
        {
            if (u->adj[i]->color == COLOR_WHITE)
            {
                u->adj[i]->color = COLOR_GRAY;
                u->adj[i]->dist++;
                u->adj[i]->parent = u;
                queue.push(u->adj[i]);
                if (op != NULL) op->count(4);
            }
            if (op != NULL) op->count();
        }
        u->color = COLOR_BLACK;
        if (op != NULL) op->count();
    }
    
}

//TREE PRINTING FUNCTIONS

nodeM* createNode(int key)
{
    nodeM* newNode = (nodeM*)malloc(sizeof(nodeM));
    newNode->key = key;
    newNode->nbChld = 0;
    for (int i = 0; i < nrChildren; i++)
        newNode->children[i] = NULL;
    return newNode;
}

nodeB* createNodeB(int key)
{
    nodeB* newNode = (nodeB*)malloc(sizeof(nodeB));
    newNode->key = key;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

int transformT1(int* parent, int size)
{
    int root = -1;
    for (int i = 0; i < size; i++)
    {
        noduri[i] = createNode(i);
    }
    for (int i = 0; i < size; i++)
    {
        if (parent[i] == -1) //found root
        {
            root = i;
        }
        else
        {
            int nodeParent = parent[i];
            int nbChildren = noduri[nodeParent]->nbChld;
            noduri[nodeParent]->children[nbChildren] = noduri[i];

            (noduri[nodeParent]->nbChld)++;
        }
    }
    return root;
}

nodeB* transformT2(nodeM* rootM, nodeM* parent, int childPos)
{
    if (rootM == NULL)
        return NULL;
    nodeB* rootB = createNodeB(rootM->key);
    if (rootM->nbChld != 0 && rootM->children[0] != NULL)
    {
        rootB->child = transformT2(rootM->children[0], rootM, 0);
    }
    if (parent != NULL && childPos + 1 < parent->nbChld && parent->children[childPos] == rootM)
    {
        rootB->sibling = transformT2(parent->children[childPos + 1], parent, childPos + 1);
    }
    return rootB;
}

void prettyPrint(nodeB* root, int nbSpaces, Point repr[])
{
    if (root == NULL)
        return;
    for (int i = 0; i < nbSpaces; i++)
        cout << " ";
    int k = root->key;
    cout << "( " << repr[k].row << " , " << repr[k].col << " )" << endl;
    prettyPrint(root->child, nbSpaces + 5, repr);
    prettyPrint(root->sibling, nbSpaces, repr);

}


void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
       
        nodeM* rootM = noduri[transformT1(p, n)];
        nodeB* rootB = transformT2(rootM, NULL, 0);
        prettyPrint(rootB, 0, repr);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
   
    int shortPath = 0;
    bfs(graph, start, NULL);
    if(end->dist == maxim)
        return -1;

    shortPath = end->dist;

    for (int i = shortPath; i >= 0 && end != NULL; i--)
    {
        path[i] = end;
        if (end->parent != NULL)
            end = end->parent;
        else
            break;
    }
    return shortPath;

}

//GENERATING RANDOM GRAPH + PERFORMANCE

void generateRandom(Graph* graph, int V, int E)
{
    int u, w, found, i, sz;

    for (i = 0; i < V; i++)
    {
        graph->v[i]->adj = (Node**)malloc(graph->nrNodes * sizeof(Node*));
        graph->v[i]->adjSize = 0;
    }

    for ( i = 0; i < V - 1; i++)
    {
        //link all consecutive nodes

        sz = graph->v[i]->adjSize;
        graph->v[i]->adj[sz] = graph->v[i+1];
        graph->v[i]->adjSize++;

        sz = graph->v[i+1]->adjSize;
        graph->v[i+1]->adj[sz] = graph->v[i];
        graph->v[i+1]->adjSize++;
    }
    //link node V-1 with node 0
    sz = graph->v[0]->adjSize;
    graph->v[0]->adj[sz] = graph->v[V-1];
    graph->v[0]->adjSize++;
    sz = graph->v[V-1]->adjSize;
    graph->v[V-1]->adj[sz] = graph->v[0];
    graph->v[V-1]->adjSize++;


    i = 0;
    while (i < E - V)
    {
        //generate E-V random edges - if the edge already exist, get 2 random vertexes again
        u = rand() % V;
        w = rand() % V;
        found = 0;
        Node* ptr = graph->v[u];
        for (int j = 0; j <= ptr->adjSize; j++)
        {
            if (ptr->adj[j] == graph->v[w])
            {
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            sz = graph->v[u]->adjSize;
            graph->v[u]->adj[sz] = graph->v[w];
            graph->v[u]->adjSize++;

            sz = graph->v[w]->adjSize;
            graph->v[w]->adj[sz] = graph->v[u];
            graph->v[w]->adjSize++;

            i++;
        }
    }
}


void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        generateRandom(&graph, graph.nrNodes, n);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        generateRandom(&graph, graph.nrNodes, 4500);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
