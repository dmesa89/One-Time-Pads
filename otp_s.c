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

const int BUFF_SIZE = 256;
const int N_SIZE = 256;
const int M_SIZE = 80000;
const int F_SIZE = 32;

int otp_s(char* port, char* enc_dec){
    int charsRead = 0;
    int sockFD, connectedFD, portNum, status;
    socklen_t sizeClientInfo;
    struct sockaddr_in serverAdr, clientAdr;
    pid_t pid, spawnid;
    char buffer[BUFF_SIZE];




    //empty server address
    memset((char*)&serverAdr, '\0', sizeof(serverAdr));

    //make address network capable
    serverAdr.sin_family = AF_INET;

    //store port number
    portNum = atoi(port);
    serverAdr.sin_port = htons(portNum);

    //allow any address for connection
    serverAdr.sin_addr.s_addr = INADDR_ANY;

    //create listening socket
    if((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("SERVER: Error Opening socket.\n");
        exit(1);
    };

    //bind socket to listen
    if(bind(sockFD, (struct sockaddr*)&serverAdr, sizeof(serverAdr)) < 0){
        perror("SERVER: Error binding.\n");
        exit(1);
    }

    //allow socket to accept max 5 connections
    listen(sockFD, 5);

    while(1){

        //length of accepted client address
        sizeClientInfo = sizeof(clientAdr);

        if((connectedFD = accept(sockFD, (struct sockaddr*)&clientAdr, &sizeClientInfo)) < 0){
            perror("SERVER: Error accepting connection.\n");
            exit(1);
        }

        pid = fork();

        if(pid == -1){
            perror("SERVER: Error creating fork.\n");
                exit(1);

        }else if(pid == 0){


            //clear buffer
            memset(buffer, '\0', sizeof(buffer));

            
            //retrieve message leaving \0
            charsRead = recv(connectedFD, buffer, 256, 0);
            if (charsRead < 0){
                perror("ERROR reading from socket");
                exit(1);
            }

        
            
            //validate connection is with appropriate server/client
            if(strncmp(buffer, enc_dec, 10) != 0){
                charsRead = send(connectedFD, "error", 5, 0);
                exit(2);
                break;
            }

            //save pt length 
            int length = atoi(&buffer[10]);

            charsRead = send(connectedFD, "right", 5, 0);
            if (charsRead < 0){
                perror("ERROR writing to socket");
                exit(1);
            }
            charsRead = 0;

            char nbuffer[length];
            char message[length];
            char key[length];

            //clear buffers
            memset(nbuffer, '\0', length);
            memset(message, '\0', length);
            memset(key, '\0', length);
            charsRead = 0;
            int received = 0;

            while(received < length){
                charsRead = recv(connectedFD, nbuffer, length, 0);
                strcat(message, nbuffer);
                received += charsRead;
                if (charsRead < 0){
                    perror("ERROR reading from socket");
                    exit(1);
                }
            }
            

            //send confirmation of received
            charsRead = send(connectedFD, "right", 5, 0);
            if (charsRead < 0){
                perror("ERROR writing to socket");
                exit(1);
            }

            //clear nbuffer
            memset(nbuffer, '\0', length);
            charsRead = 0;
            received = 0;

            
            
            while(received < length){
                //use same length for key
                charsRead = recv(connectedFD, nbuffer, length, 0);
                received += charsRead;
                strcat(key, nbuffer);
                if (charsRead < 0){
                    perror("ERROR reading from socket");
                    exit(1);
                }
            }

            //printf("key: %s\n", key);
                

            //clear buffer
            charsRead = 0;

            //printf("key: %s\n", key);

            if(strncmp(enc_dec, "enc", 3) == 0){
                encode(message, key);
            }else{
                decode(message, key);
            }

            //printf("ms: %s\n", message);
            //send message back
            charsRead = send(connectedFD, message, length, 0);


            exit(0);

        }else{
            spawnid = waitpid(pid, &status, WNOHANG);
        }

        //close connection
        close(connectedFD);

    }

    close(sockFD);
    return 0;


}