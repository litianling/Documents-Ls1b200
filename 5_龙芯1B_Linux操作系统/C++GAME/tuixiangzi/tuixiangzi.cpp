#include<stdio.h>
#include<string.h>
#include<setjmp.h> 


jmp_buf env1,env2; 
char a[50][50];
char passward[15];
char ch,GK,p='1';   
int  Player_Coordinates_Y,Player_Coordinates_X;     
int  Box1_Destination_Y,Box1_Destination_X;   
int  Box2_Destination_Y,Box2_Destination_X;    
int  Box3_Destination_Y,Box3_Destination_X;   
int  Box4_Destination_Y,Box4_Destination_X;   

void level_pushbox(char k);  
void input_pushbox(char ch); 
void supplement_pushbox();   
void success_pushbox();      
void output_pushbox();       
      

int main()
{	
	setjmp(env1); 
	printf("Please choose level(1-5): \n"); 
	setjmp(env2); 
	scanf("%c",&GK);
	if ((GK<'0'||GK>'5')&&(GK!='.')) 
	    {
	    	printf("\n choose level fail,please again: \n");
		    longjmp(env2,1); 
		}
	else if(GK>p) 
	    {
	    	printf("\n fail,please choose level again: \n");
		    longjmp(env2,2); 
		}  
    while((GK>='0'&&GK<'6')||GK=='.')
    {
	level_pushbox(GK); 
        output_pushbox(); 
	    while(1)  
	    {
		    scanf("%c",&ch);
		    input_pushbox(ch);    
		    supplement_pushbox(); 
		    output_pushbox();
		    if(a[Box1_Destination_Y][Box1_Destination_X]=='@'&&a[Box2_Destination_Y][Box2_Destination_X]=='@'  
		     &&a[Box3_Destination_Y][Box3_Destination_X]=='@'&&a[Box4_Destination_Y][Box4_Destination_X]=='@')
		    {
		    	GK++;
		    	if(GK>=p) p=GK;
                break;  
		    }
	    }
    }
    	success_pushbox();
}


