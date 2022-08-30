#include <stdio.h>
#define node_number 5		// ͼ�ڵ���	
#define max_edge_number 7	// ����ͼ����м����߱� 
#define max_graph_size 33	// 4*7+5 

int graph[max_graph_size]={0}; 	// ǰ������ֵ���һ�飬�洢�������¸��ڵ�λ�á����������һ�飬��ֵ���¸��ڵ�λ�ã� 
int graph_label=node_number;	// ��ʼ�洢λ��	
int Visited[node_number]={0};	// ����->�Ѿ��۲��־ 

/* ���·�� */
int AddOneNode(int src,int dest)	// ���·����Դ�ڵ㣬Ŀ��ڵ㣩 
{
	if(graph[src]==0) 			// ���ͼ�е�src�ڽӱ��ͷΪ�գ�ԭ��û�ڵ�=>��Ҫ�ŵ��ǵ�һ���ڵ㣩
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

	if(graph[dest]==0) 			// ���ͼ�е�src�ڽӱ��ͷΪ�գ�ԭ��û�ڵ�=>��Ҫ�ŵ��ǵ�һ���ڵ㣩
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


/* ��ӡͼ��ϵ */
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


/* �Ľ�->��ӡ����洢�ṹ */
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


/* ��չ->�����������DFS */
void printDFS(int node)
{
    printf("\n DFS: %d",node);					// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ����ڵ��Ѿ������� 
    int check = graph[node];					// check�ǵ�ַ	 
    while(check)								// check��0������������һ���� 
    {
    	if(!Visited[graph[check+1]])			// graph[check+1]�ǽڵ�ֵ
    	{
       		printf(" -> %d",graph[check+1]);
    		Visited[graph[check+1]] = 1;
        	check = graph[check+1];				// check=ͷָ�� 
			check = graph[check]; 				// check=ͷָ��ָ��Ľڵ� 
        	continue;
		}
		check = graph[check];
	}
} 


/* ��չ->�����������BFS */
void printBFS(int node,int v)
{
	int node_buffer[v] = {0};
	int buffer_flag = 0;
    printf("\n BFS: %d",node);					// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ��ʼ�ڵ��Ѿ������� 
    node_buffer[buffer_flag++] = node;			// ��ʼ�ڵ���ջ 
    
    int check;
	while(buffer_flag)							// ����ڵ㻺��ջ�ǿ� 
    {
		check=graph[node_buffer[--buffer_flag]];
		while(check)
		{
			if(!Visited[graph[check+1]])		// graph[check+1]�ǽڵ�ֵ
			{
       			printf(" -> %d",graph[check+1]);
       			node_buffer[buffer_flag++] = graph[check+1]; // ��ջ
    			Visited[graph[check+1]] = 1;	
			}
			check = graph[check];
		}
	}
    printf("\n"); 
}


/* ������ */
int main()
{
	AddOneNode(0,1);
	AddOneNode(0,4);
	AddOneNode(1,2);
	AddOneNode(1,3);
	AddOneNode(1,4);
	AddOneNode(2,3);
	AddOneNode(3,4);

	printGraph(); 					// ��ӡͼ 
	printArray();					// ��ӡ�洢�ṹ 
	printDFS(0);					// ����������� 
	for(int i=0;i<node_number;i++)	// ��չ۲���� 
		Visited[i]=0;
	printBFS(0,5);					// ����������� 
} 


// ������������ı��취--���� 
// flag��0����ͷָ�루check�ǵ�ǰ�ڵ����꣬graph[check]���¸��ڵ����꣬check�ǵ�ǰ�ڵ�ֵ��
//		 1������ָ�루check�ǵ�ǰ�ڵ����꣬graph[check]���¸��ڵ����꣬graph[check+1]�ǵ�ǰ�ڵ�ֵ��
/* 
void printDFS(int node)
{
    printf("\n DFS: %d",node);					// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ����ڵ��Ѿ������� 
    int check = graph[node];					// check�ǵ�ַ
	bool flag = 1;			 
    while(check)								// check��0������������һ���� 
    {
    	if((flag==1)&&(!Visited[graph[check+1]]))	// graph[check+1]�ǽڵ�ֵ
    	{
       		printf(" -> %d",graph[check+1]);
    		Visited[graph[check+1]] = 1;
        	check = graph[check+1];
			flag = 0; 
        	continue;
		}
		else if((flag==0)&&(!Visited[check]))		// check�ǽڵ�ֵ 
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
