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
#include "admin.h"

int server_login(int nsd,char *id)
{
	int sig=-1;
	while(1)
	{
		char user[80],pass[80];
		read(nsd,user,sizeof(user));
		read(nsd,pass,sizeof(pass));
		// printf("user = %s, pass = %s\n",user,pass);
		int uchk,pchk;
		uchk=strcmp(user,"admin");
		pchk=strcmp(pass,"admin");
		// printf("In server login, uchk = %d, pchk = %d\n",uchk,pchk);
		if(uchk==0 && pchk==0)
		{
			strcpy(id,user);
			write(nsd,"1",1);
			sig=1;
			break;
		}
		else
		{
			int no_of_accounts = load();
			int idx=present(user,no_of_accounts);
			int pcmp=strcmp(accounts[idx].pass,pass);
			if(idx==-1 || pcmp!=0)
			{
				write(nsd,"0",1);
			}
			else
			{
				strcpy(id,user);
				write(nsd,"2",1);
				sig=2;
				break;
			}
		}
	}
	// printf("%d\n",sig);
	return sig;
}

int client_login(int sd)
{
	while(1)
	{
		char user[80],pass[80],buf[80];
		printf("Enter your username: ");
		scanf("%s",user);
		printf("Enter your password: ");
		scanf("%s",pass);
		write(sd,user,sizeof(user));
		write(sd,pass,sizeof(pass));
		// printf("HERE\n");
		memset(buf,'\0',sizeof(buf));
		// printf("buf = %s\n",buf);
		read(sd,buf,sizeof(buf));
		// printf("buf = %s\n",buf);
		int res = strcmp(buf,"0");
		// printf("res = %d\n",res);
		if(res!=0)
		{
			printf("Login Successful\n");
			return (buf[0]-'0');
		}
		else
		{
			printf("Incorrect Username or Password. Try again?\n");
		}
	}
}
