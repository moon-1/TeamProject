#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define MILLION 1000000L
#define BUFF_SIZE 5000

char PickRanNum(char *Card);
int PrintCardArray(char CardArray[][11]);
void SpreadCard(char CardArray[][11], char *Card);
void MoveC(char CardArray[][11], int row, char col, int Srow, char Scol);
void DeleteL(char CardArray[][11],char *Card);
void MenuPrint();
void SortRank();
void ShowRank();
void ShowHow();
void ClearGame();
void QuitGame();
void CleanScreen();
void CallRank();
void RankSave();

int ran = 60;
char PlayerScore[20] ={0};
char Pscore[10][20] = {0};
int RealPscore[10] = {0};
char name[10][20] = {0};


int main()
{
	int i,count, SpreadCount = 3, MoveCount = 0;
	int j = 2;
	int row,Srow,EndNum,Command, score;
	char col,Scol;
	char Card[60];/*카드 준비*/
	char CardArray[21][11] = {0};/*출력할 카드배열*/
	char PName[20];
	int timedif;
	struct timeval tpend;
	struct timeval tpstart;
	char buff[BUFF_SIZE];
	int fd;	
	int num;
        int rank[1000];
        int n,f;
        
	CallRank();
	for(i = 0 ; i < 6 ; i++)
	{
		Card[i] = 1;
	}
	for(i = 6 ; i < 60 ; i++)
	{
		Card[i] = j;
		count++;
		if(count == 6){
		j++;
		count = 0;
		}
	}	
	/*출력할 카드배열*/
	CardArray[0][0] = '*';
	char column = 'A';
	for(i = 1 ; i < 11 ; i++)/*가로 좌표*/ 
	{
		CardArray[0][i] = column++;
	}
	count = 1;
	for(i = 1 ; i < 21 ; i++)/*세로 좌표*/
		CardArray[i][0] = (char)count++;
	for(i = 1 ; i < 3 ; i++)/*비공개 카드*/
	{
		for(j = 1 ; j < 11 ; j++)
		{
			CardArray[i][j] = '*';
		}
	}
	for(i = 1 ; i < 11 ; i++)/*랜덤으로공개하여 뿌려놓은 카드*/
	{
		CardArray[3][i] = PickRanNum(Card);
	}
        MenuPrint();
	scanf("%d",&num);
        while(1)
        {
                switch(num)
                {
                        
                        case 1: 
        			if (gettimeofday(&tpstart , NULL))
        			{
                			fprintf(stderr,"Failed to get end time\n");
                			return 1;
        			}

				while(1)
				{
					EndNum = PrintCardArray(CardArray);
					if(EndNum==0)
						goto B;
					else if(EndNum == 1) /*카드가 남은 경우*/
					{
						if(SpreadCount != 0)
						{
							printf("\n 명령을 슥 입력하세요:\n\n 1 : 카드를 슥 옮깁니다. \n\n 2 : 카드패를 슥 뿌립니다. 남은 횟수 (%d/3)\n",SpreadCount);
                					scanf("%d",&Command);
						}
						else if(SpreadCount == 0)
						{
							Command = 1;
							printf("\n 카드만 슥  옮길 수 있습니다\n");
						}

						if(Command == 1)
						{
							printf("옮기고 싶은 카드의 행,열을 슥 입력하세요(두개 이상이면 가장 위에 위치한 카드) : ");
       				 			scanf("%d %c", &row, &col);

        						printf("옮기려고 하는 위치의 행,열을 슥 입력하세요 : ");
        						scanf("%d %c", &Srow, &Scol);
							MoveC(CardArray, row, col,Srow,Scol);
							MoveCount++;
               				 		DeleteL(CardArray,Card);
						}
						else if(Command == 2 && SpreadCount !=-1)
						{
                        				SpreadCard(CardArray, Card);
							SpreadCount--;
						}
						else			
							printf("명령어를 다시 슥 입력해주세요.");		
					}	
				}
                                break;
                        case 2: 
                                ShowRank(rank,n);
                                scanf("%d",&num);
                                continue;
                        case 3: 
                                ShowHow();
                                scanf("%d",&num);
                                continue;
                        case 4: 
                                QuitGame();
                                return 0;
                        default:
                                printf("\n다시 입력하세요:\n");
                                scanf("%d",&num);
                }
        }
B:                                              
	getchar(); 
	if (gettimeofday(&tpend , NULL))
        {
        	fprintf(stderr,"Failed to get end time\n");
                return 1;
        }
        timedif = (MILLION*(tpend.tv_sec - tpstart.tv_sec)+tpend.tv_usec - tpstart.tv_usec)/100000;
        score = 10000-timedif-(MoveCount*10);
	ClearGame();
        printf("\n점수는 %d점입니다.\n\n\n랭킹에 등록할 이름을 입력해주세요(영어 대문자 이니셜 3글자)\n\n",score);
        fgets(PName,sizeof(PName),stdin);
        if (score>RealPscore[9])
        {
 	       RealPscore[9] = score;
               for(int u=0;u<20;u++)
               {
	               name[9][u] = PName[u];
               }
	}
	SortRank();
	RankSave();
	return 0;
}

