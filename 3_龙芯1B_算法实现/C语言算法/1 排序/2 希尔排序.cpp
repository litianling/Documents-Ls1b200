#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Shell_Sort(int *arr, int len)
{
	/*��ʼ����������*/
	int increment = len;
	int temp;
    int i,j;
    
	while (increment > 1)	/*ÿ�μ�С������ֱ��increment = 1*/
	{	
		increment = increment/3 + 1;	/*�����½���֮һ����������ȡ��+1*/
		
		/*��ÿ�����������ֺ���߼����飬����ֱ�Ӳ�������*/
		for (j = increment; j < len; j++)
		{
			if (arr[j-increment] > arr[j])	// ���ifȥ����Ӱ���������ϼ򻯼��� 
			{
				temp = arr[j];
				
				for(i = j-increment;i >= 0 && arr[i] > temp;i = i- increment)	// ���������ͺ��� 
				{
					arr[i+increment] = arr[i];
				}
				arr[i+increment] = temp;	// ����Ԫ�� 
			}
		}
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
	Shell_Sort(arr,8);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
