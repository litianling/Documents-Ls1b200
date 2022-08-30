#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Bubble_Sort(int *arr, int begin, int end)
{
	// 参数校验 begin<=end
    if(begin > end)
        return;
    
    int change;
    
    for(;begin<end;end--)		// 不带 = ，end遍历至begin+1时结束 
    {
    	// 初始化 选定基准数，初始化哨兵 
    	int i = begin+1;
    	
    	for(;i<=end;i++)		// 带 = ，i遍历至end时结束 
    	{
    		if(arr[i]<arr[i-1])	// 如果arr[i]<arr[i-1]，交换arr[i]与arr[i-1] 
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
// 显示排序前的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");

// 排序	
	Bubble_Sort(arr,0,7);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
