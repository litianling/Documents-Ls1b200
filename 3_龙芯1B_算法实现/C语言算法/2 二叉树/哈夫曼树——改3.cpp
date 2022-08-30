#include <stdio.h>
#include <stdlib.h>


// �ĸ���λΪһ�飺Ȩֵƫ����Ϊ0�����ڵ�����ƫ����Ϊ1�����ӽڵ�����ƫ����Ϊ2���Һ��ӽڵ�ƫ����Ϊ3 


//���±�Ϊ1��i-1�ķ�Χ�ҵ�Ȩֵ��С������ֵ���±꣬��������s1��ȨֵС������s2��Ȩֵ
void Select(int* HF_Tree, int len, int& s1, int& s2)
{
	int min;
	// �ҵ�һ����Сֵ
	for (int j = 1; j <= len; j++)	// �ȸ�minһ����ʼֵ�������е�һ���޸��ڵ������ 
	{
		if (HF_Tree[j*4+1] == 0)
		{
			min = j;
			break;
		}
	}
	for (int j = min + 1; j <= len; j++)	// ����Сֵ������ 
	{
		if (HF_Tree[j*4+1] == 0 && HF_Tree[j*4] < HF_Tree[min*4])
			min = j;
	}
	s1 = min; // ��һ����Сֵ�������s1
	
	// �ҵڶ�����Сֵ
	for (int j = 1; j <= len; j++)	// �ȸ�minһ����ʼֵ�������е�һ���޸��ڵ�����꣬���Ҳ������Ѿ��ù���s1 
	{
		if (HF_Tree[j*4+1] == 0 && j != s1)
		{
			min = j;
			break;
		}
	}
	for (int j = min + 1; j <= len; j++)	// �ҳ�s1֮�����Сֵ������ 
	{
		if (HF_Tree[j*4+1] == 0 && HF_Tree[j*4] < HF_Tree[min*4]&&j != s1)
			min = j;
	}
	s2 = min; // �ڶ�����Сֵ�������s2
}

//�������������������������������������ݣ������� 
void CreateHuff(int* HF_Tree, int* basic_date, int number)
{
	int m = 2 * number - 1; 			//���������ܽ����
	for (int i = 1; i <= number; i++)
	{
		HF_Tree[i*4] = basic_date[i - 1]; 		// ��Ȩֵ����n��Ҷ�ӽ��
	}
	for (int i = number + 1; i <= m; i++) 	// ����������������number+1��m�Ѹ����ڵ㲹����� 
	{
		// ѡ��Ȩֵ��С��s1��s2���������ǵĸ����
		int s1, s2;
		Select(HF_Tree, i - 1, s1, s2); 	// ���±�Ϊ1��i-1�ķ�Χ�ҵ�Ȩֵ��С������ֵ���±꣬����s1��ȨֵС��s2��Ȩֵ
		HF_Tree[i*4] = HF_Tree[s1*4] + HF_Tree[s2*4]; //i��Ȩ����s1��s2��Ȩ��֮��
		HF_Tree[s1*4+1] = i; 			// s1�ĸ�����i
		HF_Tree[s2*4+1] = i; 			// s2�ĸ�����i
		HF_Tree[i*4+2] = s1; 			// ������s1
		HF_Tree[i*4+3] = s2; 			// �Һ�����s2
	}
	// ��ӡ���������и����֮��Ĺ�ϵ
	printf("��������Ϊ:>\n");
	printf("���ڵ�����  ���ڵ�Ȩֵ  ���������  ��������  �Һ�������\n");
	printf("0                                  \n");
	for (int i = 1; i <= m; i++)
	{
		printf("%-4d   	    %-4d   	%-6d      %-6d      %-6d\n", i, HF_Tree[i*4], HF_Tree[i*4+1], HF_Tree[i*4+2], HF_Tree[i*4+3]);
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

void HuffCoding(int* HF_Tree, char* HF_Code, int number)
{
	char code[8]={0}; 				// �����ռ䣬�����Ϊn(�ʱ��ǰn-1�����ڴ洢���ݣ����1�����ڴ��'\0')
	code[number - 1] = '\0'; 		// �����ռ����һ��λ��Ϊ'\0'
	for (int i = 1; i <= number; i++)	// ��number���������α��� 
	{
		int start = number - 1; 	// ÿ���������ݵĹ���������֮ǰ���Ƚ�startָ��ָ��'\0'
		int c = i; 					// ���ڽ��еĵ�i�����ݵı���
		int p = HF_Tree[c*4+1]; 	// �ҵ������ݵĸ����
		while (p) 					// ֱ�������Ϊ0���������Ϊ�����ʱ��ֹͣ
		{
			if (HF_Tree[p*4+2] == c) 	// ����ý�����丸�������ӣ������Ϊ0������Ϊ1
			{
				start--;
				code[start] = '0';	
			}
			else
			{
				start--;
				code[start] = '1';	
			}
			c = p; 						// �������Ͻ��б���
			p = HF_Tree[c*4+1];   		// ������c�ĸ����
		}
		for(int j=0;code[start]!=NULL;j++)
		{
			HF_Code[i*8+j] = code[start];
			start++;
		}  
		
	}
}


//������
int main()
{
	/* ��������������w */
	int number = 8;
	int basic_date[8]={1,2,3,4,5,6,7,8}; 
	
	/* ������������ */					// �������ڵ�2*number-1 
	int HF_Tree[64]={0};				// ��Ϊ�±�Ϊ0���鲻�洢���ݣ����Կ��ڵ����+1�飨ÿ���ĸ���������2*8-1��+1��*4 
	CreateHuff(HF_Tree, basic_date, number); 


	/* �������������� */
	char HF_Code[72]={0};				// 8��Ϊһ��i*8+j   9*8
	HuffCoding(HF_Tree, HF_Code, number); 


	/* ��ӡ���������� */
	for (int i = 1; i <= number; i++) 
	{
		printf("����<%d>�ı���Ϊ:", HF_Tree[i*4]);
		for(int j=0;j<6;j++) 
			printf("%c", HF_Code[i*8+j]);
		printf("\n");
	}

	return 0;
}

