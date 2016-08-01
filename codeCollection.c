#include "codeCollection.h"
#include "sharedCollections.h"
#include "utils.h"

int ic = 100;

Action* getActionByName(const char* name){
    int i = 0;
    for(i = 0; i < 16; i++){
        if(strcmp((ValidActions[i].name),name) == 0){
            return &ValidActions[i];
        }
    }
    return NULL;
}

int getOperandType(const char* oper){
    int number = -1, number2 = -1;
    char operAttr[80], *extraData;
    extraData = NULL;
/*printf("%d",sscanf(oper," %*[^[][%d,%d] ",&number,&number2));*/
    if(sscanf(oper," %*[#]%*[-+]%d",&number) == 1 ||
        sscanf(oper," %*[#]%d ",&number) == 1){
        return instant;
    } else if(sscanf(oper," %*[r]%d",&number) == 1){
        if(number >= 0 && number <= 7)
            return directRegister;
        return invalidOperand;
    } else if(sscanf(oper," %*[^[][%d,%d] %s",&number,&number2,extraData) >= 2 ||
            sscanf(oper," %*[^[][%d,] %s",&number,extraData) >= 1 ||
            sscanf(oper," %*[^[][,%d] %s",&number,extraData) >= 1){
                printf("ex:%s",extraData);
        if(extraData == NULL && number < number2 && number >= 0 && number2 <= 13)
            return dynamic;
        return invalidOperand;
    } else if(sscanf(oper," %[^ \t\r] ",operAttr) == 1 && strchr(oper,'[') == NULL
             && strchr(oper,']') == NULL){
        return direct;
    } else {
        return invalidOperand;        
    }
}