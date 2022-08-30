#define Row 4 
#define Col 4 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>

void menu_2048();                                                
void Init_2048(int arr[Row][Col],int row,int col,char f);        
void Display_2048(int arr[Row][Col],int row,int col);            
void NumMoveup_2048(int arr[Row][Col],int row,int col,int ud);    
void NumMovedown_2048(int arr[Row][Col],int row,int col,int ud);  
void NumMoveleft_2048(int arr[Row][Col],int row,int col,int lr); 
void NumMoveright_2048(int arr[Row][Col],int row,int col,int lr);
void PlayerGo_2048(int arr[Row][Col],int row,int col);            
char IsWin_2048(int arr[Row][Col],int row,int col);               
void ComputerGo_2048(int arr[Row][Col],int row,int col);         


int main() 
{
    int show[Row][Col];
    Init_2048(show,Row,Col,0);
    char fin='0';
    while (1)
    {
        if(fin='0')
            ComputerGo_2048(show,Row,Col);
        fin=IsWin_2048(show,Row,Col);
        if(fin!='0') break;
        PlayerGo_2048(show,Row,Col);
        fin=IsWin_2048(show,Row,Col);
        if(fin!='0')break;
    }
    if(IsWin_2048(show,Row,Col)!='N')
    {
        system("cls");
        system("color F2");
        Display_2048(show,Row,Col);
        printf("\n\n\n¹GAME SUCCESS\n");
    }
    else 
    printf("\n\n\nSORRY YOU LOSE\n");
    system("pause");
    exit(0);
}


void menu_2048() 
{
    printf("\n\n\t****************************\n");
    printf("\t****   1.start             *****\n");
    printf("\t****   0.exit              *****\n");
}


void Init_2048(int arr[Row][Col],int rows,int cols,char f) 
{
    memset(arr,f,rows*cols*16);
}


void Display_2048(int arr[Row][Col],int row,int col) 
{
	system("cls");
    int i=0;
    int j=0;
    for(i=0;i<row;i++)
    {
        printf(" ______");
    }
    for(i=0;i<row;i++)
    {
        printf("\n|");
        for(j=0;j<col;j++)
        {
            if(arr[i][j]==0)
                printf("_    _|");
            else
                printf("_%4d_|",arr[i][j]);
        }
    }
}


void NumMoveup_2048(int arr[Row][Col],int row,int col,int ud) 
{
	for(int k=0;k<3;k++)
	{
    	for(int i=1;i<row;i++)
    	{
       		for(int j=0;j<col;j++)
        	{
            	if(arr[i-ud][j]==0)  
            	{
                	arr[i-ud][j]=arr[i][j];
                	arr[i][j]=0;
            	}
            	else if(arr[i-ud][j]==arr[i][j]) 
            	{
                	arr[i-ud][j]=2*arr[i][j];
                	arr[i][j]=0;
            	}
        	}
    	}
	}
}


void NumMovedown_2048(int arr[Row][Col],int row,int col,int ud) 
{
	for(int k=0;k<3;k++)
	{
		for(int i=row-2;i>=0;i--)
    	{
        	for(int j=0;j<col;j++)
        	{
            	if(arr[i+ud][j]==0) 
            	{
                	arr[i+ud][j]=arr[i][j];
                	arr[i][j]=0;
            	}
            	else if(arr[i+ud][j]==arr[i][j])
            	{
                	arr[i+ud][j]=2*arr[i][j]; 
                	arr[i][j]=0;
            	}
        	}
    	}
    }
}


void NumMoveleft_2048(int arr[Row][Col],int row,int col,int lr) 
{
    for(int k=0;k<3;k++)
    {
    	for(int j=1;j<col;j++)
    	{
        	for(int i=0;i<row;i++)
        	{
            	if(arr[i][j-lr]==0) 
            	{
                	arr[i][j-lr]=arr[i][j];
                	arr[i][j]=0;
            	}
            	else if(arr[i][j-lr]==arr[i][j]) 
            	{
                	arr[i][j-lr]=2*arr[i][j];
                	arr[i][j]=0;
            	}
        	}
    	}
    }
}


void NumMoveright_2048(int arr[Row][Col], int row,int col,int lr)  
{
    for(int k=0;k<3;k++)
    {
    	for(int j=col-2;j>=0;j--)
    	{
        	for(int i=0;i<row;i++)
        	{
            	if(arr[i][j+lr]==0)  
            	{
                	arr[i][j+lr]=arr[i][j];
                	arr[i][j]=0;
            	}
            	else if(arr[i][j+lr]==arr[i][j]) 
            	{
                	arr[i][j+lr]=2*arr[i][j];
                	arr[i][j]=0;
            	}
        	}
    	}
	}
}


void  PlayerGo_2048(int arr[Row][Col],int row,int col) 
{
    int  n=0;
    char a=0;
    while (1)
    {
        scanf("%c",&a);
        if     (a=='A'||a=='a') n=3;
        else if(a=='S'||a=='s') n=2;
        else if(a=='d'||a=='D') n=4;
        else if(a=='w'||a=='W') n=1;
        switch (n)
        {
        	case 1: NumMoveup_2048(arr,row,col,1);    break;
        	case 2: NumMovedown_2048(arr,row,col,1);  break;
        	case 3: NumMoveleft_2048(arr,row,col,1);  break;
        	case 4: NumMoveright_2048(arr,row,col,1); break;
        	default: n = 0;                      break;
        }
        if(n!=0) break;
    }
}


char IsWin_2048(int arr[Row][Col],int row,int col)
{
    int i=0;int j=0;
    int count=0;
    for(i=0;i<row;i++)
        for(j=0;j<col;j++)
        {
            if(arr[i][j]==2048)
                return 'Y';
            else if(arr[i][j]== 0)
                count++;
        }
    if(count == 0) return 'N';
    return '0';
}


void ComputerGo_2048(int arr[Row][Col],int row,int col)
{
    int r=0;
    int c=0;
    while (1)
    {
        r=rand()%row;
        c=rand()%col;
        if(arr[r][c]==0)
        {
            arr[r][c]=2; 
            Display_2048(arr,row,col);
            break;
        }
    }
}
