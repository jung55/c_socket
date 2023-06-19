#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1024

void send_file(FILE *fp, int client_sock)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(client_sock, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sendung data \n");
            exit(1);
        }
        memset(&data, 0, sizeof(data));
    }
}

void main ()
{
    int error;
	FILE *fp;
    //char *filename = "test.txt";
	char buf[SIZE] = {0};
	int n;
	
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
	
	//socket(): 소켓 열기
    server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_sock<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(11234);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind(): 소켓 주소 지정, 소켓 연결
    error = bind(server_sock,(struct sockaddr*)&server_addr, sizeof(server_addr));
    if(error<0)
    {
        perror("[-]Error in Binding \n");
        exit(1);
    }
    printf("[+]Binding Successfull.\n");
	
	while(1){
		//listen(): 여러 소켓들을 활성화
		error = listen(server_sock, 10);
		if(error==0)
		{
			printf("[+]Listening...\n");
		}
		else 
		{
			perror("[-]Error in Binding \n");
			exit(1);
		}

		//accept(): 소켓에 연결된 클라이언트를 받아들이기
		addr_size = sizeof(client_addr);
		client_sock = accept(server_sock,(struct sockaddr*)&client_addr, &addr_size);
		
		//클라이언트에게 받기/보내기
		n = recv(client_sock, buf, SIZE, 0);
        if(n<=0)
        {
			perror("[-] Error in recv argv[1] \n");
            exit(1);
        }
		
		fp = fopen(buf, "r");
    	if(fp == NULL)
    	{
        	perror("[-]Error in reading file. \n");
        	exit(1);
    	}
		
		send_file(fp, client_sock);
		printf("[+]File data send successfully. \n");

		//close(): 클라이언트 소켓 
		memset(&buf, 0, sizeof(buf));
		fclose(fp);
		close(client_sock);
	}
	
	return;
}