#include<stdio.h> 
#define INF 127
#define num 5
/***************************************************
*	核心思想：通过中间点迭代出最短路径。
*	【注意】：遍历每个中间点都会更改原邻接矩阵，
*			  所以不用考虑2个点，3个点……
*			  已经通过迭代的方式处理完成。	 
***************************************************/


/* Floyed算法 */
void Floyd(unsigned char e[5][5], unsigned char n)  //函数Floyd实现弗洛伊德算法
{
    int i,j,k;
    for (k=0; k<=n-1; k++)                			// 最外层for循环(遍历允许通过的点)
        for (i=0; i<=n-1; i++)             			// 中间层for循环(遍历源节点) 
            for (j=0; j<=n-1; j++)         			// 最内层for循环(遍历目标节点) 
               if (e[i][j]>e[i][k]+e[k][j]) 		// 原始路径和新路径进行比较(进行更新迭代) 
                    e[i][j]=e[i][k]+e[k][j];

}

/* 输出邻接矩阵 */
void output_graph(unsigned char arr[][num])	// 二维数参数传递 
{
	int i,j; 
	for(i=0;i<num;i++)
    {
      	for(j=0;j<num;j++)
        	printf("%3d  ", arr[i][j]);   
        printf("\n");
    }
}


//-----------------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------------
int main()
{
    /* 初始化图的邻接矩阵arr[num][num] */
    unsigned char arr[num][num]={
        {0,   1,   1, INF,  1},
        {1,   0,   1, INF, INF},
        {1,   1,   0,  1,   1},
        {INF, INF, 1,  0,  INF},
        {1,   INF, 1, INF,  0}};

    printf("\n Old path is:\n");
    output_graph(arr);			 // 输出初始邻接矩阵 
	
	Floyd(arr, num);             // 实现弗洛伊德算法
     
    printf("\n New path is:\n");
    output_graph(arr);			 // 输出更新之后的邻接矩阵 
    return 0;
}

