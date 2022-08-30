# include <stdio.h>
# include <stdlib.h>
// �����ڵ�ģ��
typedef struct TreeNode
{
    int data;
    struct TreeNode * left;
    struct TreeNode * right;
}SearchTree;

// ����
SearchTree* search(SearchTree * T, int data)
{
    // ���Ϊ�գ�
    if (T == NULL)
    {
        printf("�˶�����Ϊ��");
    }
    // ����Ϊ��ʱ�������������һ�ֱȴ˽ڵ�ֵ�󣬵ڶ��ֱȴ˽ڵ�ֵС��
    else if (T->data > data)
    {
        return search(T->right,data);
    }
    else if (T->data < data)
    {
        return search(T->left, data);
    }
    else
    {
       return T;
    }
}
// ����
SearchTree* insertTree(SearchTree * T, int data)
{
    // ���Ϊ��
    if (T == NULL)
    {
        // ����һ���ڴ�Ȼ���ٹ��ڸ�����
        T = (SearchTree *)malloc(sizeof(SearchTree));
        T->data = data;
        T->right = NULL;
        T->left = NULL;
        printf("%d��������\n",data);
        return T;
    }
    if (T->data > data)
    {
        T->left = insertTree(T->left, data);
    }
    else if (T->data < data)
    {
        T->right = insertTree(T->right, data);
    }
    else
        printf("%d�������У������ٴβ���!!!\n",data);
    return T;
}
// Ѱ��������С�ڵ�
SearchTree* FindMin(SearchTree * T)
{
    if (T == NULL)
    {
        printf("����Ϊ��");
    }
    if (T->left != NULL)
    {
        return FindMin(T->left);
    }
    return T;
}

// Ѱ���������ڵ�

SearchTree* FindMax(SearchTree * T)
{
    if (T == NULL)
    {
        printf("����Ϊ��");
    }
    if (T->right != NULL)
    {
        return FindMax(T->right);  
    }
    return T; //->data;
}
// ɾ��
SearchTree* deleteTree(SearchTree * T, int data)
{
    if (T == NULL)
    {
        return T;
    }
    if (T->data > data)
    {
        T->left = deleteTree(T->left, data);
    }
    else if(T->data < data)
    {
        T->right = deleteTree(T->right, data);
    }
    // �ҵ��ýڵ���
    else if (T->left != NULL && T->right != NULL)   // �ýڵ��Ϊ2
    {
        printf("��ɾ���ڵ�Ϊ%d\n", T->data);
        T->data = FindMin(T->right)->data;
        T->right = deleteTree(T->right,data);
    }
    else
    {
        printf("��ɾ���ڵ�Ϊ%d\n", T->data);
        T = (T->left != NULL)? T->left: T->right;
    }
    return T;
}
//�������:�������������������Եõ�ԭ�ؼ�����������
void print(SearchTree *  root)
{
    if(root!=NULL)
    {
        print(root->left);
        printf("%d ",root->data);
        print(root->right);
    }
 }
int main (void)
{
    int data[7] = {45,24,53,45,12,24,90};
    int i;
    SearchTree *  root = NULL; //������������ڵ�
    for(i=0;i<7;i++)    //�������������
    {
        root = insertTree(root,data[i]);
    }
    print(root);
    printf("\n");
    root = insertTree(root, 55);
    print(root);
    printf("\n");
    root = deleteTree(root,24);
    print(root);
    printf("\n");
    return 0;
}
