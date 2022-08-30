#include <stdio.h>
#define node_number 5		// 图节点数	
#define max_edge_number 7	// 无向图最多有几条边边 
#define max_graph_size 33	// 4*7+5 

int graph[max_graph_size]={0}; 	// 前五个数字单个一组，存储链表中下个节点位置。后面的两个一组，（值，下个节点位置） 
int graph_label=node_number;	// 起始存储位置	
int Visited[node_number]={0};	// 数组->已经观测标志 

/* 添加路劲 */
int AddOneNode(int src,int dest)	// 添加路径（源节点，目标节点） 
{
	if(graph[src]==0) 			// 如果图中的src邻接表表头为空（原来没节点=>需要放的是第一个节点）
	{
		graph[src]=graph_label;
		graph_label++;
		graph[graph_label]=dest;
		graph_label++;
	}
	else
	{
		int check=src;
		while(graph[check]!=0)
		{
			check = graph[check];
		}
		graph[check]=graph_label;
		graph_label++;
		graph[graph_label]=dest;
		graph_label++;
	}

	if(graph[dest]==0) 			// 如果图中的src邻接表表头为空（原来没节点=>需要放的是第一个节点）
	{
		graph[dest]=graph_label;
		graph_label++;
		graph[graph_label]=src;
		graph_label++;
	}
	else
	{
		int check=dest;
		while(graph[check]!=0)
		{
			check = graph[check];
		}
		graph[check]=graph_label;
		graph_label++;
		graph[graph_label]=src;
		graph_label++;
	}

}


/* 打印图关系 */
void printGraph()
{
	for(int i=0;i<node_number;i++)
	{
		int check;
		printf(" \n Adjacency list of vertex %d \n head",i);
		for(check=graph[i];check!=0;check=graph[check])
		{
			printf(" -> %d",graph[check+1]);
		}
		printf("\n");
	}
}


/* 改进->打印数组存储结构 */
void printArray()
{
	int i,j; 
	printf("\n\n array is : ");
	for(i=0;i<node_number;i++)
		printf("%d ",graph[i]);
	printf("|| ");
	for(;i<max_graph_size;i++)
	{
		printf("%d ",graph[i]);
		j++;
		if(j%2==0)
			printf("| ");
	}
	printf("\n");
}


/* 拓展->深度优先搜索DFS */
void printDFS(int node)
{
    printf("\n DFS: %d",node);					// 输出开始的节点 
    Visited[node] = 1;							// 这个节点已经看过了 
    int check = graph[node];					// check是地址	 
    while(check)								// check非0，有连接着下一个点 
    {
    	if(!Visited[graph[check+1]])			// graph[check+1]是节点值
    	{
       		printf(" -> %d",graph[check+1]);
    		Visited[graph[check+1]] = 1;
        	check = graph[check+1];				// check=头指针 
			check = graph[check]; 				// check=头指针指向的节点 
        	continue;
		}
		check = graph[check];
	}
} 


/* 拓展->广度优先搜索BFS */
void printBFS(int node,int v)
{
	int node_buffer[v] = {0};
	int buffer_flag = 0;
    printf("\n BFS: %d",node);					// 输出开始的节点 
    Visited[node] = 1;							// 开始节点已经看过了 
    node_buffer[buffer_flag++] = node;			// 开始节点入栈 
    
    int check;
	while(buffer_flag)							// 这个节点缓存栈非空 
    {
		check=graph[node_buffer[--buffer_flag]];
		while(check)
		{
			if(!Visited[graph[check+1]])		// graph[check+1]是节点值
			{
       			printf(" -> %d",graph[check+1]);
       			node_buffer[buffer_flag++] = graph[check+1]; // 入栈
    			Visited[graph[check+1]] = 1;	
			}
			check = graph[check];
		}
	}
    printf("\n"); 
}


/* 主函数 */
int main()
{
	AddOneNode(0,1);
	AddOneNode(0,4);
	AddOneNode(1,2);
	AddOneNode(1,3);
	AddOneNode(1,4);
	AddOneNode(2,3);
	AddOneNode(3,4);

	printGraph(); 					// 打印图 
	printArray();					// 打印存储结构 
	printDFS(0);					// 深度优先搜索 
	for(int i=0;i<node_number;i++)	// 清空观测矩阵 
		Visited[i]=0;
	printBFS(0,5);					// 广度优先搜索 
} 


// 深度优先搜索的笨办法--舍弃 
// flag：0加载头指针（check是当前节点坐标，graph[check]是下个节点坐标，check是当前节点值）
//		 1加载链指针（check是当前节点坐标，graph[check]是下个节点坐标，graph[check+1]是当前节点值）
/* 
void printDFS(int node)
{
    printf("\n DFS: %d",node);					// 输出开始的节点 
    Visited[node] = 1;							// 这个节点已经看过了 
    int check = graph[node];					// check是地址
	bool flag = 1;			 
    while(check)								// check非0，有连接着下一个点 
    {
    	if((flag==1)&&(!Visited[graph[check+1]]))	// graph[check+1]是节点值
    	{
       		printf(" -> %d",graph[check+1]);
    		Visited[graph[check+1]] = 1;
        	check = graph[check+1];
			flag = 0; 
        	continue;
		}
		else if((flag==0)&&(!Visited[check]))		// check是节点值 
		{
   			printf(" -> %d",check);
    		Visited[check] = 1;
       	    check = graph[check+1]; 
			flag = 0; 
			continue;
		}
		check = graph[check];
		flag = 1; 
	}
} 
*/
