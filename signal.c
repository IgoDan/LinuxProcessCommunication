#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#include <signal.h>

void kill_handler(int pid_ch){
	kill(pid_ch, SIGCONT);
	kill(pid_ch, 10);
	return;
}

void stop_handler(int pid_ch){
	kill(pid_ch, 11);
	return;
}

void start_handler(int pid_ch){
	kill(pid_ch, SIGCONT);
	kill(pid_ch, 12);
	return;
}

int main(){
	FILE *pid_read;
	int pid_m, pid1, pid2, pid3, pid_ch;
	
	int wybor = -1;
	int wybor_pid = 0;
	
	while(wybor != 0){
		
		pid_read = fopen("pid.txt", "r");
		fscanf(pid_read, "%d", &pid_m);
		fscanf(pid_read, "%d", &pid3);
		fscanf(pid_read, "%d", &pid2);
		fscanf(pid_read, "%d", &pid1);
		fclose(pid_read);
		
		printf("%d	%d	%d	%d\n", pid_m, pid1, pid2, pid3);
		
		printf("0 - Wyjdz\n");
		printf("1 - Wyślij sygnal kill\n");
		printf("2 - Wyslij sygnal stop\n");
		printf("3 - Wyslij sygnal start\n");
		printf("Podaj opcje:\n");
		scanf("%d", &wybor);
		
		if (wybor == 1 || wybor == 2 || wybor ==3){
			printf("1 - Wyślij sygnal do procesu %d\n", pid1);
			printf("2 - Wyślij sygnal do procesu %d\n", pid2);
			printf("3 - Wyślij sygnal do procesu %d\n", pid3);
			printf("Podaj opcje:\n");
			scanf("%d", &wybor_pid);
		}
		else{
			return 0;
		}
		
		if (wybor_pid == 1){
			pid_ch = pid1;
		}
		else if (wybor_pid == 2){
			pid_ch = pid2;
		}
		else if (wybor_pid == 3){
			pid_ch = pid3;
		}
		
		if (wybor == 1){
			kill_handler(pid_ch);
		}
		else if (wybor == 2){
			stop_handler(pid_ch);
		}
		else if (wybor == 3){
			start_handler(pid_ch);
		}
	}
	return 0;
}
