#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1024

void recv_file(int client_sock)
{
    int n; 
    char buf[SIZE];

    while(1)
    {
        n = recv(client_sock, buf, SIZE, 0);
        if(n<=0)
        {
            break;
        }
        printf("%s", buf);
        memset(&buf, 0, sizeof(buf));
    }
    printf("\n");
	return;
}


int main(int argc, char* argv[])
{
    int error;

    int client_sock;
    struct sockaddr_in server_addr;
	
    //socket(): 소켓 열기
	client_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(client_sock<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");
	
	//서버 주소 지정
	memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(11234);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//connect(): 서버와 연결
    error = connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(error == -1)
    {
        perror("[-]Error in Connecting \n");
        exit(1);
    }
    printf("[+]Connected to server.\n");
	
	//소켓으로부터 보내기/받기
	if(send(client_sock, argv[1], sizeof(argv[1]), 0)== -1)
    {
            perror("[-] Error in sendung argv[1]\n");
            exit(1);
    }
    recv_file(client_sock);
    printf("[+]Data written in the text file \n");
    
	//소켓 기술자 닫기
	close(client_sock);
    printf("[+]Disconnected from the server. \n");
	
    return 0;
}