void level_pushbox(char GK)  
{
	switch(GK)
	{
		case '1':
			{
				strcpy(a[0],"   ###                                           ");
				strcpy(a[1],"   #*#                                           ");
				strcpy(a[2],"   # #                                           ");
				strcpy(a[3],"####O######                                      ");
				strcpy(a[4],"#*  OM O *#                                      ");
				strcpy(a[5],"#####O#####                                      ");
				strcpy(a[6],"    # #                                          ");
				strcpy(a[7],"    #*#                                          ");
				strcpy(a[8],"    ###                                          ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"level 1/5                                       ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"Press R start again                             ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=4,Player_Coordinates_X=5;         
	            Box1_Destination_Y=1,Box1_Destination_X=4;
	            Box2_Destination_Y=4,Box2_Destination_X=1;
	            Box3_Destination_Y=4,Box3_Destination_X=9;
	            Box4_Destination_Y=7,Box4_Destination_X=5;
			}
		break;
		case '2':
			{
				strcpy(a[0],"######                                           ");
				strcpy(a[1],"#*   #                                           ");
				strcpy(a[2],"###  #                                           ");
				strcpy(a[3],"#  O ######                                      ");
				strcpy(a[4],"#*  OM O *#                                      ");
				strcpy(a[5],"#####O#####                                      ");
				strcpy(a[6],"    # #                                          ");
				strcpy(a[7],"    #*#                                          ");
				strcpy(a[8],"    ###                                          ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"level 2/5                                       ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"Press R start again                             ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=4,Player_Coordinates_X=5;         
	            Box1_Destination_Y=1,Box1_Destination_X=1;
	            Box2_Destination_Y=4,Box2_Destination_X=1;
	            Box3_Destination_Y=4,Box3_Destination_X=9;
	            Box4_Destination_Y=7,Box4_Destination_X=5;
			}
		break;
		case '3':
			{
				strcpy(a[0],"  ####                                           ");
				strcpy(a[1],"  #  #                                           ");
				strcpy(a[2],"  #  #                                           ");
				strcpy(a[3],"  #M #                                           ");
				strcpy(a[4],"### ######                                       ");
				strcpy(a[5],"#   O  O*#                                       ");
				strcpy(a[6],"# O*   ###                                       ");
				strcpy(a[7],"#####* O*#                                       ");
				strcpy(a[8],"    ######                                       ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"level 3/5                                       ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"Press R start again                             ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=3,Player_Coordinates_X=3;         
	            Box1_Destination_Y=6,Box1_Destination_X=3;
	            Box2_Destination_Y=5,Box2_Destination_X=8;
	            Box3_Destination_Y=7,Box3_Destination_X=5;
	            Box4_Destination_Y=7,Box4_Destination_X=8;
			}
		break;
		case '4':
			{
				strcpy(a[0]," ########                                        ");
				strcpy(a[1]," #     ###                                       ");
				strcpy(a[2],"##O###   #                                       ");
				strcpy(a[3],"#M  O  O #                                       ");
				strcpy(a[4],"# **# O ##                                       ");
				strcpy(a[5],"##**#   #                                        ");
				strcpy(a[6]," ########                                        ");
				strcpy(a[7],"                                                 ");
				strcpy(a[8],"                                                 ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"level 4/5                                       ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"Press R start again                             ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=3,Player_Coordinates_X=1;         
	            Box1_Destination_Y=4,Box1_Destination_X=2;
	            Box2_Destination_Y=4,Box2_Destination_X=3;
	            Box3_Destination_Y=5,Box3_Destination_X=2;
	            Box4_Destination_Y=5,Box4_Destination_X=3;
			}
		break;
		case '5':
			{
				strcpy(a[0],"  ####                                           ");
				strcpy(a[1],"  #**#                                           ");
				strcpy(a[2]," ## *##                                          ");
				strcpy(a[3]," #  O*#                                          ");
				strcpy(a[4],"## O  ##                                         ");
				strcpy(a[5],"#  #OO #                                         ");
				strcpy(a[6],"#  M   #                                         ");
				strcpy(a[7],"########                                         ");
				strcpy(a[8],"                                                 ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"level 5/5                                       ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"Press R start again                             ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=6,Player_Coordinates_X=3;         
	            Box1_Destination_Y=1,Box1_Destination_X=3;
	            Box2_Destination_Y=1,Box2_Destination_X=4;
	            Box3_Destination_Y=2,Box3_Destination_X=4;
	            Box4_Destination_Y=3,Box4_Destination_X=5;
			}
		break;	
		case '0':
			{
				printf("\n Input password for ANSWER: \n");
				scanf("%s",passward);
                if(passward[0]=='L'&&passward[1]=='T'&&passward[2]=='L'&&passward[3]=='1'&&passward[4]=='9'&&passward[5]=='9'   
				 &&passward[6]=='9'&&passward[7]=='1'&&passward[8]=='0'&&passward[9]=='1'&&passward[10]=='5')//&&passward[11]==NULL  
				{                                                                     
		    		strcpy(a[0],"                    ANSWER                       ");  
	    			strcpy(a[1],"level 1/5 :                                      ");  
		    		strcpy(a[2],"  SS WW DDD AAAAAA DD WW                         ");  
    				strcpy(a[3],"level 2/5 :                                      "); 
	    			strcpy(a[4],"  SS WW DDD AAAAAA DWW DWAA                      "); 
		    		strcpy(a[5],"level 3/5 :                                      ");
			    	strcpy(a[6],"  SSSDDDWDA SSDAWWAASAW AASDDDWDS AAWWWD WWASSSS ");
    				strcpy(a[7],"level 4/5 :                                      ");
	    			strcpy(a[8],"  DDDD SSDDWWA WWAAAA SSASDWDS WAWWDDDD SSAAADDD ");
		    		strcpy(a[9],"  WWAAAA SSASD WDDDDD SASAW DWAAA DDDWW AAAA SSS ");
			    	strcpy(a[10],"  WDDDDDD WAS AAAAA WWDDDD SDS AAAA DDDWW AAAASS");
    				strcpy(a[11],"level 5/5 :                                     ");
	    			strcpy(a[12],"  AWW DDWW SSAASS DDWWW SSSDD WASAAA WWWDDD SAAA");
		    		strcpy(a[13],"  SSDD WW DWA SAWW SSASS DDDWW                  ");
			    	strcpy(a[14],"Press R start again                             ");
			    }
			    else 
				{
					printf("\n Password fail,please choose level again: \n");
					longjmp(env2,1);
				} 
				 
			}
		break;
		case '.':
			{
				printf("\n Please input password: \n");
				scanf("%s",passward);
                if(passward[0]=='L'&&passward[1]=='T'&&passward[2]=='L'&&passward[3]=='1'&&passward[4]=='9'&&passward[5]=='9' 
				 &&passward[6]=='9'&&passward[7]=='1'&&passward[8]=='0'&&passward[9]=='1'&&passward[10]=='5')//&&passward[11]==NULL  
				{
					p='5'; 
		    		printf("\n Password right,please choose level again: \n");
		    		longjmp(env2,2);
			    }
			    else 
				{
					printf("\n Password fail,please choose level again: \n");
					longjmp(env2,3);
				} 
				 
			}
		break;
	}
} 


void input_pushbox(char ch)    
{
	switch(ch)  
	{
	    case 'w':
		    if(a[Player_Coordinates_Y-1][Player_Coordinates_X]!='#')              
		    {
			    if(a[Player_Coordinates_Y-1][Player_Coordinates_X]!='O'&&a[Player_Coordinates_Y-1][Player_Coordinates_X]!='@')     
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';   
				   Player_Coordinates_Y--;                             
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';   
			    }
			    else if(a[Player_Coordinates_Y-2][Player_Coordinates_X]!='#'&&a[Player_Coordinates_Y-2][Player_Coordinates_X]!='@'&&a[Player_Coordinates_Y-2][Player_Coordinates_X]!='O')
			    {                                                                    
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';  
				   Player_Coordinates_Y--;                             
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';   
				   a[Player_Coordinates_Y-1][Player_Coordinates_X]='O'; 
				   if(((Player_Coordinates_Y-1==Box1_Destination_Y)&&(Player_Coordinates_X==Box1_Destination_X))
				    ||((Player_Coordinates_Y-1==Box2_Destination_Y)&&(Player_Coordinates_X==Box2_Destination_X))
				    ||((Player_Coordinates_Y-1==Box3_Destination_Y)&&(Player_Coordinates_X==Box3_Destination_X))
					||((Player_Coordinates_Y-1==Box4_Destination_Y)&&(Player_Coordinates_X==Box4_Destination_X)))
				       a[Player_Coordinates_Y-1][Player_Coordinates_X]='@';
			    }
		    }
		break;   
		case 'a':
		    if(a[Player_Coordinates_Y][Player_Coordinates_X-1]!='#')
		    {
			    if(a[Player_Coordinates_Y][Player_Coordinates_X-1]!='O'&&a[Player_Coordinates_Y][Player_Coordinates_X-1]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X--;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y][Player_Coordinates_X-2]!='#'&&a[Player_Coordinates_Y][Player_Coordinates_X-2]!='@'&&a[Player_Coordinates_Y][Player_Coordinates_X-2]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X--;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y][Player_Coordinates_X-1]='O';
			       if(((Player_Coordinates_Y==Box1_Destination_Y)&&(Player_Coordinates_X-1==Box1_Destination_X))
				    ||((Player_Coordinates_Y==Box2_Destination_Y)&&(Player_Coordinates_X-1==Box2_Destination_X))
				    ||((Player_Coordinates_Y==Box3_Destination_Y)&&(Player_Coordinates_X-1==Box3_Destination_X))
					||((Player_Coordinates_Y==Box4_Destination_Y)&&(Player_Coordinates_X-1==Box4_Destination_X)))
				       a[Player_Coordinates_Y][Player_Coordinates_X-1]='@';
			    }
		    }
		break;
		case 's':
		    if(a[Player_Coordinates_Y+1][Player_Coordinates_X]!='#')
		    {
			    if(a[Player_Coordinates_Y+1][Player_Coordinates_X]!='O'&&a[Player_Coordinates_Y+1][Player_Coordinates_X]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_Y++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y+2][Player_Coordinates_X]!='#'&&a[Player_Coordinates_Y+2][Player_Coordinates_X]!='@'&&a[Player_Coordinates_Y+2][Player_Coordinates_X]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_Y++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y+1][Player_Coordinates_X]='O';
				   if(((Player_Coordinates_Y+1==Box1_Destination_Y)&&(Player_Coordinates_X==Box1_Destination_X))
				    ||((Player_Coordinates_Y+1==Box2_Destination_Y)&&(Player_Coordinates_X==Box2_Destination_X))
				    ||((Player_Coordinates_Y+1==Box3_Destination_Y)&&(Player_Coordinates_X==Box3_Destination_X))
					||((Player_Coordinates_Y+1==Box4_Destination_Y)&&(Player_Coordinates_X==Box4_Destination_X)))
				       a[Player_Coordinates_Y+1][Player_Coordinates_X]='@';
			    }
			}
		break;
	    case 'd':
		    if(a[Player_Coordinates_Y][Player_Coordinates_X+1]!='#')
		    {
			    if(a[Player_Coordinates_Y][Player_Coordinates_X+1]!='O'&&a[Player_Coordinates_Y][Player_Coordinates_X+1]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y][Player_Coordinates_X+2]!='#'&&a[Player_Coordinates_Y][Player_Coordinates_X+2]!='@'&&a[Player_Coordinates_Y][Player_Coordinates_X+2]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y][Player_Coordinates_X+1]='O';
				   if(((Player_Coordinates_Y==Box1_Destination_Y)&&(Player_Coordinates_X+1==Box1_Destination_X))
				    ||((Player_Coordinates_Y==Box2_Destination_Y)&&(Player_Coordinates_X+1==Box2_Destination_X))
				    ||((Player_Coordinates_Y==Box3_Destination_Y)&&(Player_Coordinates_X+1==Box3_Destination_X))
					||((Player_Coordinates_Y==Box4_Destination_Y)&&(Player_Coordinates_X+1==Box4_Destination_X)))
				       a[Player_Coordinates_Y][Player_Coordinates_X+1]='@';					  
			    }
		    }
		break;
		case 'r':longjmp(env1,1); 
		break;                   
		default: ;  
		break;
	}
} 


