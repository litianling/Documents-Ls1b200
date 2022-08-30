// 一个C程序演示邻接表（图的表示法） 
#include <stdio.h>
#include <stdlib.h>
 
// 表示邻接表节点的结构体 
struct AdjListNode {
    int dest;
    struct AdjListNode* next;	// 指向节点的指针 
};
 
// 表示邻接表的结构体
struct AdjList {
    struct AdjListNode* head;	// 也是指向节点的指针 
};
 
// 表示图的结构体. 图是邻接表的数组
// 数组的大小为V(图中的顶点数)  
struct Graph {
    int V;
    struct AdjList* array;
};
 
// 创建一个新的邻接表节点的函数  
struct AdjListNode* newAdjListNode(int dest)	// 返回值类型是指向 邻接表节点结构体 的指针 
{
    struct AdjListNode* newNode; 				// 先声明newNode是指向邻接表节点结构体的指针 
    newNode= (struct AdjListNode*)malloc(sizeof(struct AdjListNode)); // 其大小是一个(struct AdjListNode) 
    newNode->dest = dest;						// 当前节点是dest
    newNode->next = NULL;						// 下一个节点为空 
    return newNode;
}
 
// 创建V顶点图的效用函数  
struct Graph* createGraph(int V)		// 返回值类型是指向 图结构体 的指针 
{
    // struct Graph* graph= (struct Graph*)malloc(sizeof(struct Graph));
    struct Graph* graph; 									// 先声明graph是指向图结构体的指针 
    graph= (struct Graph*)malloc(sizeof(struct Graph));		// 再对该指针指向地址分配空间， 大小是(struct Graph)的一个单元，并强制类型转换 
	graph->V = V;											// 图结构体下有五个节点 
 
    // 创建邻接表数组。（数组大小为V） 						// array是指向 邻接表结构体 的指针，在图结构体中已经声明过 
    graph->array = (struct AdjList*)malloc(					// 再对该指针指向地址分配空间， 大小是(struct AdjList)的V个单元，并强制类型转换
        V * sizeof(struct AdjList));
 
