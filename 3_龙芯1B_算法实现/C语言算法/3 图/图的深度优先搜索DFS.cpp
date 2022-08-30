#include <stdio.h>
#include <stdlib.h>
#define Max_Node_Num 100


/* 存储结构定义 */
typedef struct Edge		// 边结构体 
{
    int locate_dest;	// 该边所连接的目标节点的位置
    Edge* next_Edge;	// 指针->指向该边的下一条边
}Edge;


typedef struct List		// 源节点列表结构体 
{
    char data;			// 节点信息 
    Edge *first_edge;	// 指针->指向该节点的第一条边 
}List,SrcList[Max_Node_Num];


typedef struct Graph  	// 图结构体 
{
    SrcList srclist;	// 图中的源节点列表 
    int node_number; 	// 图的顶点数
	int edge_number;	// 图的边数
}Graph;


/* 定位函数——获取当前元素e在节点数组中的存储位置 */
int Locate(Graph graph,char data)
{
    for(int i = 0 ;i<graph.node_number;i++)	// 从前向后遍历节点 
        if(graph.srclist[i].data==data)		// 节点内容是e就返回位置i 
            return i;
    return -1;								// 遍历完了都没找到就返回位置-1 
}


/* 创建无向图 */
void CreateGraph(Graph* graph)
{
    printf("请输入图有几个结点：\n  ");			// 输入图结构体的节点数
    scanf("%d",&(*graph).node_number);
    printf("请输入图有几条边：\n  ");			// 输入图结构体的边数 
    scanf("%d",&(*graph).edge_number);

    for(int i = 0 ; i<(*graph).node_number;i++)	// 初始化节点数组
    {
        printf("请输入顶点值：\n  ");
        getchar();								// 吃掉输入的回车键 
		scanf("%c",&(*graph).srclist[i].data); 	// 输入第i个节点的信息 
        (*graph).srclist[i].first_edge = NULL;	// 第i个节点指向的连接为NULL
    }
    
    for(int i = 0 ; i<(*graph).edge_number ; i++) 		// 初始化边 
    {
        printf("现在，来构造第%d条边，请输入源节点的值：\n  ",i+1);
        char src;
        getchar();
        scanf("%c",&src);
        printf("现在，来构造第%d条边，请输入目标节点的值：\n  ",i+1);
        char dest;
        getchar();
        scanf("%c",&dest);
        int locate_src = Locate(*graph,src);			// 获取源节点存储位置 
        int locate_dest = Locate(*graph,dest);			// 获取目标节点存储位置
		
		/* 头插法 */
        Edge * new_edge1 = (Edge*)malloc(sizeof(Edge));					// 构造一条边p 
        new_edge1->locate_dest = locate_dest;							// 这条边连接的目标节点存储位置 
        new_edge1->next_Edge = (*graph).srclist[locate_src].first_edge;	// 这条边的下一条边是源节点连接的第一条边 
        (*graph).srclist[locate_src].first_edge = new_edge1;			// 这条边变成源节点连接的第一条边 
        
        Edge * new_edge2 = (Edge*)malloc(sizeof(Edge));					// 无向图双向构造(同上) 
        new_edge2->locate_dest = locate_src;
        new_edge2->next_Edge = (*graph).srclist[locate_dest].first_edge;
        (*graph).srclist[locate_dest].first_edge = new_edge2;
    }
}


/* 打印存储结构 */
void output(Graph graph)
{
	for(int i=0;i<graph.node_number;i++)
	{
		printf("\n  begin: %c",graph.srclist[i].data);
		for(Edge *edge=graph.srclist[i].first_edge;edge;edge=edge->next_Edge)
		{
			printf(" -> %c",graph.srclist[edge->locate_dest].data);
		}
		printf("\n\n");
	}
} 


/* 深度优先搜索 */
void DFS(Graph graph,int node,int* Visited)	
{
    printf("%c ",graph.srclist[node].data);		// 输出开始的节点 
    Visited[node] = 1;							// 这个节点已经看过了 
    Edge * check_edge = graph.srclist[node].first_edge;	// 创建一条边用于遍历（起始节点第一条边） 
    while(check_edge)							// 这个边非空就遍历 
    {
        int new_node = check_edge->locate_dest;	// 新的节点是这条边连接的目标节点 
        if(!Visited[new_node])					// 如果这个新的节点没被哈看过，就搜索新节点 
            DFS(graph,new_node,Visited);
        check_edge = check_edge->next_Edge;		// 跳入下一个节点进行搜索 
    }
}


/* 主函数 */
int main()
{
    Graph graph;			// 声明一个图结构体 
    CreateGraph(&graph);	// 创建图 
    
    output(graph);
    
    int begin_node;					// 开始遍历的节点
	int Visited[Max_Node_Num]={0};	// 已经看过的节点，初始值都是0代表都没看过 
    printf("请输入从第几个顶点开始遍历（起始位置为0）：\n  ");
    scanf("%d",&begin_node);
    DFS(graph,begin_node,Visited);	// 开始遍历 
    
	return 0;
}
