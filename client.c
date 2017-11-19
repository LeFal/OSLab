#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_LEN 128
char buffer[BUF_LEN];
int port[5] = {4444,5555,6666,7777,8888};

void connection_handler(int port);

int main(){

	pthread_t thread_t[5];
	int i = 0;
	while (i < 1000){
		for (int i = 0; i < 5; i++){
			printf("iteration : %dth\n", i);

			if (pthread_create(&thread_t[i], NULL, connection_handler, port[i])){
				perror("thread create error:");
				exit(0);
			}
		
			usleep(500000); // wait for 5 seconds
		}
		i = i + 1;
	}
	return 0;
}

void connection_handler(int port){
	printf("connection handler called\n");	

	struct sockaddr_in server_addr;
	int client_fd;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("192.168.99.100");
	
	if ( (client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		printf("creation failed\n");
		exit(0);
	}
	if ( connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
		printf("connection failed\n");
	}

	read(client_fd, buffer, BUF_LEN);
	printf("read : %s\n", buffer);

	char filename[10];
	sprintf(filename, "%s", port);
	sprintf(filename, ".txt");

	FILE *file = fopen( "hi" , "wb");

	struct tm *tm_struct = localtime(time(NULL));

	fprintf(file, "%d:%d:%d.%d <%d> <%s>\n", tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec,
		strlen(buffer), buffer);
	fclose(file);

	close(client_fd);

	return;
}

