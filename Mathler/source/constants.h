#ifndef CONSTANTS_H
#define CONSTANTS_H

#define EQUATION_SIZE 6
#define NUMBER_OF_ATTEMPTS 6
#define green al_map_rgb(34,197,94) // Allegro colors 
#define yellow al_map_rgb(234,179,8)
#define grey al_map_rgb(148,163,184)
#define lightgrey al_map_rgb(226,232,240)

typedef
struct general_equation {
    char equation[EQUATION_SIZE];
    int result, type;
} eq; // Struct for both the equation string, its result and its type (one operator or two)
enum equationTypes { ONE_OP, TWO_OPS };
enum opTypes { ADD, SUB, MUL, DIV };

#endif // CONSTANTS_H