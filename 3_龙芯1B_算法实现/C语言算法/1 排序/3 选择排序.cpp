#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Select_Sort(int *arr, int begin, int end)
{
	// ����У�� begin<=end
    if(begin > end)
        return;
    
    int change;
    
    for(;begin<end;begin++)		// ���� = ��begin�����������ڶ�����ʱ���� 
    {
    	// ��ʼ�� ѡ����׼������ʼ���ڱ� 
    	int i = begin;
    	int j = begin;
    	
    	for(;i<=end;i++)		// �� = ��i���������һ����ʱ���� 
    	{
    		if(arr[i]<arr[j])	// ���arr[i]<arr[j]����arr[i]���Ϊ��Сֵ 
    		{
    			j = i;
			}
		}
		change = arr[j];		// һ�ֱ���������һ����Сֵ��λ 
		arr[j] = arr[begin];
		arr[begin] = change;
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
	Select_Sort(arr,0,7);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
