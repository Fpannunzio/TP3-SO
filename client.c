#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ADDRESS "0.0.0.0"
#define PORT 8080
#define ANS_1 "entendido\n"
#define ANS_2 "0854780\n"
#define ANS_3 "martin luther king\n"
#define ANS_4 "es_go_lo_dro_bo\n"
#define ANS_5 "too_easy\n"
#define ANS_6 ".RUN_ME\n"
#define ANS_7 "in_de_ter_mi_na_do\n"
#define ANS_8 "thunder thunder thunder thundercats\n"
#define ANS_9 "this is awesome\n"
#define ANS_10 "chin_chu_lan_cha\n"
#define ANS_11 "gdb_manda\n"


int main(int argc, char const *argv[])
{
    int sock;
	struct sockaddr_in server;
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , IPPROTO_IP);

	if (sock == -1)
		printf("Could not create socket");
	
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr(ADDRESS);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("connect failed. Error");
		return 1;
    }

    puts("Connected\n");

    if(send(sock , ANS_1 , strlen(ANS_1), 0) < 0){
		puts("Send 1 failed");
		return 1;
	}

    if(send(sock , ANS_2 , strlen(ANS_2), 0) < 0){
		puts("Send 2 failed");
		return 1;
	}

	if(send(sock , ANS_3 , strlen(ANS_3), 0) < 0){
		puts("Send 3 failed");
		return 1;
	}

	if(send(sock , ANS_4 , strlen(ANS_4), 0) < 0){
		puts("Send 4 failed");
		return 1;
	}

	if(send(sock , ANS_5 , strlen(ANS_5), 0) < 0){
		puts("Send 5 failed");
		return 1;
	}

	if(send(sock , ANS_6 , strlen(ANS_6), 0) < 0){
		puts("Send 6 failed");
		return 1;
	}

	if(send(sock , ANS_7 , strlen(ANS_7), 0) < 0){
		puts("Send 7 failed");
		return 1;
	}

	if(send(sock , ANS_8 , strlen(ANS_8), 0) < 0){
		puts("Send 8 failed");
		return 1;
	}

	if(send(sock , ANS_9 , strlen(ANS_9), 0) < 0){
		puts("Send 9 failed");
		return 1;
	}

	if(send(sock , ANS_10 , strlen(ANS_10), 0) < 0){
		puts("Send 10 failed");
		return 1;
	}

	if(send(sock , ANS_11 , strlen(ANS_11), 0) < 0){
		puts("Send 11 failed");
		return 1;
	}

    close(sock);

	return 0;
}


