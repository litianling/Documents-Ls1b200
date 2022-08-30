#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Select_Sort(int *arr, int begin, int end)
{
	// 参数校验 begin<=end
    if(begin > end)
        return;
    
    int change;
    
    for(;begin<end;begin++)		// 不带 = ，begin遍历至倒数第二个数时结束 
    {
    	// 初始化 选定基准数，初始化哨兵 
    	int i = begin;
    	int j = begin;
    	
    	for(;i<=end;i++)		// 带 = ，i遍历至最后一个数时结束 
    	{
    		if(arr[i]<arr[j])	// 如果arr[i]<arr[j]，将arr[i]标记为最小值 
    		{
    			j = i;
			}
		}
		change = arr[j];		// 一轮遍历结束，一个最小值归位 
		arr[j] = arr[begin];
		arr[begin] = change;
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
	Select_Sort(arr,0,7);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
