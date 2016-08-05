#include <stdio.h>
#include <string.h>

#include "../utils.h"
#include "../parser.h"
#include "../codeCollection.h"

/* buffer must have length >= sizeof(int) + 1
// Write to the buffer backwards so that the binary representation
// is in the correct order i.e.  the LSB is on the far right
// instead of the far left of the printed string*/
void int2bin(const int a) {
    int b = 0;
int i = 0;
printf("\n");
    b = a;
    for ( i = 15; i >= 0; i--) {
        printf("%c",(b & 1) +'0');

        b >>= 1;
    }
}

void completionTo2(const int a) {
    int b = 0,oneIndex = -1;
    char c;
    int i = 0;
    printf("\n");
    int2bin(a);
    b = a;
    for ( i = 1; i <= 15; i++) {
        c = (b & 1) +'0';
        if(c == '1'){
            oneIndex = i;
            break;
        } else {
            b >>= 1;
        }
    }
    b = ~b;
    
    for ( i = 1; i < oneIndex; i++) {
        b <<= 1;
    }
    b  = b | oneIndex;
    int2bin(b);
}

void cutByBits(const int number, int from, int to){
    int i = 0, copy = number,oneIndex;
    int rangeSize = to - from;
    char bitSign = '0';
    int2bin(copy);

    for ( i = 0; i < 15; i++) {
        if(i < from){
            copy >>= 1;
        } else if(rangeSize > 0){
            rangeSize--;
            bitSign = (b & 1) +'0';
        } else {
            if(bitSign == '1'){
                
                break;
            } else {
                b >>= 1;
            }
        }

        
    }
    int2bin(copy);
    
}

int main(){

cutByBits(22,2,4);
cutByBits(256,3,4);
    completionTo2(2);
    completionTo2(13);

    return 0;
}