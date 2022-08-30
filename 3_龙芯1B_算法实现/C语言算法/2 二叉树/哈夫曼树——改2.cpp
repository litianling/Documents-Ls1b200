#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int DataType; //结点权值的数据类型

typedef struct HTNode //单个结点的信息
{
	DataType weight; 	//权值		值 
	int parent; 		//父节点	坐标 
	int left_child;		//左孩子节点坐标
	int right_child; 	//右孩子节点坐标 
}*HuffmanTree;

typedef char **HuffmanCode; //字符指针数组中存储的元素类型

//在下标为1到i-1的范围找到权值最小的两个值的下标，其中坐标s1的权值小于坐标s2的权值
void Select(HuffmanTree& HF_Tree, int number, int& s1, int& s2)
{
	int min;
	// 找第一个最小值
	for (int i = 1; i <= number; i++)	// 先给min一个初始值，数组中第一个无父节点的坐标 
	{
		if (HF_Tree[i].parent == 0)
		{
			min = i;
			break;
		}
	}
	for (int i = min + 1; i <= number; i++)	// 找最小值点坐标 
	{
		if (HF_Tree[i].parent == 0 && HF_Tree[i].weight < HF_Tree[min].weight)
			min = i;
	}
	s1 = min; // 第一个最小值点坐标给s1
	// 找第二个最小值
	for (int i = 1; i <= number; i++)	// 先给min一个初始值，数组中第一个无父节点的坐标，并且不能是已经用过的s1 
	{
		if (HF_Tree[i].parent == 0 && i != s1)
		{
			min = i;
			break;
		}
	}
	for (int i = min + 1; i <= number; i++)	// 找除s1之外的最小值点坐标 
	{
		if (HF_Tree[i].parent == 0 && HF_Tree[i].weight < HF_Tree[min].weight&&i != s1)
			min = i;
	}
	s2 = min; // 第二个最小值点坐标给s2
}

//构建哈夫曼树（构建出来的树，待编码数据，个数） 
void CreateHuff(HuffmanTree& HF_Tree, DataType* w, int number)
{
	int m = 2 * number - 1; //哈夫曼树总结点数
	HF_Tree = (HuffmanTree)calloc(m + 1, sizeof(HTNode)); //开m+1个HTNode，因为下标为0的HTNode不存储数据
	for (int i = 1; i <= number; i++)
	{
		HF_Tree[i].weight = w[i - 1]; 		// 将权值赋给n个叶子结点
	}
	for (int i = number + 1; i <= m; i++) 	// 构建哈夫曼树，从number+1到m把辅助节点补充完毕 
	{
		// 选择权值最小的s1和s2，生成它们的父结点
		int s1, s2;
		Select(HF_Tree, i - 1, s1, s2); 	// 在下标为1到i-1的范围找到权值最小的两个值的下标，其中s1的权值小于s2的权值
		HF_Tree[i].weight = HF_Tree[s1].weight + HF_Tree[s2].weight; //i的权重是s1和s2的权重之和
		HF_Tree[s1].parent = i; 			// s1的父亲是i
		HF_Tree[s2].parent = i; 			// s2的父亲是i
		HF_Tree[i].left_child = s1; 				// 左孩子是s1
		HF_Tree[i].right_child = s2; 				// 右孩子是s2
	}
	// 打印哈夫曼树中各结点之间的关系
	printf("哈夫曼树为:>\n");
	printf("本节点坐标  本节点权值  父结点坐标  左孩子坐标  右孩子坐标\n");
	printf("0                                  \n");
	for (int i = 1; i <= m; i++)
	{
		printf("%-4d   	    %-4d   	%-6d      %-6d      %-6d\n", i, HF_Tree[i].weight, HF_Tree[i].parent, HF_Tree[i].left_child, HF_Tree[i].right_child);
	}
	printf("\n");
}

/*******************************************************************************************************
*
* 	HF_Code是指向指针的指针：开辟(number+1)个大小为(char*)的数据空间，HF_Code[i]中存放的是指向字符类型的指针 
*	HF_Code[i]是指向字符类型的指针：在该指针指向的地址下，开辟(number-start)个大小为char的数据空间，存放字符 
*
*******************************************************************************************************/
//生成哈夫曼编码
void HuffCoding(HuffmanTree& HF_Tree, HuffmanCode& HF_Code, int number)
{
	HF_Code = (HuffmanCode)malloc(sizeof(char*)*(number + 1)); 	// 开n+1个空间，因为下标为0的空间不用
	char* code = (char*)malloc(sizeof(char)*number); 		// 辅助空间，编码最长为n(最长时，前n-1个用于存储数据，最后1个用于存放'\0')
	code[number - 1] = '\0'; 		// 辅助空间最后一个位置为'\0'
	for (int i = 1; i <= number; i++)	// 对number个数据依次编码 
	{
		int start = number - 1; 	// 每次生成数据的哈夫曼编码之前，先将start指针指向'\0'
		int c = i; 					// 正在进行的第i个数据的编码
		int p = HF_Tree[c].parent; 	// 找到该数据的父结点
		while (p) 					// 直到父结点为0，即父结点为根结点时，停止
		{
			if (HF_Tree[p].left_child == c) 	// 如果该结点是其父结点的左孩子，则编码为0，否则为1
				code[--start] = '0';
			else
				code[--start] = '1';
			c = p; 						// 继续往上进行编码
			p = HF_Tree[c].parent;   	// 继续找c的父结点
		}
		HF_Code[i] = (char*)malloc(sizeof(char)*(number - start)); 		// 开辟用于存储编码的内存空间
		// strcpy(HF_Code[i], &code[start]); 							// 将编码拷贝到字符指针数组中的相应位置
		
		for(int j=0;code[start]!=NULL;j++)
		{
			HF_Code[i][j] = code[start];
			start++;
		}  
		
	}
	free(code); //释放辅助空间
}


//主函数
int main()
{
	/* 待编码数据数组w */
	int number = 8;
	int w[8]={1,2,3,4,5,6,7,8}; 
	
	/* 构建哈夫曼树 */
	HuffmanTree HF_Tree;
	CreateHuff(HF_Tree, w, number); 

	/* 构建哈夫曼编码 */
	HuffmanCode HF_Code;
	HuffCoding(HF_Tree, HF_Code, number); 

	/* 打印哈夫曼编码 */
	for (int i = 1; i <= number; i++) 
	{
		printf("数据<%d>的编码为:%s\n", HF_Tree[i].weight, HF_Code[i]);
	}
	
	return 0;
}

