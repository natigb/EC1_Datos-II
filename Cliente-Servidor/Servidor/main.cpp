#include <QCoreApplication>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>

using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


/**
 * @brief The AdjListNode struct
 * Estructura que representa a un nodo un una lista de adyacencia
 */
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
/**
 * @brief The AdjList struct
 * Estructura de la lista de adyacencia, contiene un puntero señalando a la cabeza
 */
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
/**
 * @brief The Graph struct
 * Estructura para armar el grafo, contiene una lista de adyacencia y un atributo con el numero de vértices
 */
struct Graph
{
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
/**
 * @brief newAdjListNode
 * Funcion para crear un nuevo nodo en la lista de adyacencia
 * @param dest nodo que se agrega
 * @param weight peso
 * @return nodo creado
 *
 */
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
/**
 * @brief createGraph
 * Estructura para crear un grafo con V vertices
 * @param V tamaño del grafo
 * @return grafo creado
 */
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
/**
 * @brief addEdge
 * Funcion para agregar una arista nueva
 * @param graph  grafo al que se le quiere agregar
 * @param src source
 * @param dest destination
 * @param weight peso
 */
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
/**
 * @brief The MinHeapNode struct
 * Estructura para representar el min heap node
 */
struct MinHeapNode
{
    int  v;
    int dist;
};

// Structure to represent a min heap
/**
 * @brief The MinHeap struct
 * Estructura para representar el min heap
 */
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
/**
 * @brief newMinHeapNode
 * Estructura para crear un min heap node nuevo
 * @param v tamaño
 * @param dist distancia
 * @return
 */
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
/**
 * @brief createMinHeap
 * Estructura para crear un min heap
 * @param capacity
 * @return el minHeap creado
 */
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
/**
 * @brief swapMinHeapNode
 * Hace swap de 2 nodos
 * @param a Primer nodo
 * @param b Segundo nodo
 */
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
/**
 * @brief minHeapify
 * Actualiza la posicion de los nodos del heap
 * @param minHeap el minHeap a cambiar
 * @param idx
 */
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is ampty or not
/**
 * @brief isEmpty funcion para saber si el minHeap está vacío
 * @param minHeap
 * @return
 */
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
/**
 * @brief extractMin extrae el nodo minimo del heap
 * @param minHeap
 * @return
 */
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
/**
 * @brief decreaseKeydisminuye el valor de un vertice
 * @param minHeap
 * @param v
 * @param dist
 */
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
/**
 * @brief isInMinHeap buscar un vertice en el min heap
 * @param minHeap
 * @param v
 * @return
 */
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

// A utility function used to print the solution
/**
 * @brief printArr retorna un string de la solucion
 * @param dist
 * @param src
 * @param dest
 * @return
 */
string printArr(int dist[], int src, int dest)
{
    string answer = "La distancia mas corta entre " + to_string(src)+ " y " + to_string(dest) + " es "+ to_string(dist[dest]);

    cout << answer;
    return answer;
    //printf("%d \t\t\t %d\n", src, dist[dest]);


    //printf("Vertex   Distance from Source\n");
    //for (int i = 0; i < n; ++i)
      //  printf("%d \t\t %d\n", i, dist[i]);
}

// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
/**
 * @brief dijkstra calcula la distancia más corta de un nodo a otro
 * @param graph
 * @param src
 * @param dest
 * @return
 */
string dijkstra(struct Graph* graph, int src, int dest)
{
    int V = graph->V;// Get the number of vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                                          pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
    return printArr(dist, src, dest);
}

/**
 * @brief main se rea el grafo y los sockets para servidor
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    int V = 9;
    struct Graph* graph = createGraph(V);
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);

    // Crear el socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "No se pudo crear el socket";
        return -1;
    }

    // Enlazar el socket a una IP
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint))==-1)
    {
        cerr << "No se pudo enlazar a un IP/port";
        return -2;

    }

    // Maracr el socket para qe escuche
    if (listen(listening, SOMAXCONN)==-1)
    {
        cerr << "No se puede escuchar";
        return -3;

    }

    // Aceptar la llamada
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    if (clientSocket == -1)
    {
        cerr << "Problema con la conexión del cliente";
        return -4;
    }
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client),host, NI_MAXHOST, svc, NI_MAXSERV,0);
    if (result)
    {
        cout << host << "connected on" << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET,&client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on" << ntohs(client.sin_port) << endl;
    }


    // Cerrar el socket que escucha
    // Mientras recibe, lo muestra, echo
    char buf[4096];
    while(true)
    {
        // Limpiar buffer
        memset(buf, 0, 4096);
        // Esperar msj
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            cerr << "Hubo un problema de conexión" << endl;
            break;
        }

        if (bytesRecv==0)
        {
            cout << " CLiente se desconectó" << endl;
            break;

        }

        string nodes = string(buf, 0, bytesRecv);
        string a(1,nodes[0]);
        string b(1,nodes[1]);
        int src;
        istringstream isssrc(a);
        isssrc >> src;
        int dest;
        istringstream issdest(b);
        issdest >> dest;
        string answ = dijkstra(graph,src,dest);


        // Mostrar msj
        //cout << "Recibido: " << string(buf, 0, bytesRecv) << endl;

        cout << "Recibido: " << nodes[0] << endl;

        // Reenviar msj

        send(clientSocket, answ.c_str(), answ.size(), 0);



    }
    // Cerrar el socket
    close(clientSocket);


    QCoreApplication a(argc, argv);

    return a.exec();
}
