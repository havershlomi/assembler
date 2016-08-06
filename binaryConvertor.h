
/* get word that represent an action and returns its decimal value*/
unsigned int convertCommandWordToInt(WordDef *wordDef);
/* get word that represent a register value and returns its decimal value*/
unsigned int convertRegisterValueWordToInt(WordDef *wordDef);
/* get word that represent a regular value and returns its decimal value*/
unsigned int convertRegularValueWordToInt(WordDef *wordDef);
/* get a decimal number and return a Base8 struct (dividing every 3 bits seperatly)*/
WordDef * convertDecimalToBase8(int number);
/* get Base8 struct and return a representation of its value in the special 8 base */
char* getBase8String(WordDef * base8);
/* get a decimal number and return a representation of its value in the special 8 base */
char* getSpecialBase8String(int number);