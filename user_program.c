#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 100000

int main(void) {
	int read_file = open("/proc/myproc/myproc", O_RDONLY);
	char buf[SIZE] = "";

	int read_result = read(fd, buf, SIZE);
	if (read_result < 0) {
		printf("fail\n");
		return 0;
	}

	int write_file = open("proc_result.txt", O_WRONLY);
	if (write(fd_write, buf, SIZE) < 0) {
		printf("write fail\n");
		return 0;
	}


	return 0;
}
