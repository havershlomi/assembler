#include "codeCollection.h"
#include "sharedCollections.h"
#include "utils.h"

Action* getActionByName(const char* name){
    int i = 0;
        printf("%s",name);
    for(i = 0; i < 16; i++){
        if(strcmp((ValidActions[i].name),name)==0){
            return &ValidActions[i];
        }
    }
    return NULL;
} 

