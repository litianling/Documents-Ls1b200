#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Bubble_Sort(int *arr, int begin, int end)
{
	// ����У�� begin<=end
    if(begin > end)
        return;
    
    int change;
    
    for(;begin<end;end--)		// ���� = ��end������begin+1ʱ���� 
    {
    	// ��ʼ�� ѡ����׼������ʼ���ڱ� 
    	int i = begin+1;
    	
    	for(;i<=end;i++)		// �� = ��i������endʱ���� 
    	{
    		if(arr[i]<arr[i-1])	// ���arr[i]<arr[i-1]������arr[i]��arr[i-1] 
    		{
    			change = arr[i];
    			arr[i] = arr[i-1];
    			arr[i-1] = change;
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
	Bubble_Sort(arr,0,7);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