void supplement_pushbox() 
{
	if(a[Box1_Destination_Y][Box1_Destination_X]==' ')  
	    a[Box1_Destination_Y][Box1_Destination_X]='*'; 
	if(a[Box2_Destination_Y][Box2_Destination_X]==' ')  
	    a[Box2_Destination_Y][Box2_Destination_X]='*';
	if(a[Box3_Destination_Y][Box3_Destination_X]==' ')  
	    a[Box3_Destination_Y][Box3_Destination_X]='*';
	if(a[Box4_Destination_Y][Box4_Destination_X]==' ')  
	    a[Box4_Destination_Y][Box4_Destination_X]='*';
	output_pushbox();
}


void success_pushbox()
{
    printf("\n\nGAME SUCCESS\n");
    printf("\n The password is : LTL19991015 \n");
    for (float y = 1.3f; y > -1.0f; y -= 0.1f) 
    {
        for (float x = -1.5f; x < 1.5f; x += 0.05f) 
		{
            float a = x * x + y * y - 1;
            putchar(a * a * a - x * x * y * y * y <= 0.0f ? '*' : ' ');
        }
	printf("\n");
    }
} 


void output_pushbox()  
{
	for(int i=0;i<15;i++) 
        {
	    for(int j=0;j<50;j++)
                printf("%c",a[i][j]);  
            printf("\n");
        }
}

