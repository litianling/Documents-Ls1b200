#include<stdio.h>

#define len 15
int tree[len]={20, 19, 17, 16, 18, 10, 15, 14, 12, 13, 0, 9, 7, 6, 8}; 
int stack[len]={0}; 	// 坐标缓存堆栈 
int print[len]={0};		// 后序输出缓存堆栈 
int stack_label=0;		// 坐标缓存堆栈指针 


/***************************************************
*				层序遍历（非递归） 
***************************************************/
int print_lev_1(int j)
{	
	int i=j;
	for(;i<len;i++)
	{
		if(tree[i]!=0)
			printf("%d ",tree[i]);
	} 
	return 0;
}


/***************************************************
*				先序遍历（递  归） 
*	思想："根左右"遍历法。
*		先输出当前节点；
*		再递归访问左节点；
*		最后递归访问右节点。 
***************************************************/
int print_pre(int j)
{	
	int i=j;
	if(i>=len)
		return 0;
	printf("%d ",tree[i]);	// 先输出当前节点 
	if(tree[2*i+1]!=0)
		print_pre(2*i+1);	// 访问左叶子节点 
	if(tree[2*i+2]!=0)
		print_pre(2*i+2);	// 访问右叶子节点
	return 0;
}


/***************************************************
*				先序遍历（非递归） 
*	思想：由于堆栈的先入后出性质，所以与递归实现相比，
*		  右叶子节点先入栈，左叶子节点后入栈 。 
***************************************************/
int print_pre_1()
{	
	int i;
	while(stack_label>0)				// 缓存区堆栈指针小于等于0结束遍历
	{
		i=stack[--stack_label];			// 加载缓存区中坐标
		if(i>=len)						// 当前节点超出范围，换下一个节点重新遍历
			continue;
		printf("%d ",tree[i]);			// 输出当前节点值 
		if(tree[i*2+2]!=0)				// 右叶子节点非空就入栈 
			stack[stack_label++]=i*2+2;
		if(tree[i*2+1]!=0)				// 左叶子节点非空就入栈
			stack[stack_label++]=i*2+1;
	}
}


/***************************************************
*				中序遍历（递  归） 
*	思想："左根右"遍历法。
*		先递归访问左节点；
*		无左节点时再输出当前节点；
*		最后递归访问右节点。
***************************************************/
int print_mid(int j)
{	
	int i=j;
	if(i>=len)
		return 0;
	if(tree[2*i+1]!=0)
		print_mid(2*i+1);	// 访问左叶子节点 
	printf("%d ",tree[i]); 	// 直到遇到没左叶子节点的节点，就输出当前节点 
	if(tree[2*i+2]!=0)
		print_mid(2*i+2);	// 访问右叶子节点 
	return 0;
}


/***************************************************
*				中序遍历（非递归） 
***************************************************/
int go_left(int i)
{
	while((2*i+1<len) && (tree[2*i+1]!=0)) 	// 查找最左叶子节点 
	{
		stack[stack_label++]=i;				// 不是最左叶子节点的都入栈 
		i = 2*i+1;							// 开始查找下一个 
	}
	return i;
}

int print_mid_1(int i)
{
	i = go_left(i);			// 找传入节点的最左叶子节点 
	while(1)
	{
		printf("%d ",tree[i]); 					// 输出最左叶子结点 
		if((2*i+2<len) && (tree[2*i+2]!=0)) 	// 如果该最左叶子结点存在右叶子节点 
			i = go_left(i*2+2);					// 进入上述右叶子节点，再找该节点的最左侧节点 
		else if(stack_label!=0)					// 否则无右叶子节点时，并且堆栈不为空时 
			i = stack[--stack_label];			// 从堆栈中出栈一个元素进行访问
		else
			return 0;
	}
}


/***************************************************
*				后序遍历（递  归） 
*	思想："左右根"遍历法。
*		先递归访问左节点；
*		再递归访问右节点；
*		最后左右节点均没有时再输出当前节点。
***************************************************/
int print_aft(int j)
{	
	int i=j;
	if(i>=len)
		return 0;
	if(tree[2*i+1]!=0)
		print_aft(2*i+1);	// 访问左叶子节点 
	if(tree[2*i+2]!=0)
		print_aft(2*i+2);	// 访问右叶子节点 
	printf("%d ",tree[i]);	// 左右叶子节点都没有才输出当前节点 
	return 0;
}


/***************************************************
*				后序遍历（非递归） 
*	思想：后序即为"左右根",和先序"根左右"相比可知。
*		  先序改为先入栈左叶子节点，遍历为"根右左"， 
*		  再将整体反转一下变为"左右根"，即为后序。 
***************************************************/
int print_aft_1()
{	
	int i;
	int print_label=len-1;				// 输出缓存堆栈的指针初始化 
	while(stack_label>0)				// 缓存区堆栈指针小于等于0结束遍历
	{
		i=stack[--stack_label];			// 加载缓存区中坐标
		if(i>=len)						// 当前节点超出范围，换下一个节点重新遍历
			continue;
		print[print_label--]=tree[i];	// 将当前节点放入缓存数组（反序） 
		if(tree[i*2+1]!=0)				// 左叶子节点非空就入栈 
			stack[stack_label++]=i*2+1;
		if(tree[i*2+2]!=0)				// 右叶子节点非空就入栈 
			stack[stack_label++]=i*2+2;
	}
	for(print_label=0;print_label<len;print_label++)
	{
		if(print[print_label]!=0)		// 顺序输出非空节点 
			printf("%d ",print[print_label]);
	}
}



/***************************************************
*
*						主程序 
*
//*************************************************/
int main()
{
	printf("层序遍历（非递归）："); 
	print_lev_1(0);
	printf("\n \n");

	printf("先序遍历（递  归）："); 
	print_pre(0);
	printf("\n");
	
	printf("先序遍历（非递归）："); 
	stack_label=0;			// 堆栈指针初始化 
	stack[stack_label++]=0;	// 先将根节点坐标入栈 
	print_pre_1();
	printf("\n \n");
	
	printf("中序遍历（递  归）："); 
	print_mid(0);			// 堆栈指针初始化 
	printf("\n");

	printf("中序遍历（非递归）："); 
	stack_label=0;
	print_mid_1(0);
	printf("\n \n");

	printf("后序遍历（递  归）："); 
	print_aft(0);
	printf("\n");

	printf("后序遍历（非递归）："); 
	stack_label=0;			// 堆栈指针初始化 
	stack[stack_label++]=0;	// 先将根节点坐标入栈 
	print_aft_1();
	printf("\n \n");
	
	return 0;
}
