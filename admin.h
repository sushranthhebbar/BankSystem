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

#define FAILURE 0
#define SUCCESS 1
#define PRESENT 2
#define ABSENT 3

struct account
{
	char user[80];
	char pass[80];
} accounts[500];

int chk[500];

int load()
{
	memset(chk,0,sizeof(chk));
	int ret = open("accounts.txt",O_CREAT|O_EXCL,0744);
	// perror("Error : ");
	if(ret==-1)
	{
		close(ret);
		ret = open("accounts.txt",O_RDONLY);
	}
	// printf("%d\n",ret);
	char tmp[2][80];
	int i=0,j=0,k=0;
	memset(tmp,'\0',sizeof(tmp));
	char c;
	int n;
	// printf("In load\n");
	while((n=read(ret,&c,1)))
	{
		// printf("%d\n",n);
		if(c==' ')
		{
			i=0;
			j++;
		}
		else if(c=='\n')
		{
			i=0;
			j=0;
			strcpy(accounts[k].user,tmp[0]);
			strcpy(accounts[k].pass,tmp[1]);
			// printf("account no = %d\n",k);
			// printf("User = %s, Password = %s\n",accounts[k].user,accounts[k].pass);
			k++;
			memset(tmp,'\0',sizeof(tmp));
		}
		else
		{
			tmp[j][i]=c;
			++i;
		}
	}
	close(ret);
	return k;
	// printf("Out of loop\n");
}

int present(char *user,int n)
{
	// printf("Inside unique key and n = %d\n",n);
	for(int i=0;i<n;i++)
	{
		int ures;
		ures=strcmp(accounts[i].user,user);
		// printf("%d %s %s\n",i,accounts[i].user,user);
		if(ures==0)
		{
			return i;
		}
	}
	return -1;
}

void create_txn(char path[90])
{
	strcat(path,"transactions.txt");
	int ret;
	ret=open(path,O_CREAT|O_EXCL,0744);
	if(ret>=0)
	{
		close(ret);
	}
	// printf("Exit txn\n");
}

void create_balance(char path[90])
{
	strcat(path,"balance.txt");
	int ret;
	ret=open(path,O_CREAT|O_EXCL|O_WRONLY,0744);
	if(ret>=0)
	{
		write(ret,"0",1);
		close(ret);
	}
	// printf("Exit balance\n");
}

void create_settings(char path[90],struct account acc )
{
	strcat(path,"settings.txt");
	int ret;
	ret=open(path,O_CREAT|O_EXCL|O_WRONLY,0744);
	if(ret>=0)
	{
		// printf("Inside settings\n");
		for(int i=0;i<strlen(acc.user);i++)
		{
			write(ret,&acc.user[i],1);
		}
		write(ret,"\n",1);
		for(int i=0;i<strlen(acc.pass);i++)
		{
			write(ret,&acc.pass[i],1);
		}
		write(ret,"\n",1);
		close(ret);
	}
	// printf("Exit settings\n");
}

void create_files(struct account acc)
{
	// printf("In create \n");
	// printf("%s\n",acc.user);
  struct stat st,sb;
  int ret;
  ret=stat("user_data",&st);
  if(ret<0)
  {
		// printf("Make user_data\n");
    mkdir("user_data",0744);
  }
  char path_name[90];
  strcpy(path_name,"user_data/");
  strcat(path_name,acc.user);
  ret=stat(path_name,&sb);
  if(ret<0)
  {
		// printf("Inside user_data , %s\n",path_name);
    mkdir(path_name,0744);
    strcat(path_name,"/");
		char path_name1[90],path_name2[90];
		strcpy(path_name1,path_name);
		strcpy(path_name2,path_name);
		create_balance(path_name);
		create_txn(path_name1);
		create_settings(path_name2,acc);
  }
	// printf("Exit create\n");
}

void remove_files(struct account acc)
{
	// printf("%s",acc.user);
	int ret=fork();
	if(ret==0)
	{
		char *program_name ="rm";
		char *arg1 = "-rf";
		char arg2[200] = "/home/sushranth/Downloads/Operating_Systems/project/user_data/";
		strcat(arg2,acc.user);
		// perror("Error :");
		// printf("%s\n",arg2);
		// printf("Child process\n");
		int ret1;
		ret1=execlp(program_name,program_name,arg1,arg2,NULL);
		// printf("%d\n",ret1);
		/*if(ret1<0)
		{
			perror("Error :");
		}*/
	}
}

void save(int n)
{
	// printf("In save\n");
	int fd,lp;
  char user[80],pass[80];
	char blank=' ',newline='\n';
	fd=open("accounts.txt",O_TRUNC|O_WRONLY);
  // printf("%d\n",n);
	for(int i=0;i<n;i++)
	{
		strcpy(user,accounts[i].user);
		strcpy(pass,accounts[i].pass);
		// printf("User = %s, Pass = %s chk = %d\n",user,pass,chk[i]);
    // int cmp=strcmp(user,"-1");
    if(chk[i]==1)
    {
      // printf("%s\n",accounts[i].user);
			remove_files(accounts[i]);
      continue;
    }
		for(int j=0;j<strlen(user);j++)
		{
			write(fd,&user[j],1);
		}
		// write(fd,user,sizeof(user));
		write(fd,&blank,1);
		for(int j=0;j<strlen(pass);j++)
		{
			write(fd,&pass[j],1);
		}
		// write(fd,pass,sizeof(pass));
		write(fd,&newline,1);
		create_files(accounts[i]);
	}
	close(fd);
	// printf("Added new username and password successfully\n");
}

int add_account(char *user,char *pass,int n)
{
	// printf("In add\n");
	strcpy(accounts[n].user,user);
	strcpy(accounts[n].pass,pass);
	// printf("User = %s, Pass = %s\n",accounts[n].user,accounts[n].pass);
	n++;
	return n;
}

void remove_account(char* user,int idx)
{
	// printf("user = %s, idx = %d\n",user,idx);
  chk[idx]=1;
}
