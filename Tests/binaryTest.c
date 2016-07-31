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

int main(){

    Word* shlomi;
     shlomi =  (Word*)malloc(15);

        shlomi -> data.cell = -333;
    
        printf("Test: %d",shlomi -> data.cell);
        completionTo2(14);
        completionTo2(13);

    return 0;
}