#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 100000

int main(void) {
	int read_file = open("/proc/myproc/myproc", O_RDONLY);
	char buf[SIZE] = "";
	printf("1");

	int read_result = read(read_file, buf, SIZE);
	if (read_result < 0) {
		printf("fail\n");
		return 0;
	} else {
		printf("%s\n",buf);
	}

	int write_file = open("proc_result.txt", O_WRONLY);
	if (write_file < 0) {
		printf("write fail\n");
		return 0;
	} else {
		printf("write success\n");
	}


	return 0;
}
