// һ��C������ʾ�ڽӱ�ͼ�ı�ʾ���� 
#include <stdio.h>
#include <stdlib.h>
 
// ��ʾ�ڽӱ�ڵ�Ľṹ�� 
struct AdjListNode {
    int dest;
    struct AdjListNode* next;	// ָ��ڵ��ָ�� 
};
 
// ��ʾ�ڽӱ�Ľṹ��
struct AdjList {
    struct AdjListNode* head;	// Ҳ��ָ��ڵ��ָ�� 
};
 
// ��ʾͼ�Ľṹ��. ͼ���ڽӱ������
// ����Ĵ�СΪV(ͼ�еĶ�����)  
struct Graph {
    int V;
    struct AdjList* array;
};
 
// ����һ���µ��ڽӱ�ڵ�ĺ���  
struct AdjListNode* newAdjListNode(int dest)	// ����ֵ������ָ�� �ڽӱ�ڵ�ṹ�� ��ָ�� 
{
    struct AdjListNode* newNode; 				// ������newNode��ָ���ڽӱ�ڵ�ṹ���ָ�� 
    newNode= (struct AdjListNode*)malloc(sizeof(struct AdjListNode)); // ���С��һ��(struct AdjListNode) 
    newNode->dest = dest;						// ��ǰ�ڵ���dest
    newNode->next = NULL;						// ��һ���ڵ�Ϊ�� 
    return newNode;
}
 
// ����V����ͼ��Ч�ú���  
struct Graph* createGraph(int V)		// ����ֵ������ָ�� ͼ�ṹ�� ��ָ�� 
{
    // struct Graph* graph= (struct Graph*)malloc(sizeof(struct Graph));
    struct Graph* graph; 									// ������graph��ָ��ͼ�ṹ���ָ�� 
    graph= (struct Graph*)malloc(sizeof(struct Graph));		// �ٶԸ�ָ��ָ���ַ����ռ䣬 ��С��(struct Graph)��һ����Ԫ����ǿ������ת�� 
	graph->V = V;											// ͼ�ṹ����������ڵ� 
 
    // �����ڽӱ����顣�������СΪV�� 						// array��ָ�� �ڽӱ�ṹ�� ��ָ�룬��ͼ�ṹ�����Ѿ������� 
    graph->array = (struct AdjList*)malloc(					// �ٶԸ�ָ��ָ���ַ����ռ䣬 ��С��(struct AdjList)��V����Ԫ����ǿ������ת��
        V * sizeof(struct AdjList));
 
