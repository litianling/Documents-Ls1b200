#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Quick_Sort(int *arr, int begin, int end)
{
	// 参数校验 begin<=end ****************** 重点 ：也是递归调用的结束条件 
    if(begin > end)
        return;
    
    // 初始化 选定基准数，初始化哨兵 
    int tmp = arr[begin];
    int i = begin;
    int j = end;
    
    // 开始排序 
	while(i != j)						// 一轮遍历结束条件：两哨兵相遇 
	{
        while(arr[j] >= tmp && j > i)	// 哨兵j先移动，找到小于基准数的数值 
		    j--;
        while(arr[i] <= tmp && j > i)	// 哨兵i再移动，找到大于基准数的数值 
		    i++;
            
        if(j > i)						// 如果两哨兵没相遇，交换两个哨兵找到的值 
		{
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    
    // 一轮循环结束，交换基准数与哨兵相遇时的值 
    arr[begin] = arr[i];
    arr[i] = tmp;
    

    Quick_Sort(arr, begin, i-1);	// 前段 ——递归调用 
    Quick_Sort(arr, i+1, end);		// 后段 ——递归调用
}


int main()
{
	int i;
// 显示排序前的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");

// 排序	
	Quick_Sort(arr,0,7);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
