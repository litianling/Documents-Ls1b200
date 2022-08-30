#include <stdio.h>
#include <stdlib.h>


// 四个单位为一组：权值偏移量为0，父节点坐标偏移量为1，左孩子节点坐标偏移量为2，右孩子节点偏移量为3 


//在下标为1到i-1的范围找到权值最小的两个值的下标，其中坐标s1的权值小于坐标s2的权值
void Select(int* HF_Tree, int len, int& s1, int& s2)
{
	int min;
	// 找第一个最小值
	for (int j = 1; j <= len; j++)	// 先给min一个初始值，数组中第一个无父节点的坐标 
	{
		if (HF_Tree[j*4+1] == 0)
		{
			min = j;
			break;
		}
	}
	for (int j = min + 1; j <= len; j++)	// 找最小值点坐标 
	{
		if (HF_Tree[j*4+1] == 0 && HF_Tree[j*4] < HF_Tree[min*4])
			min = j;
	}
	s1 = min; // 第一个最小值点坐标给s1
	
	// 找第二个最小值
	for (int j = 1; j <= len; j++)	// 先给min一个初始值，数组中第一个无父节点的坐标，并且不能是已经用过的s1 
	{
		if (HF_Tree[j*4+1] == 0 && j != s1)
		{
			min = j;
			break;
		}
	}
	for (int j = min + 1; j <= len; j++)	// 找除s1之外的最小值点坐标 
	{
		if (HF_Tree[j*4+1] == 0 && HF_Tree[j*4] < HF_Tree[min*4]&&j != s1)
			min = j;
	}
	s2 = min; // 第二个最小值点坐标给s2
}

//构建哈夫曼树（构建出来的树，待编码数据，个数） 
void CreateHuff(int* HF_Tree, int* basic_date, int number)
{
	int m = 2 * number - 1; 			//哈夫曼树总结点数
	for (int i = 1; i <= number; i++)
	{
		HF_Tree[i*4] = basic_date[i - 1]; 		// 将权值赋给n个叶子结点
	}
	for (int i = number + 1; i <= m; i++) 	// 构建哈夫曼树，从number+1到m把辅助节点补充完毕 
	{
		// 选择权值最小的s1和s2，生成它们的父结点
		int s1, s2;
		Select(HF_Tree, i - 1, s1, s2); 	// 在下标为1到i-1的范围找到权值最小的两个值的下标，其中s1的权值小于s2的权值
		HF_Tree[i*4] = HF_Tree[s1*4] + HF_Tree[s2*4]; //i的权重是s1和s2的权重之和
		HF_Tree[s1*4+1] = i; 			// s1的父亲是i
		HF_Tree[s2*4+1] = i; 			// s2的父亲是i
		HF_Tree[i*4+2] = s1; 			// 左孩子是s1
		HF_Tree[i*4+3] = s2; 			// 右孩子是s2
	}
	// 打印哈夫曼树中各结点之间的关系
	printf("哈夫曼树为:>\n");
	printf("本节点坐标  本节点权值  父结点坐标  左孩子坐标  右孩子坐标\n");
	printf("0                                  \n");
	for (int i = 1; i <= m; i++)
	{
		printf("%-4d   	    %-4d   	%-6d      %-6d      %-6d\n", i, HF_Tree[i*4], HF_Tree[i*4+1], HF_Tree[i*4+2], HF_Tree[i*4+3]);
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

void HuffCoding(int* HF_Tree, char* HF_Code, int number)
{
	char code[8]={0}; 				// 辅助空间，编码最长为n(最长时，前n-1个用于存储数据，最后1个用于存放'\0')
	code[number - 1] = '\0'; 		// 辅助空间最后一个位置为'\0'
	for (int i = 1; i <= number; i++)	// 对number个数据依次编码 
	{
		int start = number - 1; 	// 每次生成数据的哈夫曼编码之前，先将start指针指向'\0'
		int c = i; 					// 正在进行的第i个数据的编码
		int p = HF_Tree[c*4+1]; 	// 找到该数据的父结点
		while (p) 					// 直到父结点为0，即父结点为根结点时，停止
		{
			if (HF_Tree[p*4+2] == c) 	// 如果该结点是其父结点的左孩子，则编码为0，否则为1
			{
				start--;
				code[start] = '0';	
			}
			else
			{
				start--;
				code[start] = '1';	
			}
			c = p; 						// 继续往上进行编码
			p = HF_Tree[c*4+1];   		// 继续找c的父结点
		}
		for(int j=0;code[start]!=NULL;j++)
		{
			HF_Code[i*8+j] = code[start];
			start++;
		}  
		
	}
}


//主函数
int main()
{
	/* 待编码数据数组w */
	int number = 8;
	int basic_date[8]={1,2,3,4,5,6,7,8}; 
	
	/* 构建哈夫曼树 */					// 哈夫曼节点2*number-1 
	int HF_Tree[64]={0};				// 因为下标为0的组不存储数据，所以开节点个数+1组（每组四个）即（（2*8-1）+1）*4 
	CreateHuff(HF_Tree, basic_date, number); 


	/* 构建哈夫曼编码 */
	char HF_Code[72]={0};				// 8个为一组i*8+j   9*8
	HuffCoding(HF_Tree, HF_Code, number); 


	/* 打印哈夫曼编码 */
	for (int i = 1; i <= number; i++) 
	{
		printf("数据<%d>的编码为:", HF_Tree[i*4]);
		for(int j=0;j<6;j++) 
			printf("%c", HF_Code[i*8+j]);
		printf("\n");
	}

	return 0;
}