    // ͨ����head����ΪNULL����ÿ���ڽӱ��ʼ��Ϊ��  
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// ������ͼ���һ���� 
void addEdge(struct Graph* graph, int src, int dest)
{
    // ���һ����src��dest��·��  
	// ��ڵ�src���ڽӱ�������µĽڵ�  
	// �ڵ����ڿ�ͷ��ӵ� 
    struct AdjListNode* check = NULL;					// check���ڽӱ�ڵ�ṹ�壬���ڼ���ڽӱ�ĩβ 
    struct AdjListNode* newNode = newAdjListNode(dest); // ��Ŀ��ڵ㴴��Ϊ�½ڵ�ṹ�� 
 
    if (graph->array[src].head == NULL)	{ 				// ���ͼ�е�src�ڽӱ��ͷΪ�գ�ԭ��û�ڵ�=>��Ҫ�ŵ��ǵ�һ���ڵ㣩 
        newNode->next = graph->array[src].head;			// ��ôĿ��ڵ��Ƿ����ڽӱ�ĵ�һ���ڵ㣬��Ŀ��ڵ�֮��Ľڵ�Ϊ�� 
        graph->array[src].head = newNode;				// �ڽӱ���������һ���ڵ㣬�ýڵ���newNode
    }
    else{ 												// ���ͼ�е�src�ڽӱ��ͷ��Ϊ�գ��ڽӱ��Ѿ��нڵ��ˣ� 
        check = graph->array[src].head;					// ���ڽӱ��е�һ���ڵ㿪ʼ��� 
        while (check->next != NULL) 					// һֱ���ֱ���ڽӱ�ĩβ 
		{
            check = check->next;
        }
        check->next = newNode;							// ���µĴ���ӽڵ�����ڽӱ�ĩβ 
    }
 
    // ����ͼ��������ģ����һ���ߴ�dest��srcҲ  
    newNode = newAdjListNode(src);						// ��ԭ����Դ�ڵ㹹��Ϊ����ӵ��½ڵ� 
    if (graph->array[dest].head == NULL) {				// ��ԭ����Ŀ��ڵ���Ϊ�µ�Դ�ڵ������ͬ���� 
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
 
// ��ӡͼ���ڽӱ��ʾ��ʵ�ú���  
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v) {	// ����Դ�ڵ� 
        struct AdjListNode* pCrawl;		// ���� pCrawl��ָ��Ŀ��ڵ��ָ�� 
		pCrawl = graph->array[v].head;	// ���Ŀ��ڵ�տ�ʼ��ʱ����Դ�ڵ��ڽӱ��еĵ�һ���ڵ㣨ͷ�ڵ㣩 
        printf("\n Adjacency list of vertex %d\n head ", v);	// �����Դ�ڵ� 
        while (pCrawl) {				// ���Ŀ��ڵ���ھ����Ŀ��ڵ� 
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;		// Ŀ��ڵ�ָ����һ�� 
        }
        printf("\n");					// ��ǰԴ�ڵ��Ӧ������Ŀ��ڵ������Ͼͻ��� 
    }
}
 
 
/* ��չ->����������� */
void DFS(struct Graph *graph,int node,int* Visited)	
{
    printf("\n DFS: %d ",node);		// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ����ڵ��Ѿ������� 
    struct AdjListNode *check_node = graph->array[node].head;	// ����һ���ڵ����ڱ�������ʼ�ڵ��һ���ڵ㣩 
    while(check_node)							// ����ڵ�ǿվͱ��� 
    {
        if(!Visited[check_node->dest])			// �������µĽڵ�û�����������������½ڵ� 
            DFS(graph,check_node->dest,Visited);
        check_node = check_node->next;			// ������һ���ڵ�������� 
    }
	printf("\n");
}
 
 
/* ��չ->�ܽ������ݹ����Ƕ��ǣ���Ϊ�ǵݹ� */
void DFS_Pro(struct Graph *graph,int node,int* Visited)	
{
    printf("\n DFS: %d",node);					// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ����ڵ��Ѿ������� 
    struct AdjListNode *check_node = graph->array[node].head;	// ����һ���ڵ����ڱ�������ʼ�ڵ��һ���ڵ㣩 
    while(check_node)							// ����ڵ�ǿվͱ��� 
    {
        if(!Visited[check_node->dest])			// �������µĽڵ�û������ 
        {
        	printf(" -> %d",check_node->dest);
        	Visited[check_node->dest] = 1;
        	check_node = graph->array[check_node->dest].head;
        	continue;
		}    
        check_node = check_node->next;			// ������һ���ڵ�������� 
    }
    printf("\n"); 
}

/* ��չ->����������� */
void BFS(struct Graph *graph,int node,int* Visited,int v)	
{
	int node_buffer[v] = {0};
	int buffer_flag = 0;
    printf("\n BFS: %d",node);					// �����ʼ�Ľڵ� 
    Visited[node] = 1;					// ��ʼ�ڵ��Ѿ������� 
    node_buffer[buffer_flag++] = node;	// ��ʼ�ڵ���ջ 
    
	struct AdjListNode *check_node ;	// ����һ������ڵ����ڱ���
	while(buffer_flag)					// ����ڵ㻺��ջ�ǿ� 
    {
    	check_node = graph->array[node_buffer[--buffer_flag]].head;
    	while(check_node)
		{
			if(!Visited[check_node->dest])			// �������µĽڵ�û������ 
        	{
        		printf(" -> %d",check_node->dest);	// ��� 
        		Visited[check_node->dest] = 1;		// ����
				node_buffer[buffer_flag++] = check_node->dest; // ��ջ	 
			}
			check_node = check_node->next;			// ���ӵ���һ���ڵ� 
		} 
	}
    printf("\n");
} 
 
// �����������������Ϲ���
int main()
{
    /* �������������ͼ�� */
    int V = 5;
    struct Graph* graph = createGraph(V);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    /* ��ӡ����ͼ���ڽӱ��ʾ */
    printGraph(graph);
 
	/* ͼ�������������DFS */
	int begin_node;
	int Visited[V]={0};	// �Ѿ������Ľڵ㣬��ʼֵ����0����û���� 
    printf("\nPlease input the begin node��");
    scanf("%d",&begin_node);
    //DFS(graph,begin_node,Visited);	// ��ʼ��������DFS�ݹ�ʵ�� 
	DFS_Pro(graph,begin_node,Visited);	// ��ʼ��������DFS�ǵݹ�ʵ��
	for(int i=0;i<V;i++)				// ��չ۲���� 
		Visited[i]=0;
	BFS(graph,begin_node,Visited,V);	// ��ʼ��������BFS�ǵݹ�ʵ�� 
	
    return 0;
}
