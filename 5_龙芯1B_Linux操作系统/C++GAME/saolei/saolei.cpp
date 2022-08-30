#include<ctime>      
#include<cstdlib>  
#include<iostream>  
#include<cstring>   
#include<stdio.h>

#define setcolor(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x)

using namespace std;

int number;  
int map[12][12]; 
int derection[3]={-1,0,1};
int x,y;
char ch;

void initia_saolei();                
int  calculate_saolei(int x,int y); 
void gameover_saolei();             
void game_saolei(int x,int y);      
void print_saolei();                
bool check_saolei();                

int main () 
{
	srand(time(0));  
	    do
	    {
	    	printf("\n Please Choose Hard(1-81):");
		    cin>>number;
		}while(number<1||number>81); 
		initia_saolei();             
		print_saolei();              
		cout<<"\n";
		cout<<"X Y :";
		while(cin>>x>>y)  
		{
			if(map[x][y]==9) 
			{
				gameover_saolei();
				break;    
			} 
			game_saolei(x,y);  
			print_saolei();    
			cout<<"\nX Y :";
			if(check_saolei())  
			{
				cout<<"\n\nGAME SUCCESS"<<endl;
				break;
			}
		//	cout<<"\n";
		}	
	return 0;
} 


void initia_saolei()  
{
    memset(map,-1,sizeof(map));  
	for(int i=0;i<number;)        
	{                            
		x=rand()%9+1;      
		y=rand()%9+1;      
		if ( map[x][y]!=9 )   
		{
			map[x][y]=9;
			i++;
		}
	}
}


int calculate_saolei(int x,int y) 
{
	int counter=0;
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			if (map[x+derection[i]][y+derection[j]]==9)
				counter++;   
	return counter;
}


void gameover_saolei()   
{
	cout<<"SORRY YOU LOSE"<<endl;
	for(int i=1;i<10;i++)
	{
		for(int j=1;j<10;j++ )
		{
			if(map[i][j]==9 )
				cout<<"@ ";
			else 
				cout<<"O ";
		}
		cout<<endl;
	}	
} 


void game_saolei(int x,int y) 
{
    if(calculate_saolei(x,y)==0)
	{
		map[x][y]=0;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
				if(x+derection[i]<=9&&y+derection[j]<=9&&x+derection[i]>=1&&y+derection[j]>=1&&!(derection[i]==0&&derection[j]==0)&&map[x+derection[i]][y+derection[j]]==-1) 
					game_saolei( x+derection[i], y+derection[j] ); 
		}                 
	}
	else
		map[x][y] = calculate_saolei(x,y); 
}


void print_saolei() 
{
	for(int i=1;i<10;i++)
	{
		for(int j=1;j<10;j++)
		{
			if(map[i][j]==-1||map[i][j]==9)
				cout<<"* ";
			else
			{
				if(map[i][j]==0) 
				    cout<<"  ";
				else 
				    cout<<map[i][j]<<" "; 
			}
		}
		cout<<endl;
	}
}


bool check_saolei() 
{
	int counter=0;
	for (int i=1;i<10;i++)
		for (int j=1;j<10;j++)
			if(map[i][j]==-1) 
				counter++;
	if(counter==0)
		return true;
	else
		return false;
}

