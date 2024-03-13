#ifndef ERRORCHECKER
#define ERRORCHECKER

int hasTrailingZeros(char* eq); // Checks for trailing zeros
int hasAptLength(int eqNumId); // Checks for appropriate length
int hasForbiddenChars(char* eq); // Checks for forbidden characters
int hasDivZero(char* eq); // Checks for division by zero
int hasWrongConstruction(char* eq); // Checks for the right construction
double betterCalcResult(char* eq); // Calculates the result of an equation given as a string

/* Returns exit code
0 - correct
1 - Invalid length
2 - Forbdiden characters detected
3 - Trailing zeores detected
4 - Division by zero detected
5 - Doesnt calculate to desired value
*/
int checkCorrectness(int eqNumId, char* eq, int desiredResult); // Checks for all errors


#endif
