#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASHLENGTH  64
struct hash_struct{
    __uint32_t hash[HASHLENGTH];
};
#define byteSwap32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00)<< 8) | ((x) << 24))
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
__uint32_t sig0(__uint32_t x);
__uint32_t sig1(__uint32_t x);

__uint32_t rotr(__uint32_t n, __uint16_t x);
__uint32_t shr(__uint32_t n, __uint16_t x);

__uint32_t SIG0(__uint32_t x);
__uint32_t SIG1(__uint32_t x);

__uint32_t Ch(__uint32_t x,__uint32_t y,__uint32_t z);
__uint32_t Maj(__uint32_t x,__uint32_t y,__uint32_t z);


int main(int argc, char *argv[])
{
    struct hash_struct value;
    char * s = "abc";//malloc( sizeof(char) *2000);
    //fgets(s,8*2000,stdin);
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
char * int_to_char(__uint32_t x){
    char * t =malloc(sizeof(char)*4);
    for (int i =0;i<4;i++) {
        t[i] = x >> (24-(8*i));
    }
    return t;
}

struct hash_struct hash(char * value){
    int place = 0;
    int index = 0;
    struct hash_struct hashed_val;
    __uint32_t WV [8]; // array of variables a-h
    __uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };

    for (int i =0;i<HASHLENGTH;i++) {
        hashed_val.hash[i] = 0;
    }


    //the padding needs to have make the message of size n where n%block_size=0
    __uint64_t end = strlen(value);
    unsigned char tmp [((end/64)+1)*64];
    for (int i = strlen(tmp)-1;i>=0;i--) {
        tmp[i]=0;
    }
    strcat(tmp,value);
    for (int i =end;i<((end/64)+1)*64;i++) {
        tmp[i]=0;
    }
    tmp[48]=0;
    printf("%i\n",((end/64)+1)*64);
    for (int i = 7;i>=0;i--) {
        unsigned char t = (((strlen(value))*8)>>(8*i))%256;
        //printf("%X\n",t);
        tmp[((end/64)+1)*64-(i+1)] = t;
        printf("padding: %X %i\n",tmp[((end/64)+1)*64-1],t);
    }
    tmp[end] = 128;
    //tmp[end+1] = 128;
    for (size_t k = 0;k<8;k++){
        //WV[k] = K[k];
        //H[k] = K[k];
    }

    end = ((end/64)+1)*64;
    printf("messeage prior: %s\n",tmp);
    //printf("T1 %x\n",tmp[0]);
    while(index<end){
        printf("index value: %i,%i\n\n\n",index,end);
        for(int i =0;i<16;i++){
            //hashed value is a uint32 tmp is a char load the proper amount of bits into the uint32
            //should be about 4 tmp char to one hashed_val'
            for (int j = index+(i*4);j<index+(i*4)+4;j++) {
                //printf("T1: %s\n",tmp[0]);
                hashed_val.hash[i] = hashed_val.hash[i]<<8;
                hashed_val.hash[i] =hashed_val.hash[i]+tmp[j];
                //printf("%i,%X \n",i,tmp[j]);
            }
            /*if(i!=15){
                hashed_val.hash[i] = byteSwap32(hashed_val.hash[i]);
            }
            if(i==0){
                hashed_val.hash[i] = hashed_val.hash[i] +(1<<24);
            }*/
            printf("hashed val: %08llX,\n",hashed_val.hash[i]);
        }
        //printf("\n");

        for (int j=16; j<64; j++)
        {
            // Step 1
            hashed_val.hash[j] = sig1(hashed_val.hash[j-2]) + hashed_val.hash[j-7] + sig0(hashed_val.hash[j-15]) + hashed_val.hash[j-16];
        }

        // Initalize a..h
        // Step 2
        __uint32_t a=H[0];
        __uint32_t b=H[1];
        __uint32_t c=H[2];
        __uint32_t d=H[3];
        __uint32_t e=H[4];
        __uint32_t f=H[5];
        __uint32_t g=H[6];
        __uint32_t h=H[7];

        for (int tt = 0;tt<8;tt++) {
            printf("%08llx\n",H[tt]);
        }

        // For loop
        // Step 3
        for(int j = 0; j < 64; j++)
        {
            // Creating new variables
            __uint32_t T1 = h + SIG1(e) + Ch(e,f,g) + K[j] + hashed_val.hash[j];
            __uint32_t T2 = SIG0(a) + Maj(a,b,c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        // Step 4
        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h+ H[7];
       /* for (size_t j = 16; j < 64; j++) {
            hashed_val.hash[j] = sig1(hashed_val.hash[j-2]) + hashed_val.hash[j-7] + sig0(hashed_val.hash[j-15]) + hashed_val.hash[j-16];
        }

        for (size_t i =0; i<64;i++) {
            __uint32_t tmp1 = WV[7] + SIG1(WV[4])+ Ch(WV[4],WV[5],WV[6]) + K[i]+hashed_val.hash[i];
            __uint32_t tmp2 = SIG0(WV[0])+Maj(WV[0],WV[1],WV[2]);
            for (size_t j =7;j>0;j--) {
                if(j!=4){
                    WV[j] = WV[j-1];
                }else{
                    WV[j] = WV[j-1]+tmp1;
                }
            }
            WV[0] = tmp1+tmp2;
        }
        for (size_t i = 0;i<8;i++) {
            H[i] = WV[i]+H[i];
        }*/
        index = ++place*(4*HASHLENGTH);
    }

    printf("this is what the hash should be:\n");
    for (size_t i = 0;i<1;i++) {
        for (int j=0;j<8;j++) {
            printf("%08llX",H[j]);
        }
        for (int j =0;j<8;j++) {
            hashed_val.hash[j] = H[j];
        }
    }
    printf("\n");
    return hashed_val;
}


__uint32_t sig0(__uint32_t x)
{
	return (rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3));
};

__uint32_t sig1(__uint32_t x)
{
	return (rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10));
};

__uint32_t rotr(__uint32_t x, __uint16_t a)
{
	return (x >> a) | (x << (32 - a));
};

__uint32_t shr(__uint32_t x, __uint16_t b)
{
	return (x >> b);
};

__uint32_t SIG0(__uint32_t x)
{
	return (rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22));
};

__uint32_t SIG1(__uint32_t x)
{
	return (rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25));
};

__uint32_t Ch(__uint32_t x,__uint32_t y,__uint32_t z)
{
	return ((x & y) ^ (~(x)&z));
};

__uint32_t Maj(__uint32_t x,__uint32_t y,__uint32_t z)
{
	return ((x & y) ^ (x & z) ^ (y & z));
};

