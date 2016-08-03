#include "codeCollection.h"
#include "utils.h"

extern Action ValidActions[];

static int ic = 100;

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
    char operAttr[LINE_LENGTH]  = "", extraData[LINE_LENGTH] = "";
    
    if(sscanf(oper," %*[#]%*[-+]%d %[^\n]",&number,extraData) >= 1 ||
        sscanf(oper," %*[#]%d %[^\n]",&number,extraData) >= 1){
            if(strlen(extraData) == 0)
                return instant;
            return invalidOperand;
    } else if(sscanf(oper," %*[r]%d %[^\n]",&number,extraData) >= 1){
        if(number >= 0 && number <= 7 && strlen(extraData) == 0)
            return directRegister;
        return invalidOperand;
    } else if(sscanf(oper," %*[^[][%d,%d] %[^\n]",&number,&number2,extraData) >= 2 ||
            sscanf(oper," %*[^[][%d,] %[^\n]",&number,extraData) >= 1 ||
            sscanf(oper," %*[^[][,%d] %[^\n]",&number,extraData) >= 1){
        if(strlen(extraData) == 0 && number < number2 && number >= 0 && number2 <= 13)
            return dynamic;
        return invalidOperand;
    } else if(sscanf(oper," %[^ \t\r] %[^\n]",operAttr,extraData) >= 1 && strchr(oper,'[') == NULL
             && strchr(oper,']') == NULL){
        if(strlen(extraData) == 0)
            return direct;
        return invalidOperand;
    } else {
        return invalidOperand;        
    }
}

int isValidBlockAddressTypeForAction(int sourcingType, int* validBLA){
    int i = 0, arrayMaxLength = 4; 
    for(i = 0; i < arrayMaxLength; i++){
        if(validBLA[i] == -1)
            return false;
        if(validBLA[i] == sourcingType){
            return true;
        }
    }
    return false;
}

int getActionRefrenceinMemory(int blaSrc, int blaDest){
    int numberOfRows = 1;
    if(blaDest == directRegister && blaSrc == directRegister){
        numberOfRows += 1;
    } else if(blaSrc != notUsedOper && blaDest != notUsedOper){
        numberOfRows += 2;
    } 
    else if((blaSrc == notUsedOper && blaDest != notUsedOper) ||
            (blaDest == notUsedOper && blaSrc != notUsedOper)){
        numberOfRows += 1;
    }
    ic += numberOfRows;

    return ic;
}

int getICPointer(){
    return ic;
}