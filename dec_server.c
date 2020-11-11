#include "otp.h"

int main(int argc, char* argv[]){

    //validate arguments
    if(argc < 2){
        
        fprintf(stderr, "USAGE: %s port\n", argv[0]);
        exit(1);
    }

    //call one time pad connection only allowed with dec_client
    otp_s(argv[1], "dec_client");

    return 0;
}