#include"stdio.h"
#define length  8 

// 汇编搭配，半字=两字节=16位 
// 为了方便向汇编移植， barrel没用二维数组 

//int arr[length] = {3,8,1,5,2,4,6,7};			// 待排序数组，i，arr[i]对应的某一位是j 
int arr[length] = {33,58,999,0,233,54,133,7};	// 待排序数组，i，arr[i]对应的某一位是j
int record[10] = {0};							// 桶内元素个数记录，j，record[j]是k 
int barrel[length*10] = {0};					// 桶，k，barrel[j*len+k]是桶中的元素 

void Radix_Sort(int *arr, int *record, int *barrel, int len, int max_scale)
{
	int i,j,k;
	int min_scale = 1; 
	while(min_scale<max_scale)		// 从低位向高位递进 
	{
		int mid_scale = min_scale * 10;
		for(i=0;i<len;i++)			// 遍历待排序数组 
		{
			j = arr[i] % mid_scale;	// 确定某个待排序数arr[i]的某一位j 
			j = j / min_scale;
			
			k = record[j];			// 将待排序数arr[i]放入对应的桶中 
			barrel[j*len+k] = arr[i];
			
			record[j] ++ ;  		// 记录对应桶中元素数量+1 
		}
				
		j = 0;						// 记录点复位 
		
/*
		for(i=0;i<8;i++)			// 出桶顺序不对 
		{
			while(record[j] ==0)	// 检测哪个桶非空 
			{
				j++;
			}
			
			k = record[j];			// 从桶中取出放入原来的待排序数组 
			arr[i] = barrel[j*len+k-1];
			
			record[j] -- ; 			// 记录对应桶中元素数量-1
		}
*/

		for(i=0;i<len;)					// 遍历空位 
		{
			for(;record[j] ==0;j++) ;	// 检测哪个桶非空
			
			for(k=0;k<record[j];k++)	// 从桶中取出放入空位
			{				
				arr[i] = barrel[j*len+k];
				i++;
			}
			record[j] =0 ;				// 取完一个桶则清空记录
		}
	
		min_scale = mid_scale;
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
	Radix_Sort(arr,record,barrel,8,1000);

// 显示排序后的序列 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
