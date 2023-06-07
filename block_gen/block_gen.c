#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASHLENGTH  64
struct hash_struct{
    unsigned char hash[HASHLENGTH];
};
__uint32_t K[] =
    {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
struct hash_struct hash(char * value);
struct hash_struct shift_right(char * value);


int main(int argc, char *argv[])
{
    struct hash_struct value;
    char * s = malloc( sizeof(char) *2000);
    fgets(s,8*2000,stdin);
    value = hash(s);

    printf("this is the hash:");
    int i =0;
    while(i<HASHLENGTH){
        if(value.hash[i]>16){
            printf("%X " ,value.hash[i++]);
            continue;
        }
            printf("0%X " ,value.hash[i++]);
    }
    printf("\n");
    return 0;
}


struct hash_struct shift_right(char * value){
    struct hash_struct v;
    char tmp = value[HASHLENGTH-1];
    for (int i = HASHLENGTH-1;i>0;i--) {
        v.hash[i] = value[i-1];
    }
    v.hash[0] = tmp;
}

struct hash_struct pad(char * word){
    int count =0;
    while(word[count++]!='\0'){
    }
    count = count-HASHLENGTH;
    char length [10];
    int rounds;
    struct hash_struct ret_val;
    for(rounds =0; count>0;count = count/2){
        length[rounds++] = count%2;
    }
    for (int i =rounds-1;i>=0;i--) {
        ret_val.hash[HASHLENGTH-1-i] = length[rounds-(i+1)];
    }
    return ret_val;
}


struct hash_struct hash(char * value){
    int place = 0;
    int index = 0;
    struct hash_struct hashed_val;
    char * tmp = malloc(sizeof(char)*(HASHLENGTH+1));
    for (int i =0;i<HASHLENGTH;i++) {
        hashed_val.hash[i] = 0;
        tmp[i] =0;
    }
    int end = strlen(value);
    while(index<end-HASHLENGTH){
        strncpy(tmp,value,HASHLENGTH);
        for(int i =0;i<HASHLENGTH;i++){
            hashed_val.hash[i] = hashed_val.hash[i]^tmp[i];
        }
        for (size_t j = 0; j < 80; j++) {
            for(int i =0;i<HASHLENGTH;i++){
                hashed_val.hash[i] = hashed_val.hash[i]^K[(i+j)%60];
            }
        }
        index = ++place*HASHLENGTH;
    }

    strncpy(tmp,value,end-index);
    for (size_t j = 0; j < 80; j++) {
        for(int i =0;i<HASHLENGTH;i++){
            if(j==0){
                hashed_val.hash[i] = hashed_val.hash[i]^K[(i+j)%60]^tmp[i];
            }else{
                hashed_val.hash[i] = hashed_val.hash[i]^K[(i+j)%60];
            }
        }
    }
    return hashed_val;
}




