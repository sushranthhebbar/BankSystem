#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include <sys/file.h>

#include "login.h"
#include "transmit.h"

int main()
{
	struct sockaddr_in serv,cli;
	int sd,sz,nsd,ret,opt;
	char buf[80];
	sd = socket(AF_INET,SOCK_STREAM,0);
	int ret1=setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(ret1<0)
	{
		perror("Error :");
		exit(EXIT_FAILURE);
	}
	// sd = socket(AF_UNIX,SOCK_STREAM,0);
	serv.sin_family=AF_INET;
	// serv.sin_family=AF_UNIX;
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	// serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(5555);

	ret=bind(sd,(void *)(&serv),sizeof(serv));
	if(ret<0)
	{
		// printf("Error in binding and value of ret is: %d\n",ret);
		perror("Error :");
		exit(1);
	}
	printf("Binded socket successfully\n");
	listen(sd,2);
	sz=sizeof(cli);
	pid_t childpid;
	while(1)
	{
		nsd=accept(sd,(void *)(&cli),&sz);
		if(nsd<0)
		{
			printf("Error accepting connection\n");
			exit(1);
		}
		childpid=fork();
		if(childpid==0)
		{
			// printf("Connection accepted\n");
			close(sd);
			char id[80];
			int type=server_login(nsd,id);
			if(type==1)
			{
				int no_of_accounts=load();
				// printf("Press 1 to add\n");
				// printf("Press 2 to remove\n");
				// printf("Press 3 to exit\n");
				int flag=0;
				while(!flag)
				{
					char buf[10];
					int ch;
					rd(nsd,buf);
					ch=(buf[0]-'0');
					switch(ch)
					{
						// char snd[80];
						case 1:
						{
							/*char user[80],pass[80];
							rd(nsd,user,sizeof(user));
							rd(nsd,pass,sizeof(pass));
							printf("User = %s,Pass = %s\n",user,pass);
							write(nsd,"2",1);*/
							char user[80],pass[80];
							int res= PRESENT;
							// printf("Before loop\n");
							while(res==PRESENT)
							{
								rd(nsd,user);
								rd(nsd,pass);
								int idx;
								idx=present(user,no_of_accounts);
								if(idx<0 || chk[idx]==1) res=ABSENT;
								// printf("%d\n",res);
								char snd[5];
								snd[0]=(res+'0');
								// printf("%c\n",snd[0]);
								wr(nsd,snd);
							}
							// printf("Outside loop\n");
							// printf("no of accounts = %d\n",no_of_accounts);
							// save(no_of_accounts);
							// memset(snd,'\0',sizeof(snd));
							no_of_accounts=add_account(user,pass,no_of_accounts);
							char snd[]={"Account has been added successfully\n"};
							wr(nsd,snd);
							break;
						}
						case 2:
						{
							char user[80];
							int res=ABSENT,idx;
							// printf("no of accounts = %d\n",no_of_accounts);
							while(res==ABSENT)
							{
								rd(nsd,user);
								idx=present(user,no_of_accounts);
								// printf("user = %s\n",user);
								if(idx>=0 && chk[idx]==0) res=PRESENT;
								// printf("%d\n",idx);
								char snd[5];
								snd[0]=(res+'0');
								wr(nsd,snd);
							}
							// printf("Outside loop\n");
							remove_account(user,idx);
							char snd[]={"Account has been removed successfully\n"};
							wr(nsd,snd);
							break;
						}
						case 3:
						{
							int res=ABSENT,idx;
							while(res==ABSENT)
							{
								char user[80];
								rd(nsd,user);
								idx=present(user,no_of_accounts);
								if(idx>=0) res=PRESENT;
								char snd1[5];
								snd1[0]=res+'0';
								wr(nsd,snd1);
							}
							char new_user[80];
							rd(nsd,new_user);
							char path[80]={"user_data/"},path_name[80]={"user_data/"};
							strcat(path_name,accounts[idx].user);
							strcat(path_name,"/settings.txt");
							int fd,i=0,n;
							fd=open(path_name,O_RDONLY);
							char c,user1[80],pass[80];
							char *x=user1;
							while(n=read(fd,&c,1))
							{
								if(c=='\n')
								{
									x=pass;
									i=0;
								}
								else
								{
									x[i]=c;
									i++;
								}
							}
							close(fd);
							fd=open(path_name,O_WRONLY|O_TRUNC);
							write(fd,new_user,strlen(new_user));
							write(fd,"\n",1);
							write(fd,pass,strlen(pass));
							close(fd);
							strcpy(accounts[idx].user,new_user);
							strcpy(path_name,path);
							strcat(path_name,user1);
							strcat(path,new_user);
							rename(path_name,path);
							save(no_of_accounts);
							no_of_accounts=load();
							char snd[]={"Account has been modified successfully\n"};
							wr(nsd,snd);
							break;
						}
						case 4:
						{
							char snd[]={"Logged out successfully\n"};
							wr(nsd,snd);
							flag=1;
							break;
						}
						case 5:
						{
							save(no_of_accounts);
							char snd[]={"Changes have been saved successfully\n"};
							wr(nsd,snd);
							no_of_accounts=load();
							break;
						}
					}
				}
			}
			else
			{
				int flag=0,i,n,fd;
				char path_name[80]={"user_data/"},c,cur_path[80];
				strcat(path_name,id);
				while(!flag)
				{
					char buf[10];
					int ch;
					rd(nsd,buf);
					ch=(buf[0]-'0');
					switch(ch)
					{
						case 1:
						{
							// printf("In case 1\n");
							strcpy(cur_path,path_name);
							strcat(cur_path,"/transactions.txt");
							fd=open(cur_path,O_WRONLY);

							char snd[80]={" has deposited "};
							char tmp[80];
							strcpy(tmp,id);
							strcat(tmp,snd);
							char rcv[80]={};
							rd(nsd,rcv);
							strcat(tmp,rcv);
							strcat(tmp,"\n");

							int x;
							x=atoi(rcv);
							// printf("Before transactions' critical section\n");
							// char snd3[]={"Before transactions' critical section\n"};
							// wr(nsd,snd3);

							flock(fd,LOCK_EX);
							lseek(fd,0,SEEK_END);
							write(fd,tmp,strlen(tmp));
							char c;
							// char snd2[80]={"Press enter to unlock\n"};
							// wr(nsd,snd2);
							// read(nsd,&c,1);
							flock(fd,LOCK_UN);
  						// printf("\n release lock \n");
							close(fd);


							strcpy(cur_path,path_name);
							strcat(cur_path,"/balance.txt");
							fd=open(cur_path,O_WRONLY);
							// fl.l_pid= 0;
							// printf("Before balance's critical section\n");
							// char snd4[]={"Before balance's critical section\n"};
							// wr(nsd,snd4);

							flock(fd,LOCK_EX);
  						// fcntl(fd,F_SETLKW,&fl);
							// printf("HERE\n");
							int fd1;
							fd1=open(cur_path,O_RDONLY);
							char snd1[80]={};
							// printf("%s\n",snd);
							i=0;
							while(n=read(fd1,&c,1))
							{
								// printf("%c\n",c);
								snd1[i]=c;
								i++;
							}
							// printf("Outside loop\n");
							int balance=atoi(snd1);
							// printf("%d\n",balance);
							balance=balance+x;
							// printf("%d\n",balance);
							close(fd1);
							fd1=open(cur_path,O_WRONLY|O_TRUNC);
							close(fd1);
							char buffer[10];
							sprintf(buffer,"%d",balance);
							// printf("%s\n",buffer);
							write(fd,buffer,strlen(buffer));

							// wr(nsd,snd2);
							// read(nsd,&c,1);
							flock(fd,LOCK_UN);
  						// printf("\n release lock \n");
							close(fd);

							char msg[]={"Your deposit has been successful\n"};
							wr(nsd,msg);
							break;
						}
						case 2:
						{
							// printf("In case 2\n");
							strcpy(cur_path,path_name);
							strcat(cur_path,"/transactions.txt");
							fd=open(cur_path,O_WRONLY);

							char rcv[80]={};
							rd(nsd,rcv);
							char snd[80]={" has withdrawn "};
							char tmp[80];
							strcpy(tmp,id);
							strcat(tmp,snd);
							strcat(tmp,rcv);
							strcat(tmp,"\n");

							int x;
							x=atoi(rcv);
							// char snd3[]={"Before transactions' critical section\n"};
							// wr(nsd,snd3);

							flock(fd,LOCK_EX);
							lseek(fd,0,SEEK_END);
							write(fd,tmp,strlen(tmp));

							char c;
							// char snd2[80]={"Press enter to unlock\n"};
							// printf("HERE\n");
							// wr(nsd,snd2);
							// read(nsd,&c,1);
							flock(fd,LOCK_UN);
  						// printf("\n release lock \n");
							close(fd);

							strcpy(cur_path,path_name);
							strcat(cur_path,"/balance.txt");
							fd=open(cur_path,O_WRONLY);
							// char snd4[]={"Before balance's critical section\n"};
							// wr(nsd,snd4);
							flock(fd,LOCK_EX);
							int fd1;
							fd1=open(cur_path,O_RDONLY);
							char snd1[80]={};
							// printf("%s\n",snd);
							i=0;
							while(n=read(fd1,&c,1))
							{
								// printf("%c\n",c);
								snd1[i]=c;
								i++;
							}
							// printf("Outside loop\n");
							int balance=atoi(snd1);
							// printf("%d\n",balance);
							balance=balance-x;
							// printf("%d\n",balance);
							close(fd1);
							fd1=open(cur_path,O_WRONLY|O_TRUNC);
							close(fd1);

							char buffer[10];
							sprintf(buffer,"%d",balance);
							// printf("%s\n",buffer);
							write(fd,buffer,strlen(buffer));

							// wr(nsd,snd2);
							// read(nsd,&c,1);
							flock(fd,LOCK_UN);
  						// printf("\n release lock \n");
							close(fd);

							char msg[]={"Your have withdrawn successful\n"};
							wr(nsd,msg);
							break;
						}
						case 3:
						{
							strcpy(cur_path,path_name);
							strcat(cur_path,"/balance.txt");
							// printf("%s\n",cur_path);
							fd=open(cur_path,O_RDONLY);
							// char snd4[]={"Before balance's critical section\n"};
							// wr(nsd,snd4);
							flock(fd,LOCK_SH);
							i=0;
							char snd[80]={};
							// printf("%s\n",snd);
							while(n=read(fd,&c,1))
							{
								// printf("%c\n",c);
								snd[i]=c;
								i++;
							}
							// printf("%s\n",snd);
							// char snd2[]={"Press enter to unlock\n"};
							// wr(nsd,snd2);
							// read(nsd,&c,1);
							flock(fd,LOCK_UN);
  						// printf("\n release lock \n");
							close(fd);

							wr(nsd,snd);
							break;
						}
						case 4:
						{
							strcpy(cur_path,path_name);
							strcat(cur_path,"/settings.txt");
							// printf("%s\n",cur_path);
							char snd1[80]={"UserName : "},snd2[80]={"Password : "};
							char *x=snd1;
							fd=open(cur_path,O_RDONLY);
							flock(fd,LOCK_SH);
							i=strlen(snd1);
							while(n=read(fd,&c,1))
							{
								if(c=='\n')
								{
									x=snd2;
									i=strlen(snd2);
								}
								else
								{
									x[i]=c;
									i++;
								}
							}
							// printf("%s\n",snd1);
							// printf("%s\n",snd2);
							wr(nsd,snd1);
							wr(nsd,snd2);
							flock(fd,LOCK_UN);
							close(fd);
							break;
						}
						case 5:
						{
							// printf("In case 5\n");
							char rcv[80]={};
							rd(nsd,rcv);
							// printf("%s\n",rcv);
							int x,idx;
							x=load();
							idx=present(id,x);
							// printf("%d\n",idx);
							// printf("%s\n",accounts[idx].pass);
							strcpy(accounts[idx].pass,rcv);
							save(x);
							strcpy(cur_path,path_name);
							strcat(cur_path,"/settings.txt");
							int fd,off;
							fd=open(cur_path,O_WRONLY|O_TRUNC);
							write(fd,id,strlen(id));
							write(fd,"\n",1);
							write(fd,rcv,strlen(rcv));
							close(fd);
							char snd[]={"Updated password successfully\n"};
							wr(nsd,snd);
							break;
						}
						case 6:
						{
							char snd[]={"Logged out successfully\n"};
							wr(nsd,snd);
							flag=1;
							break;
						}
					}
				}
			}
			// int mz;
			// mz=rd(nsd,buf,sizeof(buf));;
			// printf("Message from client: %d\n",mz);
			// write(nsd,"ACK from server\n",17);
			// printf("Thank you for using our service\n");
			exit(1);
		}
		else
		{
			close(nsd);
		}
	}
}
