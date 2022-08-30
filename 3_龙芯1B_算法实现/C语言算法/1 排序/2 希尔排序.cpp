#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Shell_Sort(int *arr, int len)
{
	/*初始化划分增量*/
	int increment = len;
	int temp;
    int i,j;
    
	while (increment > 1)	/*每次减小增量，直到increment = 1*/
	{	
		increment = increment/3 + 1;	/*增量下降法之一：除三向下取整+1*/
		
		/*对每个按增量划分后的逻辑分组，进行直接插入排序*/
		for (j = increment; j < len; j++)
		{
			if (arr[j-increment] > arr[j])	// 这个if去掉不影响结果，加上简化计算 
			{
				temp = arr[j];
				
				for(i = j-increment;i >= 0 && arr[i] > temp;i = i- increment)	// 满足条件就后移 
				{
					arr[i+increment] = arr[i];
				}
				arr[i+increment] = temp;	// 插入元素 
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
	Shell_Sort(arr,8);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