int PrintCardArray(char CardArray[][11])
{
        char Command[3];
        int i,j,k,EndNum=0;
        printf("\n\n");
	for(k = 0 ; k < 11 ; k++)
	{
		printf("%3c",CardArray[0][k]);
	}
	printf("\n");
        for(i = 1 ; i < 21 ; i++)
        {
                for(j = 0 ; j < 11 ; j++)
                {
			char a = CardArray[i][j];
			if(a != 0)
			{
				if(a == '*')
					printf("%3c",a);
				else
					printf("%3d",a);
			}
			else
			{
				printf("   ");
			}
                }
                printf("\n");
        }
	for(i = 1 ; i < 11 ; i++)
	{
		if(CardArray[1][i] != 0)
			EndNum = 1;
	}
	return EndNum;
}

char PickRanNum(char *Card)
{       
        int ret;        //ranindex값 임시저장
	int ranindex;
        while(1)
        {
                srand((int)time(NULL)); //난수생성
                ranindex = rand()%ran;       //배열 인덱스 값을 0~60 사이로 난수로 설정
                if(Card[ranindex] == 0)
			continue;
		ret = Card[ranindex];
		Card[ranindex] = Card[ran-1];
                Card[ran-1] = 0;  
		ran--;    
                break;
        }
        return ret;
}

void SpreadCard(char CardArray[][11], char *Card)
{
	for(int i=1; i<11; i++)
	{
       		for(int j=1; j<21; j++)
        	{
                	if(CardArray[j][i] == 0)
                	{
                        	CardArray[j][i] = PickRanNum(Card);   /*랜덤함수*/
                        	break;
                	}
        	}
	}
}

void MoveC(char CardArray[][11], int row, char col, int Srow, char Scol)
{
        int p=0;
        int i,j=0,k=0;
        int AScol,Acol,count;

        AScol=Scol-64;
        Acol=col-64;

        for(i=1;i<21;i++)
        {
                if(CardArray[i][Acol]!=0)
                k++;
        }

	count = k-row+1;

        for(i=row;i<row+count-1;i++)
        {
                if(CardArray[i+1][Acol]!=CardArray[i][Acol]-1)
                j=1;
        }

        if(CardArray[Srow][AScol]!=0)
	        p=1;
        else if(j==1)
	        p=1;
        else if(CardArray[row][Acol]!=CardArray[Srow-1][AScol]-1&&Srow!=1)
        {
                p=1;
        }
        if(p!=1)
        {
                while(1)
                {
                        CardArray[Srow][AScol]=CardArray[row][Acol];
                        CardArray[row][Acol]=0;
                        Srow++;
                        row++;

                        if(CardArray[row][Acol]==0)
                        break;
                }
        }
        else if(p==1)
        printf("못갑니다");

}

