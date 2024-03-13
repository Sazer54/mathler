#ifndef EQGENERATOR_H
#define EQGENERATOR_H

int* getDivisors(int n, int digitNum); // Finds divisors of n with a specified number length
void fillEquation1(eq* eq, int ff, int sf, int r, char op); // Fills an equation with one operator
void fillEquation2(eq* eq, int ff, int sf, int tf, int r, char op1, char op2); // Fills an equation with two operators
int areEqual(eq eq1, eq eq2); // Checks if two equations are equal
eq generateEquation(); // Generates a new equation

#endif // EQGENERATOR_H

