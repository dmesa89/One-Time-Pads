#include "otp.h"

char intToChar(int num){

    if(num > 26){
        num = num % 27;
    }

    return letters[num];

}

int charToInt( char c){
    
    int i;

    for(i = 0; i < sizeof(letters)/sizeof(letters[0]); i++){
        if(letters[i] == c){
            return i;
        }
    }

    return 0;
}


void encode(char plaintext[], char key[]){

    int i = 0;
    char n;
    int c;


    while(plaintext[i] != '\0'){

        if(plaintext[i] == '\n'){
            continue;
        }
        //get char
        n = plaintext[i];
        //convert letters in both pt and key to ints and 
        //add their sum modulo 27
        c = (charToInt(plaintext[i]) + charToInt(key[i])) % 27;
        //printf("pt:%c(%d)  +  key:%c(%d) mod27 = %c(%d)\n", plaintext[i], charToInt(plaintext[i]), key[i], charToInt(key[i]), letters[c], c);

        //replace current char in pt to encoded char
        plaintext[i] = letters[c];
        i++;
    }


    return;
}


void decode(char cipher[], char key[]){

    int i = 0;
    char n;
    int c;


    
    while(cipher[i] != '\0'){

        if(cipher[i] == '\n'){
            continue;
        }

        //get char
        n = cipher[i];

        //convert letters in both pt and key to ints and 
        //add their sum modulo 27
        c = (charToInt(cipher[i]) - charToInt(key[i]));

        //add 27 to negative results
        if(c < 0){
            c += 27;
        }

        //replace current char in cipher with decoded char
        cipher[i] = letters[c];
        i++;
    }


    return;
}