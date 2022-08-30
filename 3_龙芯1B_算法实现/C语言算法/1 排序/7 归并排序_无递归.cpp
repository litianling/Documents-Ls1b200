#include"stdio.h"
#include"stdlib.h"

#define len 9 
int leb_start = 0;
int leb_end = 2;

// �ϲ� left �� right ��Ԫ�� 
void Merge_Sort_2(int *arr,int *temparr,int left,int right)
{
	int mid = (left+right)/2;
	// ����������һ��δ����Ԫ��
	int l_pose = left; 
	// ����Ұ�����һ��δ����Ԫ��
	int r_pose = mid +1;
	// ��ʱ����Ԫ�ص��±�
	int pose = left;
	// �ϲ� ���Ұ�������ʣ��Ԫ�� 
	while(l_pose<=mid && r_pose<=right)
	{
		if (arr[l_pose]<arr[r_pose])	//  �������һ��δ����Ԫ�� С�� �Ұ�����һ��δ����Ԫ�� 
		{
			temparr[pose++] =  arr[l_pose++];	// �� �������һ��δ����Ԫ�� �ŵ���ʱ���� 
		}
		else							//  �Ұ�����һ��δ����Ԫ�� С�ڻ���� �������һ��δ����Ԫ��
		{
			temparr[pose++] =  arr[r_pose++];	// �� �Ұ�����һ��δ����Ԫ�� �ŵ���ʱ����
		}
	}
	// �ϲ� �����ʣ��Ԫ��
	while(l_pose<=mid)
	{
		temparr[pose++] =  arr[l_pose++];	// �� �����δ����Ԫ�� ����������ʱ���� 
	}
	// �ϲ� �Ұ���ʣ��Ԫ�� 
	while(r_pose<=right)
	{
		temparr[pose++] =  arr[r_pose++];	// �� �Ұ���δ����Ԫ�� ����������ʱ���� 
	}
	// ����ʱ�����кϲ���Ԫ�ظ��ƻ�ԭ������
	while(left<=right)
	{
		arr[left]=temparr[left];
		left++;
	} 
}

void Merge_Sort_1(int *lebal,int left,int right)
{
	int mid = (left+right)/2;				// ���м�� 
	if(left-mid)							// left������mid�ű��� 
	{
		lebal[leb_end] = left;
		leb_end++;
		lebal[leb_end] = mid;
		leb_end++;
	}
	if(mid+1-right)							// mid+1������right�ű��� 
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

// ��ʾ����ǰ������ 
	show(arr,len);

// ���� 
	for(;lebal[leb_start+1]-lebal[leb_start]>1;)
	{
		left = lebal[leb_start];
		leb_start ++ ;
		right = lebal[leb_start];
		leb_start ++ ;
		Merge_Sort_1(lebal,left,right);
	}
	show(lebal,leb_end);

// �ϲ� 
	for(;leb_end>0;)
	{
		leb_end--;
		right = lebal[leb_end];
		leb_end--;
		left = lebal[leb_end];
		Merge_Sort_2(arr,temparr,left,right);
	}
	
// ��ʾ���������� 
	show(arr,len);
} 
