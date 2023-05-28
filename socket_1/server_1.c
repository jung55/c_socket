#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define buf_size 20

void main()
{
	//socket(): 소켓 열기
	int ss, sa;
	char buf_recv[buf_size];
	char buf_send[buf_size];
	ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//bind(): 소켓 주소 지정, 소켓 연결
	struct sockaddr_in ssa;
	memset(&ssa, 0, sizeof(ssa));
	ssa.sin_family = AF_INET;
	ssa.sin_addr.s_addr = htonl(INADDR_ANY);
	ssa.sin_port = htons(11234);
	bind(ss, (struct sockaddr *)&ssa, sizeof(ssa));

	while(1){
		//listen(): 여러 소켓들을 활성화
		listen(ss, 10);
		//accept(): 소켓에 연결된 클라이언트를 받아들이기
		sa = accept(ss, 0, 0);
		//클라이언트에게 받기/보내기
		recv(sa, buf_recv, buf_size, 0);
		printf("server send string[%s]\n", buf_recv);
		
		int i = 0;
		int d = 0;
		char s[buf_size] = "";
		char ascii[buf_size] = "";
		int len = 0;
		for (i; buf_recv[i] != '\0'; i++) {
			d = (int)buf_recv[i];
			len = sprintf(ascii, "%d", d);
			if (strlen(s)+len+1> buf_size) {
				break;
			}
			strcat(s, ascii);
		}
		sprintf(buf_send, s);
		
		send(sa, buf_send, strlen(buf_send)+1, 0);
		//close(): 클라이언트 소켓 닫기
		close(sa);
	}

}
