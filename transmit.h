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

void convert_num_to_string(int n,char *num)
{
	int x=n/10;
	int y=n%10;
	num[0]=(x+'0');
	num[1]=(y+'0');
}
void wr(int nsd,char *s)
{
	int n=strlen(s);
	// printf("%d\n",n);
	char num[2];
	convert_num_to_string(n,num);
	// printf("%s\n",num);
	write(nsd,num,sizeof(num));
	// printf("%s\n",s);
	write(nsd,s,n+1);
}

void rd(int sd,char *s)
{
	char num[2];
	read(sd,num,sizeof(num));
	// printf("%s\n",num);
	int n=0;
	for(int i=0;i<2;i++) n=10*n+(num[i]-'0');
	// printf("n = %d\n",n);
	read(sd,s,n+1);
}