void DeleteL(char CardArray[][11], char *Card)
{
	int i,j,k=0,l,m;
	int num=10;
	
	for(j=0;j<11;j++) //1행 1열 ~ 1행 10열
	{
		for(i=0;i<21;i++)  //1행 1열 ~ 26행 1열
		{
			num = 10;
			m = i;
			while(1)
			{
				if(CardArray[m][j]==num) //10봤을때 10~1연결된 열이면 삭제
				{
					num--;
					m++;
					if(num==0)
					for(l=m-1;l>=m-10;l--)
						CardArray[l][j] = 0;
				}
				else
				break;
			}
			if(CardArray[m][j] == '*')
                        {       
                                if(CardArray[m+1][j] == 0)
					CardArray[m][j] = PickRanNum(Card);
			}
                                        

		}
	}
}
void MenuPrint()
{
        char buff[5000];
        int fd;
        if( 0<(fd = open("./MenuPrint.txt",O_RDONLY)))
        {
                read(fd,buff,1024);
                puts(buff);
                close(fd);
        }

}
void SortRank()
{
        int i,j,k;
        int tmp;
        char tmpname[20];

        for(i=0;i<9;i++)
        {
                for(j=0; j<9; j++)
                {
                        if(RealPscore[j] < RealPscore[j+1])
                        {
                                tmp = RealPscore[j];
                                RealPscore[j] = RealPscore[j+1];
                                RealPscore[j+1] = tmp;
                                for(k=0;k<20;k++)
                                {
                                        tmpname[k] = name[j][k];
                                        name[j][k] = name[j+1][k];
                                        name[j+1][k] = tmpname[k];
                                } 
                        }
                }
        }
}

void ShowRank()
{
        int i;

        printf(" 		──────────────Ranking─────────────\n");
        SortRank();
	for(i=0; i<10; i++)
                printf("\n	 	 [%2d위] :  %4d점, (이름) :%s ", i+1, RealPscore[i], name[i]);
        printf("\n		──────────────────────────────────\n");
        printf("\n\n");
}


void ShowHow()
{
        int fd;
        char buff[10000];
            if( 0 < (fd = open("./GameManual.txt",O_RDONLY)));
        {
                read(fd,buff,10000);
                puts(buff);
                close(fd);
        }
}
void ClearGame()
{               
        int fd; 
        char buff[10000];
            if( 0 < (fd = open("./EndGame.txt",O_RDONLY)));
        {                       
                read(fd,buff,10000);
                puts(buff);     
                close(fd);      
        }                       
}
void QuitGame()
{
        int fd;
        char buff[10000];
            if( 0 < (fd = open("./QuitGame.txt",O_RDONLY)));
        {
                read(fd,buff,10000);
                puts(buff);
                close(fd);
        }
}
                 

void CleanScreen()
{
        system("clear");
}

