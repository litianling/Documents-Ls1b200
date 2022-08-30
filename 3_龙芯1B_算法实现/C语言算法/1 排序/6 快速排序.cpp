#include"stdio.h"

int arr[8] = {3,8,1,5,2,4,6,7};

void Quick_Sort(int *arr, int begin, int end)
{
	// ����У�� begin<=end ****************** �ص� ��Ҳ�ǵݹ���õĽ������� 
    if(begin > end)
        return;
    
    // ��ʼ�� ѡ����׼������ʼ���ڱ� 
    int tmp = arr[begin];
    int i = begin;
    int j = end;
    
    // ��ʼ���� 
	while(i != j)						// һ�ֱ����������������ڱ����� 
	{
        while(arr[j] >= tmp && j > i)	// �ڱ�j���ƶ����ҵ�С�ڻ�׼������ֵ 
		    j--;
        while(arr[i] <= tmp && j > i)	// �ڱ�i���ƶ����ҵ����ڻ�׼������ֵ 
		    i++;
            
        if(j > i)						// ������ڱ�û���������������ڱ��ҵ���ֵ 
		{
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    
    // һ��ѭ��������������׼�����ڱ�����ʱ��ֵ 
    arr[begin] = arr[i];
    arr[i] = tmp;
    

    Quick_Sort(arr, begin, i-1);	// ǰ�� �����ݹ���� 
    Quick_Sort(arr, i+1, end);		// ��� �����ݹ����
}


int main()
{
	int i;
// ��ʾ����ǰ������ 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");

// ����	
	Quick_Sort(arr,0,7);

// ��ʾ���������� 
	for(i=0;i<=7;i++)
		printf("%d ",arr[i]);
	printf("\n");
	
} 
