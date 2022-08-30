#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Insertion_Sort(int *arr, int begin, int end)
{
	// 参数校验 begin<=end
    if(begin > end)
        return;
    
    int temp;
    int i,j;
	for(i=begin;arr[i+1]>=arr[i];i++) ;	// 寻找有序数组 
	
	for(j=i+1;j<=end;j++)				// 待插入数据遍历到end
	{
		temp = arr[j];					// 某一待插入数据
		
		for(i=j-1;i>=begin && arr[i]>temp;i--) 			// 满足条件就后移 
		{
			arr[i+1] = arr[i];
		}
		arr[i+1] = temp;								// 插入元素 
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
	Insertion_Sort(arr,0,7);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
