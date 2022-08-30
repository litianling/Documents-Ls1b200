#include"stdio.h"
#include"stdlib.h"

#define len 9 
int leb_start = 0;
int leb_end = 2;

// 合并 left 到 right 的元素 
void Merge_Sort_2(int *arr,int *temparr,int left,int right)
{
	int mid = (left+right)/2;
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

void Merge_Sort_1(int *lebal,int left,int right)
{
	int mid = (left+right)/2;				// 找中间点 
	if(left-mid)							// left不等于mid才保存 
	{
		lebal[leb_end] = left;
		leb_end++;
		lebal[leb_end] = mid;
		leb_end++;
	}
	if(mid+1-right)							// mid+1不等于right才保存 
	{
		lebal[leb_end] = mid+1;
		leb_end++;
		lebal[leb_end] = right;
		leb_end++;
	}
}

void show(int *arr,int _long)
{
	int i; 
	for(i=0;i<_long;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 

int main()
{

	// int arr[8] = {3,8,1,5,2,4,6,7};
	int arr[len] = {9,5,2,7,12,4,3,1,11};	
	int lebal[len*2];
	int temparr[len]; 
	int left,right;

	lebal[0] = 0;
	lebal[1] = len-1 ;

// 显示排序前的序列 
	show(arr,len);

// 划分 
	for(;lebal[leb_start+1]-lebal[leb_start]>1;)
	{
		left = lebal[leb_start];
		leb_start ++ ;
		right = lebal[leb_start];
		leb_start ++ ;
		Merge_Sort_1(lebal,left,right);
	}
	show(lebal,leb_end);

// 合并 
	for(;leb_end>0;)
	{
		leb_end--;
		right = lebal[leb_end];
		leb_end--;
		left = lebal[leb_end];
		Merge_Sort_2(arr,temparr,left,right);
	}
	
// 显示排序后的序列 
	show(arr,len);
} 
