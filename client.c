#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include "login.h"
#include "transmit.h"

int main(int argc, char * argv[])
{
	struct sockaddr_in serv;
	int sd;
	char buf[80];
	sd = socket(AF_INET,SOCK_STREAM,0);

	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv.sin_port=htons(5555);

	connect(sd,(void *)(&serv),sizeof(serv));

	int type = client_login(sd);
	if(type==1)
	{
		printf("Press 1 to add\n");
		printf("Press 2 to remove\n");
		printf("Press 3 to modify\n");
		printf("Press 4 to logout\n");
		printf("Press 5 to save\n");
		int flag=0;
		while(!flag)
		{
			char buf[10]={};
			int ch;
			scanf("%d",&ch);
			buf[0]=(ch+'0');
			wr(sd,buf);
			char rcv[80]={};
			switch(ch)
			{
				case 1:
				{
					char user[80]={},pass[80]={};
					int x=PRESENT;
					// printf("%d\n",DUPLICATE);
					while(x==PRESENT)
					{
						printf("Enter a username and password\n");
						scanf("%s",user);
						scanf("%s",pass);
						wr(sd,user);
						// sleep(1);
						wr(sd,pass);
						rd(sd,rcv);
						x=(rcv[0]-'0');
						// printf("x = %d\n",x);
					}
					memset(rcv,'\0',sizeof(rcv));
					rd(sd,rcv);
					printf("%s",rcv);
					// printf("OPtion 1\n");
					break;
				}
				case 2:
				{
					char user[80]={};
					int x=ABSENT;
					while(x==(ABSENT))
					{
						printf("Enter the user account which you want to delete\n");
						scanf("%s",user);
						wr(sd,user);
						rd(sd,rcv);
						x=(rcv[0]-'0');
						// printf("x = %d\n",x);
					}
					// printf("Outside loop\n");
					memset(rcv,'\0',sizeof(rcv));
					rd(sd,rcv);
					printf("%s",rcv);
					break;
				}
				case 3:
				{
					char user[80],new_user[80];
					int x=ABSENT;
					while(x==(ABSENT))
					{
						printf("Enter the user account which you wish to modify\n");
						scanf("%s",user);
						wr(sd,user);
						rd(sd,rcv);
						x=(rcv[0]-'0');
						// printf("x = %d\n",x);
					}
					printf("Enter the name of the new user\n");
					scanf("%s",new_user);
					wr(sd,new_user);
					memset(rcv,'\0',sizeof(rcv));
					rd(sd,rcv);
					printf("%s",rcv);
					break;
				}
				case 4:
				{
					rd(sd,rcv);
					// printf("OPtion 4\n");
					// rd(sd);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					flag=1;
					break;
				}
				case 5:
				{
					rd(sd,rcv);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					break;
				}
			}
		}
	}
	else
	{
		printf("Press 1 to deposit\n");
		printf("Press 2 to withdraw\n");
		printf("Press 3 for balance enquiry\n");
		printf("Press 4 to view details\n");
		printf("Press 5 to change password\n");
		printf("Press 6 to exit\n");
		int flag=0;
		while(!flag)
		{
			char buf[10];
			int ch;
			scanf("%d",&ch);
			buf[0]=(ch+'0');
			wr(sd,buf);
			char rcv[80]={},rcv1[80]={};
			switch(ch)
			{
				case 1:
				{
					printf("Enter the amount of money you want to deposit\n");
					char snd[80]={},c;
					scanf("%s",snd);
					wr(sd,snd);
					// while((getchar())!='\n');

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// c=getchar();
					// write(sd,&c,1);

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// while((getchar())!='\n');
					// c=getchar();
					// write(sd,&c,1);

					rd(sd,rcv);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					break;
				}
				case 2:
				{
					printf("Enter the amount of money you want to withdraw\n");
					char snd[80]={},c;
					scanf("%s",snd);
					wr(sd,snd);
					// while((getchar())!='\n');

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// c=getchar();
					// write(sd,&c,1);

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// while((getchar())!='\n');
					// c=getchar();
					// write(sd,&c,1);

					rd(sd,rcv);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					break;
				}
				case 3:
				{
					char c;
					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// while((getchar())!='\n');

					// rd(sd,rcv);
					// printf("%s",rcv);
					// memset(rcv,'\0',sizeof(rcv));
					// c=getchar();
					// write(sd,&c,1);

					rd(sd,rcv);
					printf("%s\n",rcv);
					memset(rcv,'\0',sizeof(rcv));
					break;
				}
				case 4:
				{
					rd(sd,rcv);
					rd(sd,rcv1);
					printf("%s\n",rcv);
					printf("%s\n",rcv1);
					memset(rcv,'\0',sizeof(rcv));
					memset(rcv,'\0',sizeof(rcv1));
					break;
				}
				case 5:
				{
					char p1[80],p2[80];
					int flag =0;
					while(!flag)
					{
						printf("Enter your new password\n");
						scanf("%s",p1);
						printf("Enter your new password again\n");
						scanf("%s",p2);
						int ret;
						ret=strcmp(p1,p2);
						if(ret==0)
						{
							flag=1;
						}
						else
						{
							printf("Password mismatch. Enter again\n");
						}
					}
					// printf("Outside loop\n");
					wr(sd,p1);
					rd(sd,rcv);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					break;
				}
				case 6:
				{
					rd(sd,rcv);
					printf("%s",rcv);
					memset(rcv,'\0',sizeof(rcv));
					flag=1;
					break;
				}
			}
		}
	}
}
