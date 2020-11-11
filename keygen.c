#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "otp.h"

int main(int argc, char* argv[]) {

    //validate num of args
	if(argc < 2){
		fprintf(stderr, "USAGE: %s number\n", argv[0]);
		exit(1);
	}

    srand(time(0));
    int i;
    int kLength = atoi(argv[1]);
    char key[kLength+1];
    
    //assign random chars into key
    for(i = 0; i < kLength; i++){
        int letter = rand() % 27;
        key[i] = letters[letter];

    }

    key[i] = '\0';

    printf("%s\n", key);
}