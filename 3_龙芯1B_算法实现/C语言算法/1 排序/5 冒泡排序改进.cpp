#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

/*  �����������+ð�ݣ��൱��ð�ݵĸĽ������ǲ�������  */ 

void Bubble_Sort_Pro(int *arr, int begin, int end)
{
	// ����У�� begin<=end
    if(begin > end)
        return;
    
    int change;
    int i,j;
	for(i=begin;arr[i+1]>=arr[i];i++) ;	// Ѱ���������� 
	
	for(j=i+1;j<=end;j++)				// ���������ݱ�����end 
	{
		for(i=j-1;i>=begin;i--)			// ĳһ���������� 
		{
			if(arr[i]>arr[i+1])			// ����������ð�� 
			{
				change = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = change;
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
	Bubble_Sort_Pro(arr,0,7);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
