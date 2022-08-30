#include"stdio.h"
#define length  8 

// �����䣬����=���ֽ�=16λ 
// Ϊ�˷���������ֲ�� barrelû�ö�ά���� 

//int arr[length] = {3,8,1,5,2,4,6,7};			// ���������飬i��arr[i]��Ӧ��ĳһλ��j 
int arr[length] = {33,58,999,0,233,54,133,7};	// ���������飬i��arr[i]��Ӧ��ĳһλ��j
int record[10] = {0};							// Ͱ��Ԫ�ظ�����¼��j��record[j]��k 
int barrel[length*10] = {0};					// Ͱ��k��barrel[j*len+k]��Ͱ�е�Ԫ�� 

void Radix_Sort(int *arr, int *record, int *barrel, int len, int max_scale)
{
	int i,j,k;
	int min_scale = 1; 
	while(min_scale<max_scale)		// �ӵ�λ���λ�ݽ� 
	{
		int mid_scale = min_scale * 10;
		for(i=0;i<len;i++)			// �������������� 
		{
			j = arr[i] % mid_scale;	// ȷ��ĳ����������arr[i]��ĳһλj 
			j = j / min_scale;
			
			k = record[j];			// ����������arr[i]�����Ӧ��Ͱ�� 
			barrel[j*len+k] = arr[i];
			
			record[j] ++ ;  		// ��¼��ӦͰ��Ԫ������+1 
		}
				
		j = 0;						// ��¼�㸴λ 
		
/*
		for(i=0;i<8;i++)			// ��Ͱ˳�򲻶� 
		{
			while(record[j] ==0)	// ����ĸ�Ͱ�ǿ� 
			{
				j++;
			}
			
			k = record[j];			// ��Ͱ��ȡ������ԭ���Ĵ��������� 
			arr[i] = barrel[j*len+k-1];
			
			record[j] -- ; 			// ��¼��ӦͰ��Ԫ������-1
		}
*/

		for(i=0;i<len;)					// ������λ 
		{
			for(;record[j] ==0;j++) ;	// ����ĸ�Ͱ�ǿ�
			
			for(k=0;k<record[j];k++)	// ��Ͱ��ȡ�������λ
			{				
				arr[i] = barrel[j*len+k];
				i++;
			}
			record[j] =0 ;				// ȡ��һ��Ͱ����ռ�¼
		}
	
		min_scale = mid_scale;
	}
}


int main()
{
	int i;
// ��ʾ����ǰ������ 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");

// ����	
	Radix_Sort(arr,record,barrel,8,1000);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
