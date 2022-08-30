#include"stdio.h"

#define len 7

/* ������ */
// size��ȥ���Ѿ�����õĽڵ�ʣ�µĽڵ��� 
// root�Ǹ��ڵ����� 
void adjust_heap(int *arr,int root,int size)
{
    int l_leaf = 2 * root + 1;	// ��Ҷ�ӽڵ�����	
    int r_leaf = 2 * root + 2;	// ��Ҷ�ӽڵ�����
    int max = root;				// ���ֵ���� ����ʼ��Ϊ���ڵ� 
    int change;					// ������������ 
    if(l_leaf < size && arr[l_leaf] > arr[max])	// ���������Ҷ�ӽڵ㣬������Ҷ�ӽڵ��֮ǰ�����ֵ�����ڵ㣩�� 
    	max = l_leaf;							// ���ֵ����ָ����Ҷ�ӽڵ� 
    if(r_leaf < size && arr[r_leaf] > arr[max])	// ���������Ҷ�ӽڵ㣬������Ҷ�ӽڵ��֮ǰ�����ֵ�����ڵ����ڵ㣩�� 
    	max = r_leaf;							// ���ֵ����ָ����Ҷ�ӽڵ� 
    if(max!=root)					// ������ֵ���Ǹ��ڵ� 
    {
		change=arr[max];			// �����ڵ������ֵ�ڵ㽻�� 
		arr[max]=arr[root];
		arr[root]=change;
		root = max;					// ��֮ǰ���ֵ����ָ��Ľڵ�Ϊ���ڵ���е���
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

// ��ʾ����ǰ������ 
	show(arr,len);

// ��ʼ���������
	int size,root_1,change;
	size = len;
	for(root_1=len/2-1;root_1>=0;root_1--)// ��������� 
	{
		adjust_heap(arr,root_1,size);	// ������������������ɵĽڵ㣬����ʣ�೤����ԶΪlen 
	}
	
// ��ʾ����ѹ������ 
	show(arr,len);
	
// ��ʼ�������� 
	root_1 = 0;
	for(size=len-1;size>=0;size--)
	{
		change=arr[0];				// �����׺Ͷ�β�Ե���δ����̶�i��һ 
		arr[0]=arr[size];
		arr[size]=change;
		adjust_heap(arr,root_1,size);	// ��ʼ���������ѣ��������֮���Ǵ�0�ڵ㿪ʼ 
	}

// ��ʾ���������� 
	show(arr,len);
} 
