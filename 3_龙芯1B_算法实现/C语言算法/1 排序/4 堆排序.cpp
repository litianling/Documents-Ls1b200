#include"stdio.h"

#define len 7

/* 调整堆 */
// size是去掉已经排序好的节点剩下的节点数 
// root是根节点坐标 
void adjust_heap(int *arr,int root,int size)
{
    int l_leaf = 2 * root + 1;	// 左叶子节点坐标	
    int r_leaf = 2 * root + 2;	// 右叶子节点坐标
    int max = root;				// 最大值坐标 ，初始化为根节点 
    int change;					// 交换辅助变量 
    if(l_leaf < size && arr[l_leaf] > arr[max])	// 如果存在左叶子节点，并且左叶子节点比之前的最大值（根节点）大 
    	max = l_leaf;							// 最大值坐标指向左叶子节点 
    if(r_leaf < size && arr[r_leaf] > arr[max])	// 如果存在右叶子节点，并且右叶子节点比之前的最大值（根节点或左节点）大 
    	max = r_leaf;							// 最大值坐标指向右叶子节点 
    if(max!=root)					// 如果最大值不是根节点 
    {
		change=arr[max];			// 将根节点与最大值节点交换 
		arr[max]=arr[root];
		arr[root]=change;
		root = max;					// 以之前最大值坐标指向的节点为根节点进行调整
		adjust_heap(arr,root,size);	
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
	int arr[len] = {3,4,2,8,9,5,1};	

// 显示排序前的序列 
	show(arr,len);

// 开始构建大根堆
	int size,root_1,change;
	size = len;
	for(root_1=len/2-1;root_1>=0;root_1--)// 构建大根堆 
	{
		adjust_heap(arr,root_1,size);	// 构建过程中无排序完成的节点，所以剩余长度永远为len 
	}
	
// 显示大根堆构建结果 
	show(arr,len);
	
// 开始调整排序 
	root_1 = 0;
	for(size=len-1;size>=0;size--)
	{
		change=arr[0];				// 将堆首和堆尾对调，未排序程度i减一 
		arr[0]=arr[size];
		arr[size]=change;
		adjust_heap(arr,root_1,size);	// 开始交换调整堆，构建完毕之后都是从0节点开始 
	}

// 显示排序后的序列 
	show(arr,len);
} 
