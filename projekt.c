#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#include <signal.h>

#define FIFO "fifo"
#define PLIK "plik.txt"
#define PID "pid.txt"


int pid_m, pid1, pid2, pid3;

void kill_handler(int signal){
	FILE *pid_read;
	
	int pid_parent, pid_1, pid_2, pid_3;
	
	pid_read = fopen(PID, "r");
	fscanf(pid_read, "%d", &pid_parent);
	fscanf(pid_read, "%d", &pid_3);
	fscanf(pid_read, "%d", &pid_2);
	fscanf(pid_read, "%d", &pid_1);
	fclose(pid_read);
	
	int pid_check = getpid();
	
	if (pid_1 != pid_check){
		kill(pid_1, 9);
	}
	if (pid_2 != pid_check){
		kill(pid_2, 9);
	}
	if (pid_3 != pid_check){
		kill(pid_3, 9);
	}
	kill(pid_parent, 18);
	kill(pid_check, 9);
	
	return;
}

void stop_handler(int signal){
	FILE *pid_read;
	
	int pid_parent, pid_1, pid_2, pid_3;
	
	pid_read = fopen(PID, "r");
	fscanf(pid_read, "%d", &pid_parent);
	fscanf(pid_read, "%d", &pid_3);
	fscanf(pid_read, "%d", &pid_2);
	fscanf(pid_read, "%d", &pid_1);
	fclose(pid_read);
	
	int pid_check = getpid();
	
	printf("STOP: %d	%d	%d	%d\n", pid_parent, pid_1, pid_2, pid_3);
	
	if (pid_1 != pid_check){
		kill(pid_1, 19);
	}
	if (pid_2 != pid_check){
		kill(pid_2, 19);
	}
	if (pid_3 != pid_check){
		kill(pid_3, 19);
	}
	kill(pid_check, 19);
	
	return;
}

void start_handler(int signal){
	FILE *pid_read;
	
	int pid_parent, pid_1, pid_2, pid_3;
	
	pid_read = fopen(PID, "r");
	fscanf(pid_read, "%d", &pid_parent);
	fscanf(pid_read, "%d", &pid_3);
	fscanf(pid_read, "%d", &pid_2);
	fscanf(pid_read, "%d", &pid_1);
	fclose(pid_read);
	
	int pid_check = getpid();
	
	printf("START: %d	%d	%d	%d\n", pid_parent, pid_1, pid_2, pid_3);
	
	if (pid_1 != pid_check){
		kill(pid_1, SIGCONT);
	}
	if (pid_2 != pid_check){
		kill(pid_2, SIGCONT);
	}
	if (pid_3 != pid_check){
		kill(pid_3, SIGCONT);
	}
}

int main(void){
	FILE *pid_write;
	char pid_char[4];
	
	umask(0);
    mkfifo(FIFO, 0666);
    
    int wybor = -1;
    
    
    remove(PID);
	
	pid_m = getpid();
	sprintf(pid_char, "%d", pid_m);
	
	pid_write = fopen(PID, "a");
	fputs(pid_char, pid_write);
	fprintf(pid_write, "\n");
	fclose(pid_write);
	
	//Proces potomny 1
	if(fork() == 0){
		signal(10, kill_handler);
		signal(11, stop_handler);
		signal(12, start_handler);
		FILE *src;
		FILE *fifo_write;
		
		char string[100];
		
		int pid1 = getpid();
		sprintf(pid_char, "%d", pid1);
		
		pid_write = fopen(PID, "a");
		fputs(pid_char, pid_write);
		fprintf(pid_write, "\n");
		fclose(pid_write);
		
		while(1){
			usleep(10000);
			printf("Wybierz sposob podania danych:\n");
			printf("1. Z klawiatury\n");
			printf("2. Z pliku txt\n");
			printf("3. Z pliku /dev/random\n");
			scanf("%d", &wybor);
			
			//Z klawiatury
			if (wybor == 1){
				fprintf(stdout, "Podaj dane: ");
				scanf("%s", string);
			}
			
			//Z pliku txt
			else if (wybor == 2){
				src = fopen("dane.txt", "r");
				fgets(string, sizeof(string), src);
				fclose(src);
			}
			
			//Z plik /dev/random
			else if (wybor == 3){
				while(1){
				
					int random_data = open("/dev/urandom", O_RDONLY);
					read(random_data, string, 30 * sizeof(string[0]));
					close(random_data);
					
					string[30] = '\0';
					
					int k = 0;
					
					for(k = 0; k < 30; k++){
						if (string[k] < 0){
							string[k] = abs(string[k]);
						}
					}
					
					fifo_write = fopen(FIFO, "w");
					fputs(string, fifo_write);
					fclose(fifo_write);
					
					sleep(1);
				}
			}
			else if (wybor == 0){
			}
			
			fifo_write = fopen(FIFO, "w");
			fputs(string, fifo_write);
			fclose(fifo_write);
		}
	}
	
	//Proces potomny 2
	if(fork() == 0){
		signal(10, kill_handler);
		signal(11, stop_handler);
		signal(12, start_handler);
		
		FILE *fifo_read;
		FILE *file_write;
		
		char string[100];
		
		int pid2 = getpid();
		sprintf(pid_char, "%d", pid2);
		
		pid_write = fopen(PID, "a");
		fputs(pid_char, pid_write);
		fprintf(pid_write, "\n");
		fclose(pid_write);
		
		while(1){
			fifo_read = fopen(FIFO, "r");
			fgets(string, 100, fifo_read);
			fclose(fifo_read);
			
			//printf("Proces potomny 2: Otrzymalem %s\n", string);
			
			file_write = fopen(PLIK, "w");
			fputs(string, file_write);
			fclose(file_write);
		}
	}
	
	//Proces potomny 3
	if(fork() == 0){
		signal(10, kill_handler);
		signal(11, stop_handler);
		signal(12, start_handler);
		
		FILE *file_read;
		char odczyt[100];
		
		int pid3 = getpid();
		sprintf(pid_char, "%d", pid3);
		
		pid_write = fopen(PID, "a");
		fputs(pid_char, pid_write);
		fprintf(pid_write, "\n");
		fclose(pid_write);
		
		while(1){
			//opusc(semid, 0);
			file_read = fopen(PLIK, "r");
			while(!file_read){
				file_read = fopen(PLIK, "r");
				usleep(1000);
			}
			fseek(file_read, 0, SEEK_END);
			int size = ftell(file_read);
			fseek(file_read, 0, SEEK_SET);
			
			fgets(odczyt, sizeof(odczyt) - 1, file_read);
			fclose(file_read);
			
			int k = 0;
			while(k < size){
				printf("%02hhX", odczyt[k]);
				k++;
				if(k != 0)
					printf(" ");
				if(k != 0 && k % 15 == 0)
					printf("\n");
			}
			printf("\n\n");
				
			//printf("Proces potomny 3 otrzymalem: %s\n", odczyt);
			
			remove(PLIK);
		} 
	}
	
	//Proces macierzysty
	raise(SIGSTOP);
	
	remove("plik.txt");
	unlink(FIFO);
	remove(FIFO);
	remove(PID);
	
	return 0;
}
