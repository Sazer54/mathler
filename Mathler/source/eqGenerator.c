#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "eqGenerator.h"


int* getDivisors(int n, int digitNum) {
    int* divisors = malloc(sizeof(int) * (n + 1));
    int currentIndex = 1;
    for (int i = pow(10, digitNum - 1); i < pow(10, digitNum); i++) {
        if (n % i == 0) {
            divisors[currentIndex++] = i;
        }
    }
    divisors[0] = currentIndex - 1;
    return divisors;
}

void fillEquation1(eq* eq, int ff, int sf, int r, char op) {
    eq->equation[2] = ff % 10 + '0';
    ff /= 10;
    eq->equation[1] = ff % 10 + '0';
    ff /= 10;
    eq->equation[0] = ff + '0';
    eq->equation[3] = op;
    eq->equation[5] = sf % 10 + '0';
    sf /= 10;
    eq->equation[4] = sf + '0';
    eq->result = r;
    eq->type = ONE_OP;
}

void fillEquation2(eq* eq, int ff, int sf, int tf, int r, char op1, char op2) {
    eq->equation[1] = ff % 10 + '0';
    ff /= 10;
    eq->equation[0] = ff + '0';
    eq->equation[2] = op1;
    eq->equation[3] = sf + '0';
    eq->equation[4] = op2;
    eq->equation[5] = tf + '0';
    eq->result = r;
    eq->type = TWO_OPS;
}

int areEqual(eq eq1, eq eq2) {
    for (int i = 0; i < EQUATION_SIZE; i++) {
        if (eq1.equation[i] != eq2.equation[i]) {
            return 0;
        }
    }
    return 1;
}

eq generateEquation() {
    srand(time(NULL));
    int eqType = rand() % 2;
    int result;
    int* divList;
    eq equation;
    if (eqType == ONE_OP) { // case for when there is only one operator within the equation
        int opType = rand() % 4;
        int firstFactor, secondFactor;
        firstFactor = rand() % 899 + 100;
        secondFactor = rand() % 89 + 10;
        switch (opType) {
        case ADD:
            result = firstFactor + secondFactor;
            fillEquation1(&equation, firstFactor, secondFactor, result, '+');
            break;
        case SUB:
            result = firstFactor - secondFactor;
            fillEquation1(&equation, firstFactor, secondFactor, result, '-');
            break;
        case MUL:
            result = firstFactor * secondFactor;
            fillEquation1(&equation, firstFactor, secondFactor, result, '*');
            break;
        case DIV:
            do {
                firstFactor = rand() % 899 + 100;
                divList = getDivisors(firstFactor, 2);
            } while (divList[0] == 0);
            secondFactor = divList[rand() % divList[0] + 1];
            result = firstFactor / secondFactor;
            fillEquation1(&equation, firstFactor, secondFactor, result, '/');
            break;
        default:
            printf("Something went wrong...\n");
        }
        printf("%c%c%c %c %c%c = %d", equation.equation[0], equation.equation[1], equation.equation[2],
            equation.equation[3], equation.equation[4], equation.equation[5], equation.result);
    }
    else { // case for when there are two operators within the equation
        int opType1 = rand() % 4;
        int opType2 = rand() % 4;
        double result, firstFactor, secondFactor, thirdFactor;
        firstFactor = rand() % 89 + 10;
        secondFactor = rand() % 8 + 1;
        thirdFactor = rand() % 8 + 1;
        switch (opType1) {
        case ADD:
            switch (opType2) {
            case ADD:
                result = firstFactor + secondFactor + thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '+', '+');
                break;
            case SUB:
                result = firstFactor + secondFactor - thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '+', '-');
                break;
            case MUL:
                result = firstFactor + secondFactor * thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '+', '*');
                break;
            case DIV: //sometimes divides by 0 idk why
                divList = getDivisors(secondFactor, 1);
                thirdFactor = divList[rand() % divList[0] + 1];
                result = firstFactor + secondFactor / thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '+', '/');
                break;
            }
            break;
        case SUB:
            switch (opType2) {
            case ADD:
                result = firstFactor - secondFactor + thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '-', '+');
                break;
            case SUB:
                result = firstFactor - secondFactor - thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '-', '-');
                break;
            case MUL:
                result = firstFactor - secondFactor * thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '-', '*');
                break;
            case DIV:
                divList = getDivisors(secondFactor, 1);
                thirdFactor = divList[rand() % divList[0] + 1];
                result = firstFactor - secondFactor / thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '-', '/');
                break;
            }
            break;
        case MUL:
            switch (opType2) {
            case ADD:
                result = firstFactor * secondFactor + thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '*', '+');
                break;
            case SUB:
                result = firstFactor * secondFactor - thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '*', '-');
                break;
            case MUL:
                result = firstFactor * secondFactor * thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '*', '*');
                break;
            case DIV:
                divList = getDivisors(firstFactor * secondFactor, 1);
                thirdFactor = divList[rand() % divList[0] + 1];
                result = firstFactor * secondFactor / thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '*', '/');
                break;
            }
            break;
        case DIV:
            divList = getDivisors(firstFactor, 1);
            secondFactor = divList[rand() % divList[0] + 1];
            switch (opType2) {
            case ADD:
                result = firstFactor / secondFactor + thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '/', '+');
                break;
            case SUB:
                result = firstFactor / secondFactor - thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '/', '-');
                break;
            case MUL:
                divList = getDivisors(firstFactor * thirdFactor, 1);
                secondFactor = divList[rand() % divList[0] + 1];
                result = firstFactor / secondFactor * thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '/', '*');
                break;
            case DIV:
                divList = getDivisors(firstFactor / secondFactor, 1);
                thirdFactor = divList[rand() % divList[0] + 1];
                result = firstFactor / secondFactor / thirdFactor;
                fillEquation2(&equation, firstFactor, secondFactor, thirdFactor, result, '/', '/');
                break;
            }
        }
        printf("%c%c %c %c %c %c = %d", equation.equation[0], equation.equation[1], equation.equation[2],
            equation.equation[3], equation.equation[4], equation.equation[5], equation.result);
    }
    return equation;
}
