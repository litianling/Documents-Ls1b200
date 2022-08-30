#include <stdio.h>
#include <stdlib.h>
#define Max_Node_Num 100


/* �洢�ṹ���� */
typedef struct Edge		// �߽ṹ�� 
{
    int locate_dest;	// �ñ������ӵ�Ŀ��ڵ��λ��
    Edge* next_Edge;	// ָ��->ָ��ñߵ���һ����
}Edge;


typedef struct List		// Դ�ڵ��б�ṹ�� 
{
    char data;			// �ڵ���Ϣ 
    Edge *first_edge;	// ָ��->ָ��ýڵ�ĵ�һ���� 
}List,SrcList[Max_Node_Num];


typedef struct Graph  	// ͼ�ṹ�� 
{
    SrcList srclist;	// ͼ�е�Դ�ڵ��б� 
    int node_number; 	// ͼ�Ķ�����
	int edge_number;	// ͼ�ı���
}Graph;


/* ��λ����������ȡ��ǰԪ��e�ڽڵ������еĴ洢λ�� */
int Locate(Graph graph,char data)
{
    for(int i = 0 ;i<graph.node_number;i++)	// ��ǰ�������ڵ� 
        if(graph.srclist[i].data==data)		// �ڵ�������e�ͷ���λ��i 
            return i;
    return -1;								// �������˶�û�ҵ��ͷ���λ��-1 
}


/* ��������ͼ */
void CreateGraph(Graph* graph)
{
    printf("������ͼ�м�����㣺\n  ");			// ����ͼ�ṹ��Ľڵ���
    scanf("%d",&(*graph).node_number);
    printf("������ͼ�м����ߣ�\n  ");			// ����ͼ�ṹ��ı��� 
    scanf("%d",&(*graph).edge_number);

    for(int i = 0 ; i<(*graph).node_number;i++)	// ��ʼ���ڵ�����
    {
        printf("�����붥��ֵ��\n  ");
        getchar();								// �Ե�����Ļس��� 
		scanf("%c",&(*graph).srclist[i].data); 	// �����i���ڵ����Ϣ 
        (*graph).srclist[i].first_edge = NULL;	// ��i���ڵ�ָ�������ΪNULL
    }
    
    for(int i = 0 ; i<(*graph).edge_number ; i++) 		// ��ʼ���� 
    {
        printf("���ڣ��������%d���ߣ�������Դ�ڵ��ֵ��\n  ",i+1);
        char src;
        getchar();
        scanf("%c",&src);
        printf("���ڣ��������%d���ߣ�������Ŀ��ڵ��ֵ��\n  ",i+1);
        char dest;
        getchar();
        scanf("%c",&dest);
        int locate_src = Locate(*graph,src);			// ��ȡԴ�ڵ�洢λ�� 
        int locate_dest = Locate(*graph,dest);			// ��ȡĿ��ڵ�洢λ��
		
		/* ͷ�巨 */
        Edge * new_edge1 = (Edge*)malloc(sizeof(Edge));					// ����һ����p 
        new_edge1->locate_dest = locate_dest;							// ���������ӵ�Ŀ��ڵ�洢λ�� 
        new_edge1->next_Edge = (*graph).srclist[locate_src].first_edge;	// �����ߵ���һ������Դ�ڵ����ӵĵ�һ���� 
        (*graph).srclist[locate_src].first_edge = new_edge1;			// �����߱��Դ�ڵ����ӵĵ�һ���� 
        
        Edge * new_edge2 = (Edge*)malloc(sizeof(Edge));					// ����ͼ˫����(ͬ��) 
        new_edge2->locate_dest = locate_src;
        new_edge2->next_Edge = (*graph).srclist[locate_dest].first_edge;
        (*graph).srclist[locate_dest].first_edge = new_edge2;
    }
}


/* ��ӡ�洢�ṹ */
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


/* ����������� */
void DFS(Graph graph,int node,int* Visited)	
{
    printf("%c ",graph.srclist[node].data);		// �����ʼ�Ľڵ� 
    Visited[node] = 1;							// ����ڵ��Ѿ������� 
    Edge * check_edge = graph.srclist[node].first_edge;	// ����һ�������ڱ�������ʼ�ڵ��һ���ߣ� 
    while(check_edge)							// ����߷ǿվͱ��� 
    {
        int new_node = check_edge->locate_dest;	// �µĽڵ������������ӵ�Ŀ��ڵ� 
        if(!Visited[new_node])					// �������µĽڵ�û�����������������½ڵ� 
            DFS(graph,new_node,Visited);
        check_edge = check_edge->next_Edge;		// ������һ���ڵ�������� 
    }
}


/* ������ */
int main()
{
    Graph graph;			// ����һ��ͼ�ṹ�� 
    CreateGraph(&graph);	// ����ͼ 
    
    output(graph);
    
    int begin_node;					// ��ʼ�����Ľڵ�
	int Visited[Max_Node_Num]={0};	// �Ѿ������Ľڵ㣬��ʼֵ����0����û���� 
    printf("������ӵڼ������㿪ʼ��������ʼλ��Ϊ0����\n  ");
    scanf("%d",&begin_node);
    DFS(graph,begin_node,Visited);	// ��ʼ���� 
    
	return 0;
}
