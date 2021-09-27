#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAXLEN 100

bool send_string(FILE *fp, char *str) {
	if (fprintf(fp, "%s\n", str)) {
		printf("Sent %s\n", str);
		fflush(fp);
		return true;
	} 
	return false;
}

bool receive_string(FILE *fp, char *newStr) {
	char line[MAXLEN];
	if (fgets(line, MAXLEN, fp) == NULL)
		return false;
	strcpy(newStr, strtok(line, "\n"));
	printf("Received %s\n", newStr);
	return true;
}

// Client V5 - same as Client V4, but using the functions...

int main(int argc, char *argv[]) {
	char filename[MAXLEN], clientfifo[MAXLEN], serverfifo[MAXLEN], msgs[MAXLEN];
	sprintf(filename, "/tmp/%s", getenv("USER"));
	sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(clientfifo, 0666);

	FILE *fp = fopen(filename, "w");
	send_string(fp, clientfifo);
	fclose(fp);
	
	fp = fopen(clientfifo, "r");
	receive_string(fp, serverfifo);
	fclose(fp);
//	remove(clientfifo);

        fp = fopen(clientfifo, "r");
        receive_string(fp, serverfifo);
        fclose(fp);
        //remove(clientfifo);

	int count = 0;
	while(count < 1) {
		FILE *fp = fopen(msgs, "w");
        	send_string(fp, "test");
	        fclose(fp);	
		count++;
	}
	remove(clientfifo);
	fp = fopen(serverfifo, "w");
	send_string(fp, (argc > 1 ? argv[1] : "Done."));
	fclose(fp);
	remove(serverfifo);
}
