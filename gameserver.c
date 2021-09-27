#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include<time.h>

#define MAXLEN 1000
char words[84095][25];
char ltr[1];

void filetoarray() {
        FILE *fp;
        char *line = NULL;
        fp = fopen("dictionary.txt", "r");
        char word[25];

        int count = 0;
        if (fp != NULL) {
                while (fscanf(fp, "%24s", word) > 0) {
                        strcpy(words[count], word);
                        count++;
                }
        }
	fclose(fp);
}


int randomNumber() {
        int randNum;
        srand(time(NULL));
        randNum = (rand()*64*1024+rand())%84095;
        if (randNum < 0) {
                randNum = randNum * -1;
        }
	//printf("%s\n", words[randNum]);
	return randNum;
}

bool send_string(FILE *fp, char *str) {
	if (fprintf(fp, "%s\n", str)) {
		printf("Sent %s\n", str);
		fflush(fp);
		return true;
	} 
	return false;
}

bool send_string2(FILE *fp, char *sendStr) {
/*        char sendStr[100] = "(Guess) Enter a letter in word ";
        //printf("%c", sendStr[31]);
	for (int i = 31, j = 0; i < (31+length); i++) {
		sendStr[i] = gWord[j];
		j++; 
	}*/
        if (fprintf(fp, "%s\n", sendStr)) {
                printf("Sent %s\n", sendStr);
                fflush(fp);
                return true;
        }
	return false;
}

bool receive_string(FILE *fp, char *newStr) {
	char line[MAXLEN];
	if (fgets(line, MAXLEN, fp) == NULL){
		return false;}
	strcpy(newStr, strtok(line, "\n"));
	printf("Received %s\n", newStr);
	return true;
}

bool receive_string2(FILE *fp, char *newStr) {
        char line[MAXLEN];
        if (fgets(line, MAXLEN, fp) == NULL)
                return false;
        strcpy(newStr, strtok(line, "\n"));
	printf("Received %s\n", newStr);
//        ltr[0] = newStr[0];
	return true;
}

int main()
{
	filetoarray();
//	int randNum = randomNumber();
//	int length = strlen(words[randNum]);
//	printf("%s %d\n", words[randNum], length);

	char filename[MAXLEN], line[MAXLEN], serverfifo[MAXLEN], clientfifo[MAXLEN];
	sprintf(filename, "/tmp/%s", getenv("USER"));
	mkfifo(filename, 0666); 
	FILE *fp = fopen(filename, "r");
	//try to create just in case if it is not already there

	while (1) {
		//new client request
		if (receive_string(fp, clientfifo)) {
			if (fork() > 0) //child can handle the transaction
				continue;	//parent can go and wait for the next client

			//fclose(fp);
			//create a new specific pipe for the client to write back
			sprintf(serverfifo, "/tmp/%s-%d", getenv("USER"), getpid());
			mkfifo(serverfifo, 0666);

			//write to client's fifo pipe
			FILE *fp2 = fopen(clientfifo, "w");
			send_string(fp2, serverfifo);
			fclose(fp2);

		        int randNum = randomNumber();
			int length = strlen(words[randNum]);
			char gWord[length];
			for (int i = 0; i < length; i++) {
				gWord[i] = '*';
			}
		//	printf("%s %d\n", words[randNum], length);

			int count = 0;
			while (count < 1) {
				FILE *fp2 = fopen(clientfifo, "w");
			        char sendStr[100] = "(Guess) Enter a letter in word ";
			        //printf("%c", sendStr[31]);
			        for (int i = 31, j = 0; i < (31+length); i++) {
				        sendStr[i] = gWord[j];
                			j++;
        			}
				send_string2(fp2, sendStr);
				fclose(fp2);
				remove(clientfifo);
				count++;
				
                        	FILE *fp3 = fopen(serverfifo, "r");
                       	        receive_string2(fp3, line);						
				fclose(fp3);
				printf("%c\n", ltr[0]);				

			}
			printf("%d\n", 9);			
			//read the final response from the client
			FILE *fp3 = fopen(serverfifo, "r");
			receive_string(fp3, line);
			
			//it is time for clean up!
			//fclose(fp2);
			fclose(fp3);
			remove(serverfifo);
			exit(0);
		}
	}
	fclose(fp);

/*	for (int i = 0; i < 84095; i++) {
		printf("%s\n", words[i]);
	}
*/
}