void CallRank()
{
	int fd,Pnum;
        if( 0< (fd = open("./score1.txt",O_RDWR)))
        {
                read(fd,Pscore[0],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score2.txt",O_RDWR)))
        {
                read(fd,Pscore[1],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score3.txt",O_RDWR)))
        {
                read(fd,Pscore[2],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score4.txt",O_RDWR)))
        {
                read(fd,Pscore[3],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score5.txt",O_RDWR)))
        {
                read(fd,Pscore[4],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score6.txt",O_RDWR)))
        {
                read(fd,Pscore[5],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score7.txt",O_RDWR)))
        {
		read(fd,Pscore[6],BUFF_SIZE);
		write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score8.txt",O_RDWR)))
        {
                read(fd,Pscore[7],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score9.txt",O_RDWR)))
        {
                read(fd,Pscore[8],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./score10.txt",O_RDWR)))
        {
                read(fd,Pscore[9],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
	for(int i = 0 ; i<10;i++)
	{	
		int sum = 0;
		for(int j = 0; j<20;j++)
		{
			Pnum = Pscore[i][j];
			Pscore[i][j] = 0;
			if(isdigit(Pnum))
				sum = sum*10+Pnum-'0';
			else
			{
				RealPscore[i] = sum;
				break;
			}	
		}
	}
        if( 0< (fd = open("./name1.txt",O_RDWR)))
        {
                read(fd,name[0],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name2.txt",O_RDWR)))
        {
                read(fd,name[1],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name3.txt",O_RDWR)))
        {
                read(fd,name[2],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name4.txt",O_RDWR)))
        {
                read(fd,name[3],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name5.txt",O_RDWR)))
        {
                read(fd,name[4],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name6.txt",O_RDWR)))
        {
                read(fd,name[5],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name7.txt",O_RDWR)))
        {
                read(fd,name[6],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name8.txt",O_RDWR)))
        {
                read(fd,name[7],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name9.txt",O_RDWR)))
        {
                read(fd,name[8],BUFF_SIZE);
                write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
        if( 0< (fd = open("./name10.txt",O_RDWR)))
        {
                read(fd,name[9],BUFF_SIZE);
		write(fd,NULL,BUFF_SIZE);
                close(fd);
        }
}

void RankSave()
{
	int fd,RPS;
	for(int i= 0;i<10;i++)
	{
		RPS = RealPscore[i];
		sprintf(Pscore[i], "%d", RPS); 
	}		
	if(0 < (fd = open("./score1.txt",O_WRONLY)))
        {
		write(fd,Pscore[0],strlen(Pscore[0]));
		close(fd);
        }
        if(0 < (fd = open("./score2.txt",O_WRONLY)))
        {
                write(fd,Pscore[1],strlen(Pscore[1]));
                close(fd);
        }
        if(0 < (fd = open("./score3.txt",O_WRONLY)))
        {
                write(fd,Pscore[2],strlen(Pscore[2]));
                close(fd);
        }
        if(0 < (fd = open("./score4.txt",O_WRONLY)))
        {
                write(fd,Pscore[3],strlen(Pscore[3]));
                close(fd);
        }
        if(0 < (fd = open("./score5.txt",O_WRONLY)))
        {
                write(fd,Pscore[4],strlen(Pscore[4]));
                close(fd);
        }
        if(0 < (fd = open("./score6.txt",O_WRONLY)))
        {
                write(fd,Pscore[5],strlen(Pscore[5]));
                close(fd);
        }
        if(0 < (fd = open("./score7.txt",O_WRONLY)))
        {
                write(fd,Pscore[6],strlen(Pscore[6]));
                close(fd);
        }
        if(0 < (fd = open("./score8.txt",O_WRONLY)))
        {
                write(fd,Pscore[7],strlen(Pscore[7]));
                close(fd);
        }
        if(0 < (fd = open("./score9.txt",O_WRONLY)))
        {
                write(fd,Pscore[8],strlen(Pscore[8]));
                close(fd);
        }
        if(0 < (fd = open("./score10.txt",O_WRONLY)))
        {
                write(fd,Pscore[9],strlen(Pscore[9]));
                close(fd);
        }

        if(0 < (fd = open("./name1.txt",O_WRONLY)))
        {
                write(fd,name[0],strlen(name[0]));
                close(fd);
        }
        if(0 < (fd = open("./name2.txt",O_WRONLY)))
        {
                write(fd,name[1],strlen(name[1]));
                close(fd);
        }
        if(0 < (fd = open("./name3.txt",O_WRONLY)))
        {
                write(fd,name[2],strlen(name[2]));
                close(fd);
        }
        if(0 < (fd = open("./name4.txt",O_WRONLY)))
        {
                write(fd,name[3],strlen(name[3]));
                close(fd);
        }
        if(0 < (fd = open("./name5.txt",O_WRONLY)))
        {
                write(fd,name[4],strlen(name[4]));
                close(fd);
        }
        if(0 < (fd = open("./name6.txt",O_WRONLY)))
        {
                write(fd,name[5],strlen(name[5]));
                close(fd);
        }
        if(0 < (fd = open("./name7.txt",O_WRONLY)))
        {
                write(fd,name[6],strlen(name[6]));
                close(fd);
        }
        if(0 < (fd = open("./name8.txt",O_WRONLY)))
        {
                write(fd,name[7],strlen(name[7]));
                close(fd);
        }
        if(0 < (fd = open("./name9.txt",O_WRONLY)))
        {
                write(fd,name[8],strlen(name[8]));
                close(fd);
        }
        if(0 < (fd = open("./name10.txt",O_WRONLY)))
        {
                write(fd,name[9],strlen(name[9]));
                close(fd);
        }
}
