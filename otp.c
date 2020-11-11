#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "otp.h"

const int BUFF_SIZE = 1024;


int otp(char* plaintext, char* key, char* port, char* enc_dec){

    int sockFD, portNum, charsWritten, charsRead, i , j;
    struct hostent* serverHostInfo;
    struct sockaddr_in serverAdr;
    
    

    //compare sizes and check for valid chars of key file and plaintext
    FILE* ptFile = fopen(plaintext, "r");
    FILE* kFile = fopen(key, "r");
    int ptCount = 0;
    int kCount = 0;
    char letter;


    //count and check plaintext file
    while(((letter = fgetc(ptFile)) != EOF)){
        //count
        ptCount++;
        //validate letter within allowed letters
        if(strchr(letters, letter) == NULL && letter != '\n'){
            fprintf(stderr, "ERROR: Plaintext contains invalid characters.\n");
            exit(1);
        }
    }


    //count and check key file
    while((letter = fgetc(kFile)) != EOF){
        //count
        kCount++;
        //validate letter within allowed letters
        if((strchr(letters, letter) == NULL) && letter != '\n'){
            fprintf(stderr, "ERROR: Key contains invalid characters.\n");
            exit(1);
        }
    }

    //compare counts
    if(kCount < ptCount){
        fprintf(stderr, "ERROR: Key count is less than Plaintext count.\n");
        exit(1);

    }

    char buffer[ptCount];
    char cipher[ptCount];
    char pt[ptCount];
    char ky[ptCount];

    //empty server address
    memset((char*)&serverAdr, '\0', sizeof(serverAdr));

    //make address network capable
    serverAdr.sin_family = AF_INET;

    //store port number
    portNum = atoi(port);
    serverAdr.sin_port = htons(portNum);

    //get DNS entry for host name
    serverHostInfo = gethostbyname("localhost");
    if(serverHostInfo == NULL){
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(0);
    }

    //copy first IP address from DNS entry to 
    memcpy(&serverAdr.sin_addr.s_addr, serverHostInfo->h_addr_list[0], serverHostInfo->h_length);

    //create socket
    if((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ClIENT: Error Opening socket.\n");
        exit(0);
    };

    //connect socket
    if((connect(sockFD, (struct sockaddr*)&serverAdr, sizeof(serverAdr))) < 0){
        perror("ClIENT: Error connecting.\n");
        exit(0);
    };

    //clear buffer
    memset(buffer, '\0', sizeof(buffer));
    sprintf(buffer, "%s%d",enc_dec,ptCount);
    charsWritten = 0;

    
    //validate connection is with appropriate client/server
    charsWritten = send(sockFD, buffer, strlen(buffer), 0);
	
    //clear buffer
    memset(buffer, '\0', sizeof(buffer));
    charsRead = 0;

    //wait for response
    charsRead = recv(sockFD, buffer, 5, 0);
    if (charsRead < 0){
        perror("ERROR reading from socket");
        exit(1);
    }

    //check validation response
    if(strcmp(buffer, "error") == 0){
        perror("ClIENT: ERROR Wrong Connection.\n");
        exit(2);
    }

    //clear buffer
    memset(buffer, '\0', sizeof(buffer));
    memset(pt, '\0', sizeof(pt));
    memset(ky, '\0', sizeof(ky));
    charsWritten = 0;

    //restart file pointers to the beginning of file
    rewind(ptFile);
    rewind(kFile);
    fgets(pt, ptCount, ptFile);
    fgets(ky, ptCount, kFile);
    
    
    //send plaintext to server
    charsWritten = send(sockFD, pt, ptCount, 0);
    if (charsWritten < 0){
        perror("ERROR reading from socket");
        exit(1);
    }


    //clear buffer
    memset(buffer, '\0', sizeof(buffer));
    charsRead = 0;

    //receive confirmation
    charsRead = recv(sockFD, buffer, 5, 0);
    if (charsRead < 0){
        perror("ERROR reading from socket");
        exit(1);
    }

    
    //send key to server
    charsWritten = send(sockFD, ky, ptCount, 0);
    if (charsWritten < 0){
        perror("ERROR reading from socket");
        exit(1);
    }

    //clear buffer
    memset(buffer, '\0', ptCount);
    charsRead = 0;
    int received = 0;

    memset(pt, '\0', ptCount);

    //wait for cipher
    while(received < ptCount){
        charsRead = recv(sockFD, buffer, ptCount, 0);
        received += charsRead;
        strcat(pt, buffer);
        if (charsRead < 0){
            perror("ERROR reading from socket");
            exit(1);
        }
    }
    
    
    strcat(pt, "\n");
    printf("%s", pt);

    //close socket
    close(sockFD);   

    return 0;


}