    // 通过将head设置为NULL，将每个邻接表初始化为空  
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// 向无向图添加一条边 
void addEdge(struct Graph* graph, int src, int dest)
{
    // 添加一条从src到dest的路劲  
	// 向节点src的邻接表中添加新的节点  
	// 节点是在开头添加的 
    struct AdjListNode* check = NULL;					// check是邻接表节点结构体，用于检查邻接表末尾 
    struct AdjListNode* newNode = newAdjListNode(dest); // 将目标节点创建为新节点结构体 
 
    if (graph->array[src].head == NULL)	{ 				// 如果图中的src邻接表表头为空（原来没节点=>需要放的是第一个节点） 
        newNode->next = graph->array[src].head;			// 那么目标节点是放在邻接表的第一个节点，即目标节点之后的节点为空 
        graph->array[src].head = newNode;				// 邻接表里面有了一个节点，该节点是newNode
    }
    else{ 												// 如果图中的src邻接表表头不为空（邻接表已经有节点了） 
        check = graph->array[src].head;					// 从邻接表中第一个节点开始检查 
        while (check->next != NULL) 					// 一直检测直到邻接表末尾 
		{
            check = check->next;
        }
        check->next = newNode;							// 将新的待添加节点放在邻接表末尾 
    }
 
    // 由于图形是无向的，添加一条边从dest到src也  
    newNode = newAdjListNode(src);						// 将原来的源节点构建为待添加的新节点 
    if (graph->array[dest].head == NULL) {				// 将原来的目标节点作为新的源节点进行相同操作 
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
    else {
        check = graph->array[dest].head;
        while (check->next != NULL) {
            check = check->next;
        }
        check->next = newNode;
    }
}
 
// 打印图的邻接表表示的实用函数  
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v) {	// 遍历源节点 
        struct AdjListNode* pCrawl;		// 声明 pCrawl是指向目标节点的指针 
		pCrawl = graph->array[v].head;	// 这个目标节点刚开始的时候是源节点邻接表中的第一个节点（头节点） 
        printf("\n Adjacency list of vertex %d\n head ", v);	// 先输出源节点 
        while (pCrawl) {				// 如果目标节点存在就输出目标节点 
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;		// 目标节点指向下一个 
        }
        printf("\n");					// 当前源节点对应的所有目标节点输出完毕就换行 
    }
}
 
 
/* 拓展->深度优先搜索 */
void DFS(struct Graph *graph,int node,int* Visited)	
{
    printf("\n DFS: %d ",node);		// 输出开始的节点 
    Visited[node] = 1;							// 这个节点已经看过了 
    struct AdjListNode *check_node = graph->array[node].head;	// 创建一个节点用于遍历（起始节点第一个节点） 
    while(check_node)							// 这个节点非空就遍历 
    {
        if(!Visited[check_node->dest])			// 如果这个新的节点没被哈看过，就搜索新节点 
            DFS(graph,check_node->dest,Visited);
        check_node = check_node->next;			// 跳入下一个节点进行搜索 
    }
	printf("\n");
}
 
 
/* 拓展->总结上述递归似是而非，改为非递归 */
void DFS_Pro(struct Graph *graph,int node,int* Visited)	
{
    printf("\n DFS: %d",node);					// 输出开始的节点 
    Visited[node] = 1;							// 这个节点已经看过了 
    struct AdjListNode *check_node = graph->array[node].head;	// 创建一个节点用于遍历（起始节点第一个节点） 
    while(check_node)							// 这个节点非空就遍历 
    {
        if(!Visited[check_node->dest])			// 如果这个新的节点没被看过 
        {
        	printf(" -> %d",check_node->dest);
        	Visited[check_node->dest] = 1;
        	check_node = graph->array[check_node->dest].head;
        	continue;
		}    
        check_node = check_node->next;			// 跳入下一个节点进行搜索 
    }
    printf("\n"); 
}

/* 拓展->广度优先搜索 */
void BFS(struct Graph *graph,int node,int* Visited,int v)	
{
	int node_buffer[v] = {0};
	int buffer_flag = 0;
    printf("\n BFS: %d",node);					// 输出开始的节点 
    Visited[node] = 1;					// 开始节点已经看过了 
    node_buffer[buffer_flag++] = node;	// 开始节点入栈 
    
	struct AdjListNode *check_node ;	// 创建一个检验节点用于遍历
	while(buffer_flag)					// 这个节点缓存栈非空 
    {
    	check_node = graph->array[node_buffer[--buffer_flag]].head;
    	while(check_node)
		{
			if(!Visited[check_node->dest])			// 如果这个新的节点没被看过 
        	{
        		printf(" -> %d",check_node->dest);	// 输出 
        		Visited[check_node->dest] = 1;		// 看过
				node_buffer[buffer_flag++] = check_node->dest; // 入栈	 
			}
			check_node = check_node->next;			// 链接的下一个节点 
		} 
	}
    printf("\n");
} 
 
// 驱动程序来测试以上功能
int main()
{
    /* 创建上面给出的图表 */
    int V = 5;
    struct Graph* graph = createGraph(V);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    /* 打印上面图的邻接表表示 */
    printGraph(graph);
 
	/* 图的深度优先搜索DFS */
	int begin_node;
	int Visited[V]={0};	// 已经看过的节点，初始值都是0代表都没看过 
    printf("\nPlease input the begin node：");
    scanf("%d",&begin_node);
    //DFS(graph,begin_node,Visited);	// 开始遍历——DFS递归实现 
	DFS_Pro(graph,begin_node,Visited);	// 开始遍历——DFS非递归实现
	for(int i=0;i<V;i++)				// 清空观测矩阵 
		Visited[i]=0;
	BFS(graph,begin_node,Visited,V);	// 开始遍历——BFS非递归实现 
	
    return 0;
}
