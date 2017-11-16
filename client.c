#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_LEN 128
char buffer[BUF_LEN];

int *connection_handler(void *);

int main(int argc, char *argv[]){

	pthread_t thread_t[5];
	struct sockaddr_in server_addr;
	int client_fd[5];	
	int port[5];

	printf("Type Server Ports(format : <Port1> <Port2> <Port3> <Port4> <Port5>");
	scanf("%d %d %d %d %d", &port[1], &port[2], &port[3], &port[4], &port[5]);

	for (int i = 0; i < 5; i++){
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port[i]);
		server_addr.sin_addr.s_addr = inet_addr();
		
		if ( (client_fd[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
			printf("creation failed\n");
			exit(0);
		}
		if ( connect(client_fd[i], (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
			printf("connection failed\n");
		}
		if (pthread_create(&thread_t[i], NULL, connection_handler, (void*)&client_fd[i]) < 0){
			perror("thread create error:");
			exit(0);
		}
	
		delay(5000); // wait for 5 seconds
	}
	return 0;
}

int *connection_handler(void *client_fd){

	read(client_fd, buffer, BUF_LEN);

	char filename[2];
	filename[1] = client_fd.sin_port + '0';
	filename[2] = ".txt"

	FILE *file = fopen( *filename , "a");

	struct tm *tm_struct = localtime(time(NULL));

	fprintf(file, "%d:%d:%d.%d <%d> <%s>\n", tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec,
		strlen(buffer), buffer);
	fclose(file);

	close(client_fd);

	return 0;
}

