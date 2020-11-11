#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>


#ifndef OTP_H
#define OTP_H

static const char letters[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

void encode(char plaintext[], char key[]);
void decode(char cipher[], char key[]);

int charToInt( char c);
char intToChar(int num);

int otp_s(char* port, char* enc_dec);
int otp(char* plaintext, char* key, char* port, char* enc_dec);


#endif