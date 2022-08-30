#include"stdio.h"
#include"stdlib.h"

#define len 9 

// 合并 left 到 right 的元素 
void merge(int *arr,int *temparr,int left,int mid,int right)
{
	// 标记左半区第一个未排序元素
	int l_pose = left; 
	// 标记右半区第一个未排序元素
	int r_pose = mid +1;
	// 临时数组元素的下标
	int pose = left;
	// 合并 左右半区都有剩余元素 
	while(l_pose<=mid && r_pose<=right)
	{
		if (arr[l_pose]<arr[r_pose])	//  左半区第一个未排序元素 小于 右半区第一个未排序元素 
		{
			temparr[pose++] =  arr[l_pose++];	// 将 左半区第一个未排序元素 放到临时数组 
		}
		else							//  右半区第一个未排序元素 小于或等于 左半区第一个未排序元素
		{
			temparr[pose++] =  arr[r_pose++];	// 将 右半区第一个未排序元素 放到临时数组
		}
	}
	// 合并 左半区剩余元素
	while(l_pose<=mid)
	{
		temparr[pose++] =  arr[l_pose++];	// 将 左半区未排序元素 不断塞入临时数组 
	}
	// 合并 右半区剩余元素 
	while(r_pose<=right)
	{
		temparr[pose++] =  arr[r_pose++];	// 将 右半区未排序元素 不断塞入临时数组 
	}
	// 把临时数组中合并后元素复制回原来数组
	while(left<=right)
	{
		arr[left]=temparr[left];
		left++;
	} 
}

void Merge_Sort(int *arr,int *temparr,int left,int right)
{
	// 只有一个元素就不需要划分
	// 只有一个元素的区域本身就是有序的，只需要被归并
	if(left<right)	// 说明有两个以上元素
	{
		int mid = (left+right)/2;				// 找中间点 
		Merge_Sort(arr,temparr,left,mid);		// 递归划分左半区 
		Merge_Sort(arr,temparr,mid+1,right);	// 递归划分右半区
		
		merge(arr,temparr,left,mid,right);		// 合并已经排序的部分 
	} 
}


int main()
{

	// int arr[8] = {3,8,1,5,2,4,6,7};
	int arr[len] = {9,5,2,7,12,4,3,1,11};	
	int temparr[len]; 

	int i;
// 显示排序前的序列 
	for(i=0;i<=8;i++)
		printf("%d ",arr[i]);
	printf("\n");

// 排序	
	Merge_Sort(arr,temparr,0,len-1);

// 显示排序后的序列 
	for(i=0;i<=8;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
