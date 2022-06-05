#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


int8_t buffer[1024];

int main(){
	int fd;
	char option;

	printf("Welcome to the demo of character device driver...\n");
	fd = open("/dev/my_device", O_RDWR);
	if(fd < 0){
		printf("Cannot open device file...\n");
	}

	while(1){
		printf("*****Please enter your option*****\n");
		printf("            1.Write               \n");
		printf("            2.Read                \n");
		printf("            3.Exit                \n");
		scanf(" %c", &option);
		printf("Your options is: %c\n", option);
		
		switch(option){
			case '1':
				printf("Enter the string to write into driver:\n");
				scanf(" %[^\t\n]s", buffer);
				printf("Data Writting...");
				write(fd, buffer, strlen(buffer)+1);
				printf("Done!!!\n\n");
				break;
			case '2':
				printf("Data Reading...");
				read(fd, buffer, 1024);
				printf("Done!!!\n\n");
				printf("Your input data is: %s\n\n", buffer);
				break;
			case '3':
				printf("Bye~\n");
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter valid option!!!\nPlease try again!!!\n\n");
				break;
		}
	}
	close(fd);
}
