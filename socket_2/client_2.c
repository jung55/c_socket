#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define buf_size 20

int main(int argc, char **argv)
{
	//socket(): 소켓 열기
	int cs;
	char buf[buf_size];
	cs = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	//서버 주소 지정
	struct sockaddr_in csa;
	memset(&csa, 0, sizeof(csa));
	csa.sin_family = AF_INET;
	csa.sin_addr.s_addr = inet_addr("127.0.0.1");
	csa.sin_port = htons(11234);

	//connect(): 서버와 연결
	connect(cs, (struct sockaddr *) &csa, sizeof(csa));

	//소켓으로부터 보내기/받기 
	send(cs, argv[1], strlen( argv[1])+1,0);
	recv(cs, buf, buf_size, 0);
	printf("client receive answer[%s]\n", buf);

	//소켓 기술자 닫기
	close(cs);
}
