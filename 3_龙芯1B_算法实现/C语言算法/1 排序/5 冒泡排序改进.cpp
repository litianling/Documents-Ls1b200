#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

/*  检测有序数组+冒泡，相当于冒泡的改进，不是插入排序  */ 

void Bubble_Sort_Pro(int *arr, int begin, int end)
{
	// 参数校验 begin<=end
    if(begin > end)
        return;
    
    int change;
    int i,j;
	for(i=begin;arr[i+1]>=arr[i];i++) ;	// 寻找有序数组 
	
	for(j=i+1;j<=end;j++)				// 待插入数据遍历到end 
	{
		for(i=j-1;i>=begin;i--)			// 某一待插入数据 
		{
			if(arr[i]>arr[i+1])			// 满足条件就冒泡 
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
// 显示排序前的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");

// 排序	
	Bubble_Sort_Pro(arr,0,7);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
