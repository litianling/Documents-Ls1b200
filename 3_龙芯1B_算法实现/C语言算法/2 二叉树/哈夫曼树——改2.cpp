#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int DataType; //���Ȩֵ����������

typedef struct HTNode //����������Ϣ
{
	DataType weight; 	//Ȩֵ		ֵ 
	int parent; 		//���ڵ�	���� 
	int left_child;		//���ӽڵ�����
	int right_child; 	//�Һ��ӽڵ����� 
}*HuffmanTree;

typedef char **HuffmanCode; //�ַ�ָ�������д洢��Ԫ������

//���±�Ϊ1��i-1�ķ�Χ�ҵ�Ȩֵ��С������ֵ���±꣬��������s1��ȨֵС������s2��Ȩֵ
void Select(HuffmanTree& HF_Tree, int number, int& s1, int& s2)
{
	int min;
	// �ҵ�һ����Сֵ
	for (int i = 1; i <= number; i++)	// �ȸ�minһ����ʼֵ�������е�һ���޸��ڵ������ 
	{
		if (HF_Tree[i].parent == 0)
		{
			min = i;
			break;
		}
	}
	for (int i = min + 1; i <= number; i++)	// ����Сֵ������ 
	{
		if (HF_Tree[i].parent == 0 && HF_Tree[i].weight < HF_Tree[min].weight)
			min = i;
	}
	s1 = min; // ��һ����Сֵ�������s1
	// �ҵڶ�����Сֵ
	for (int i = 1; i <= number; i++)	// �ȸ�minһ����ʼֵ�������е�һ���޸��ڵ�����꣬���Ҳ������Ѿ��ù���s1 
	{
		if (HF_Tree[i].parent == 0 && i != s1)
		{
			min = i;
			break;
		}
	}
	for (int i = min + 1; i <= number; i++)	// �ҳ�s1֮�����Сֵ������ 
	{
		if (HF_Tree[i].parent == 0 && HF_Tree[i].weight < HF_Tree[min].weight&&i != s1)
			min = i;
	}
	s2 = min; // �ڶ�����Сֵ�������s2
}

//�������������������������������������ݣ������� 
void CreateHuff(HuffmanTree& HF_Tree, DataType* w, int number)
{
	int m = 2 * number - 1; //���������ܽ����
	HF_Tree = (HuffmanTree)calloc(m + 1, sizeof(HTNode)); //��m+1��HTNode����Ϊ�±�Ϊ0��HTNode���洢����
	for (int i = 1; i <= number; i++)
	{
		HF_Tree[i].weight = w[i - 1]; 		// ��Ȩֵ����n��Ҷ�ӽ��
	}
	for (int i = number + 1; i <= m; i++) 	// ����������������number+1��m�Ѹ����ڵ㲹����� 
	{
		// ѡ��Ȩֵ��С��s1��s2���������ǵĸ����
		int s1, s2;
		Select(HF_Tree, i - 1, s1, s2); 	// ���±�Ϊ1��i-1�ķ�Χ�ҵ�Ȩֵ��С������ֵ���±꣬����s1��ȨֵС��s2��Ȩֵ
		HF_Tree[i].weight = HF_Tree[s1].weight + HF_Tree[s2].weight; //i��Ȩ����s1��s2��Ȩ��֮��
		HF_Tree[s1].parent = i; 			// s1�ĸ�����i
		HF_Tree[s2].parent = i; 			// s2�ĸ�����i
		HF_Tree[i].left_child = s1; 				// ������s1
		HF_Tree[i].right_child = s2; 				// �Һ�����s2
	}
	// ��ӡ���������и����֮��Ĺ�ϵ
	printf("��������Ϊ:>\n");
	printf("���ڵ�����  ���ڵ�Ȩֵ  ���������  ��������  �Һ�������\n");
	printf("0                                  \n");
	for (int i = 1; i <= m; i++)
	{
		printf("%-4d   	    %-4d   	%-6d      %-6d      %-6d\n", i, HF_Tree[i].weight, HF_Tree[i].parent, HF_Tree[i].left_child, HF_Tree[i].right_child);
	}
	printf("\n");
}

/*******************************************************************************************************
*
* 	HF_Code��ָ��ָ���ָ�룺����(number+1)����СΪ(char*)�����ݿռ䣬HF_Code[i]�д�ŵ���ָ���ַ����͵�ָ�� 
*	HF_Code[i]��ָ���ַ����͵�ָ�룺�ڸ�ָ��ָ��ĵ�ַ�£�����(number-start)����СΪchar�����ݿռ䣬����ַ� 
*
*******************************************************************************************************/
//���ɹ���������
void HuffCoding(HuffmanTree& HF_Tree, HuffmanCode& HF_Code, int number)
{
	HF_Code = (HuffmanCode)malloc(sizeof(char*)*(number + 1)); 	// ��n+1���ռ䣬��Ϊ�±�Ϊ0�Ŀռ䲻��
	char* code = (char*)malloc(sizeof(char)*number); 		// �����ռ䣬�����Ϊn(�ʱ��ǰn-1�����ڴ洢���ݣ����1�����ڴ��'\0')
	code[number - 1] = '\0'; 		// �����ռ����һ��λ��Ϊ'\0'
	for (int i = 1; i <= number; i++)	// ��number���������α��� 
	{
		int start = number - 1; 	// ÿ���������ݵĹ���������֮ǰ���Ƚ�startָ��ָ��'\0'
		int c = i; 					// ���ڽ��еĵ�i�����ݵı���
		int p = HF_Tree[c].parent; 	// �ҵ������ݵĸ����
		while (p) 					// ֱ�������Ϊ0���������Ϊ�����ʱ��ֹͣ
		{
			if (HF_Tree[p].left_child == c) 	// ����ý�����丸�������ӣ������Ϊ0������Ϊ1
				code[--start] = '0';
			else
				code[--start] = '1';
			c = p; 						// �������Ͻ��б���
			p = HF_Tree[c].parent;   	// ������c�ĸ����
		}
		HF_Code[i] = (char*)malloc(sizeof(char)*(number - start)); 		// �������ڴ洢������ڴ�ռ�
		// strcpy(HF_Code[i], &code[start]); 							// �����뿽�����ַ�ָ�������е���Ӧλ��
		
		for(int j=0;code[start]!=NULL;j++)
		{
			HF_Code[i][j] = code[start];
			start++;
		}  
		
	}
	free(code); //�ͷŸ����ռ�
}


//������
int main()
{
	/* ��������������w */
	int number = 8;
	int w[8]={1,2,3,4,5,6,7,8}; 
	
	/* ������������ */
	HuffmanTree HF_Tree;
	CreateHuff(HF_Tree, w, number); 

	/* �������������� */
	HuffmanCode HF_Code;
	HuffCoding(HF_Tree, HF_Code, number); 

	/* ��ӡ���������� */
	for (int i = 1; i <= number; i++) 
	{
		printf("����<%d>�ı���Ϊ:%s\n", HF_Tree[i].weight, HF_Code[i]);
	}
	
	return 0;
}

