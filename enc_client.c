#include "otp.h"

int main(int argc, char* argv[]){

    //validate arguments
    if(argc < 4){
        fprintf(stderr, "USAGE: %s plaintext, key, port\n", argv[0]);
        exit(0);
    }

    //call one time pad encode = 0, decode = 1;
    otp(argv[1], argv[2], argv[3], "enc_client");

    return 0;

}