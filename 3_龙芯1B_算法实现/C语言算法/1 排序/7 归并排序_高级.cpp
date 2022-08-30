#include"stdio.h"
#include"stdlib.h"

// int arr[8] = {3,8,1,5,2,4,6,7};
int arr[9] = {9,5,2,7,12,4,3,1,11};

// �ϲ� left �� right ��Ԫ�� 
void merge(int *arr,int *temparr,int left,int mid,int right)
{
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

void msort(int *arr,int *temparr,int left,int right)
{
	// ֻ��һ��Ԫ�ؾͲ���Ҫ����
	// ֻ��һ��Ԫ�ص��������������ģ�ֻ��Ҫ���鲢
	if(left<right)	// ˵������������Ԫ��
	{
		int mid = (left+right)/2;		// ���м�� 
		msort(arr,temparr,left,mid);	// �ݹ黮������� 
		msort(arr,temparr,mid+1,right);	// �ݹ黮���Ұ���
		
		merge(arr,temparr,left,mid,right);	// �ϲ��Ѿ�����Ĳ��� 
	} 
}

void Merge_Sort(int *arr, int len)
{
	int *temparr = (int *)malloc(len*sizeof(int));	// ���丨������ 
	if(temparr)	// ���temparr��Ϊ�գ������������ɹ�
	{
		msort(arr,temparr,0,len-1);	// ���������±�
		free(temparr);				// �ͷŸ������� 
	} 
	else		// �������ʧ�� 
	{
		printf("error: failed to allocate memory"); 
	}
}


int main()
{
	int i;
// ��ʾ����ǰ������ 
	for(i=0;i<=8;i++)
		printf("%d ",arr[i]);
	printf("\n");

// ����	
	Merge_Sort(arr,9);

// ��ʾ���������� 
	for(i=0;i<=8;i++)
		printf("%d ",arr[i]);
	printf("\n");
} 
