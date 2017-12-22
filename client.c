#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
 
#define BUF_LEN 100000
int port[5] = {4444,5555,6666,7777,8888};
 
void *connection_handler(int port);
 
int main(){
 
  pthread_t thread_t[5];
  int i;
 
  for (i = 0; i < 5; i++){
      printf("iteration : %dth\n", i);
      if (pthread_create(&thread_t[i], NULL, connection_handler, port[i])){
        perror("thread create error:");
        exit(0);
      }
      //pthread_detach(thread_t[i]);
  }
 
  for (i = 0; i < 5; i++) {
    pthread_join(thread_t[i], NULL);
  }
 
  return 0;
}
 
void *connection_handler(int port){
  char buffer[BUF_LEN] = {0, };
  printf("connection handler called\n");  
 
  struct sockaddr_in server_addr;
  int client_fd;
 
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("192.168.99.100");
 
  printf("port : %d", port);
 
  if ( (client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    printf("creation failed\n");
    exit(1);
  }
  printf("creation succeeded\n");
 
  if ( connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ){
    printf("connection failed\n");
    exit(1);
  }
  printf("connection succeeded\n");
 
  char filename[10];
  sprintf(filename, "%d.txt", port);
 
  FILE *file = fopen(filename , "w");
  while (1) {
    int ret = read(client_fd, buffer, BUF_LEN - 1);
    if (ret <= 0) break;
 
    //printf("read : %s\n", buffer);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    double time_in_ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
 
    time_t timer;
    time(&timer);
    struct tm *tm_struct = localtime(&timer);
    fprintf(file, "%d:%d:%d:%2lf <%d> <%s>\n",
      tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec,
      time_in_ms, ret, buffer);
    memset(buffer, 0, sizeof(buffer));
  }
 
  fclose(file);
  close(client_fd);
}
