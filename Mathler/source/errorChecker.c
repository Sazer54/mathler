#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "errorChecker.h"

int hasTrailingZeros(char* eq) {
    if (eq[0] == '0' && !(eq[1] == '+' || eq[1] == '-' || eq[1] == '*' || eq[1] == '/')) {
        return 1;
    }
    for (int i = 1; i < EQUATION_SIZE - 2; i++) {
        if ((eq[i] == '+' || eq[i] == '-' || eq[i] == '*' || eq[i] == '/') && (eq[i + 1] == '0') &&
            !(eq[i + 2] == '+' || eq[i + 2] == '-' || eq[i + 2] == '*' || eq[i + 2] == '/')) {
            return 1;
        }
    }
    return 0;
}

int hasAptLength(int eqNumId) {
    return eqNumId == EQUATION_SIZE;
}

int hasForbiddenChars(char* eq) {
    for (int i = 0; i < EQUATION_SIZE; i++) {
        if (!(eq[i] >= '0' && eq[i] <= '9') && !(eq[i] == '+' || eq[i] == '-' || eq[i] == '*' || eq[i] == '/')) {
            return 1;
        }
    }
    return 0;
}

int hasDivZero(char* eq) {
    char temp[2];
    for (int i = 0; i < EQUATION_SIZE - 1; i++) {
        temp[0] = eq[i];
        temp[1] = eq[i + 1];
        if (temp[0] == '/' && temp[1] == '0') {
            return 1;
        }
    }
    return 0;
}

int hasWrongConstruction(char* eq) {
    for (int i = 0; i < EQUATION_SIZE - 1; i++) {
        if (!(eq[i] >= '0' && eq[i] <= '9') && !(eq[i + 1] >= '0' && eq[i + 1] <= '9')) {
            return 1;
        }
        if (i == 0 && !(eq[i] >= '0' && eq[i] <= '9')) {
            return 1;
        }
        if (i == EQUATION_SIZE - 2 && !(eq[i + 1] >= '0' && eq[i + 1] <= '9')) {
            return 1;
        }
    }
    return 0;
}

double betterCalcResult(char* eq) {
    double firstFactor = 0, secondFactor = 0;
    int opIndex = 0;
    char operators[2] = { NULL, NULL };
    int operatorPositions[2] = { NULL, NULL };
    for (int i = 0; i < EQUATION_SIZE; i++) {
        if (eq[i] == '+' || eq[i] == '-' || eq[i] == '*' || eq[i] == '/') {
            operators[opIndex] = eq[i];
            operatorPositions[opIndex++] = i;
        }
    }
    if (opIndex == 1) {
        for (int i = 0; i < operatorPositions[0]; i++) {
            firstFactor += pow(10, i) * (eq[operatorPositions[0] - i - 1] - '0');
        }
        for (int i = 0; i < EQUATION_SIZE - operatorPositions[0] - 1; i++) {
            secondFactor += pow(10, i) * (eq[EQUATION_SIZE - i - 1] - '0');
        }
        switch (operators[0]) {
        case '+':
            return firstFactor + secondFactor;
        case '-':
            return firstFactor - secondFactor;
        case '*':
            return firstFactor * secondFactor;
        case '/':
            return firstFactor / secondFactor;
        default:
            return INT_MIN;
        }
    }
    else {
        double thirdFactor = 0;
        for (int i = 0; i < operatorPositions[0]; i++) {
            firstFactor += pow(10, i) * (eq[operatorPositions[0] - i - 1] - '0');
        }
        for (int i = 0; i < operatorPositions[1] - operatorPositions[0] - 1; i++) {
            secondFactor += pow(10, i) * (eq[operatorPositions[1] - i - 1] - '0');
        }
        for (int i = 0; i < EQUATION_SIZE - operatorPositions[1] - 1; i++) {
            thirdFactor += pow(10, i) * (eq[EQUATION_SIZE - i - 1] - '0');
        }
        switch (operators[0]) {
        case '+':
            switch (operators[1]) {
            case '+':
                return firstFactor + secondFactor + thirdFactor;
            case '-':
                return firstFactor + secondFactor - thirdFactor;
            case '*':
                return firstFactor + secondFactor * thirdFactor;
            case '/':
                return firstFactor + secondFactor / thirdFactor;
            }
            break;
        case '-':
            switch (operators[1]) {
            case '+':
                return firstFactor - secondFactor + thirdFactor;
            case '-':
                return firstFactor - secondFactor - thirdFactor;
            case '*':
                return firstFactor - secondFactor * thirdFactor;
            case '/':
                return firstFactor - secondFactor / thirdFactor;
            }
            break;
        case '*':
            switch (operators[1]) {
            case '+':
                return firstFactor * secondFactor + thirdFactor;
            case '-':
                return firstFactor * secondFactor - thirdFactor;
            case '*':
                return firstFactor * secondFactor * thirdFactor;
            case '/':
                return firstFactor * secondFactor / thirdFactor;
            }
            break;
        case '/':
            switch (operators[1]) {
            case '+':
                return firstFactor / secondFactor + thirdFactor;
            case '-':
                return firstFactor / secondFactor - thirdFactor;
            case '*':
                return firstFactor / secondFactor * thirdFactor;
            case '/':
                return firstFactor / secondFactor / thirdFactor;
            }
            break;
        }
    }
}

int checkCorrectness(int eqNumId, char* eq, int desiredResult) {
    if (!hasAptLength(eqNumId)) {
        return 1;
    }
    if (hasForbiddenChars(eq)) {
        return 2;
    }
    if (hasTrailingZeros(eq)) {
        return 3;
    }
    if (hasDivZero(eq)) {
        return 4;
    }
    if (hasWrongConstruction(eq)) {
        return 5;
    }
    if (betterCalcResult(eq) != (double)desiredResult) {
        return 6;
    }
    return 0;
}