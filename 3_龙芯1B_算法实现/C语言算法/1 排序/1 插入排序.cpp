#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Insertion_Sort(int *arr, int begin, int end)
{
	// ����У�� begin<=end
    if(begin > end)
        return;
    
    int temp;
    int i,j;
	for(i=begin;arr[i+1]>=arr[i];i++) ;	// Ѱ���������� 
	
	for(j=i+1;j<=end;j++)				// ���������ݱ�����end
	{
		temp = arr[j];					// ĳһ����������
		
		for(i=j-1;i>=begin && arr[i]>temp;i--) 			// ���������ͺ��� 
		{
			arr[i+1] = arr[i];
		}
		arr[i+1] = temp;								// ����Ԫ�� 
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
	Insertion_Sort(arr,0,7);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
