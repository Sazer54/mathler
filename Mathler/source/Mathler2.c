#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h" // Constants used throughout the project
#include "eqGenerator.h" // Equation generation and randomisation
#include "errorChecker.h" // User input error checking

// Checks if user input matches the generated equation
int isCorrect(char* colors) {
    for (int i = 0; i < EQUATION_SIZE; i++) {
        if (colors[i] == 'G') {
            return 0;
        }
    }
    return 1;
}

int main()
{  
    int initialize = 1; // Toggles game start/restart within main game loop
    int gameRunning = 1; // Toggles main game loop

    eq equation, userInput; // The generated equation and the one user inputs
    char* findEqMessage = malloc(100*sizeof(char)); // Message displayed in the beginning
    char equationSigns[4] = { '+', '-', '*', '/' };
    
    // Necessary functions to initiate Allegro
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
    al_init_primitives_addon();

	ALLEGRO_DISPLAY* display = al_create_display(640, 640);
	ALLEGRO_FONT* font = al_load_ttf_font("Zag Regular.otf", 32, 0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_BITMAP* bitmap = NULL;

    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    eq testUserInputs[NUMBER_OF_ATTEMPTS]; // Array of user-input equations
    int userInputIndex, equationsNumberIndex, gameFinished, occurrencesInResult, occurrecenesSoFar, cheatingEnabled;
    char* errorMessage = malloc(sizeof(char)*100); // Message displayed on error in user input
    char* victoryMessage = malloc(sizeof(char) * 100); // Message displayed on victory
    char* currentChar = malloc(sizeof(char)); // Helper with accessing the file data
    char* numbersColorCues = malloc(sizeof(char) * 10); // Colors of the number blocks
    char* signsColorCues = malloc(sizeof(char) * 4); // Colors of the sign blocks
    char colors2[NUMBER_OF_ATTEMPTS][EQUATION_SIZE]; // Colors of the rectangles
    char* tempUserInput = malloc(sizeof(char) * 100); // Helper with accessing the file data

    int readFromFile = 1; // Read data from file or generate new data
    int hasRecentlyWon; // After winning and closing, new data gets generated

    // Save file handling
    FILE* saveFile = fopen("savefile.txt", "r");
    if (saveFile == NULL) {
        printf("File could not be opened");
        readFromFile = 0;
    }
    fscanf(saveFile, "%d\n", &hasRecentlyWon);
    readFromFile = !hasRecentlyWon;

	while (gameRunning) { // Main game loop
        if (initialize) {
            if (readFromFile) {
                fgets(equation.equation, EQUATION_SIZE+1, saveFile);  // Reading the equation from file
                equation.result = betterCalcResult(equation.equation);

                fscanf(saveFile, "%d\n", &equationsNumberIndex); // Reading the currently edited equation's index from file

                for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) { // Reading all equations input so far from file
                    fgets(tempUserInput, EQUATION_SIZE + 2, saveFile);
                    if (tempUserInput[0] != 'n') {
                        sprintf(testUserInputs[i].equation, "%s", tempUserInput);
                        //printf("%s", testUserInputs[i].equation);
                    }                 
                }

                for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) { // Reading rectangle colors from file
                    fgets(tempUserInput, EQUATION_SIZE + 2, saveFile);
                    if (tempUserInput[0] != 'n') {
                        sprintf(colors2[i], "%s", tempUserInput);
                    }                   
                }
                fgets(numbersColorCues, 12, saveFile); // Reading number blocks colors from file
                fgets(signsColorCues, 6, saveFile); // Reading sign blocks colros from file

                readFromFile = 0;
                fclose(saveFile);
            }
            else {               
                equation = generateEquation(); // Generating the equation
                equationsNumberIndex = 0;// Index of the first equation to fill in

                for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) { // Clearing every user input
                    for (int j = 0; j < EQUATION_SIZE; j++) {
                        testUserInputs[i].equation[j] = '\0';
                    }
                }

                for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) {// Clearing every rectangle color
                    for (int j = 0; j < EQUATION_SIZE; j++) {
                        colors2[i][j] = '\0';
                    }                   
                }

                for (int i = 0; i < 10; i++) { // Clearing every number block color
                    numbersColorCues[i] = 'u';
                }
                for (int i = 0; i < 4; i++) { // Clearing every sign block color
                    signsColorCues[i] = 'u';
                }
            }
                   
            sprintf(findEqMessage, "Find the hidden equation that equals %d", equation.result);
            sprintf(victoryMessage, "");

            // Initialising necessary variables
            cheatingEnabled = 0;
            userInputIndex = 0;
            gameFinished = 0;
            occurrencesInResult = 0;
            occurrecenesSoFar = 0;
            initialize = 0;
        }
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        ALLEGRO_KEYBOARD_STATE keyState;
        al_get_keyboard_state(&keyState);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            gameRunning = 0;
        }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_text(font, al_map_rgb(0, 0, 0), al_get_display_width(display)/2, 20, 
                ALLEGRO_ALIGN_CENTRE, findEqMessage);  //Printing welcome message

            for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) { // Printing the rectangles and numbers within
                for (int j = 0; j < EQUATION_SIZE; j++) {
                    if (i < equationsNumberIndex || (gameFinished && i == equationsNumberIndex)) {
                        al_draw_filled_rectangle(85 + (80 * j), 80 + (55 * i), 155 + (80 * j), 125 + (55 * i),
                            colors2[i][j] == 'Z' ? green : 
                            (colors2[i][j] == 'Y' ? yellow : grey));
                    }
                    else {
                        al_draw_rectangle(85 + (80 * j), 80 + (55 * i), 155 + (80 * j), 125 + (55 * i),
                            al_map_rgb(0, 0, 0), i == equationsNumberIndex ? 3 : 0);
                    }                    
                    sprintf(currentChar, "%c ", testUserInputs[i].equation[j]);
                    al_draw_text(font, (i != equationsNumberIndex || i == equationsNumberIndex && gameFinished) ? al_map_rgb(255, 255, 255) : al_map_rgb(0, 0, 0), 45 + 80 * (j + 1), 90 + 55 * i,
                        ALLEGRO_ALIGN_CENTRE, currentChar);
                }
            }
            
            for (int i = 0; i <= 9; i++) {  // Printing number blocks
                al_draw_filled_rectangle(57 + 45 * (i + 1), 435, 90 + 45 * (i + 1), 490, 
                    numbersColorCues[i] == 'Z' ? green : 
                    numbersColorCues[i] == 'Y' ? yellow : 
                    numbersColorCues[i] == 'G' ? grey : lightgrey);
                sprintf(currentChar, "%c ", i + '0');
                al_draw_text(font, numbersColorCues[i] == 'u' ? al_map_rgb(0, 0, 0) : al_map_rgb(255,255,255), 80 + 45 * (i + 1), 450,
                    ALLEGRO_ALIGN_CENTRE, currentChar);
            }
            for (int i = 0; i < 4; i++) { // Printing sign blocks
                al_draw_filled_rectangle(192 + 45 * (i + 1), 505, 225 + 45 * (i + 1), 560, 
                    signsColorCues[i] == 'Z' ? green :
                    signsColorCues[i] == 'Y' ? yellow :
                    signsColorCues[i] == 'G' ? grey : lightgrey);
                sprintf(currentChar, "%c ", equationSigns[i]);
                al_draw_text(font, signsColorCues[i] == 'u' ? al_map_rgb(0, 0, 0) : al_map_rgb(255, 255, 255), 214 + 45 * (i + 1), 520,
                    ALLEGRO_ALIGN_CENTRE, currentChar);
            }

            //Pritning error/victory/cheat messages
            al_draw_text(font, al_map_rgb(255, 0, 0), al_get_display_width(display) / 2, 
                al_get_display_height(display) - 50, ALLEGRO_ALIGN_CENTRE, errorMessage);
            if (cheatingEnabled) {
                al_draw_text(font, al_map_rgb(0, 0, 255), al_get_display_width(display) / 2,
                    al_get_display_height(display) - 50, ALLEGRO_ALIGN_CENTRE, equation.equation);
            }            
            al_draw_text(font, al_map_rgb(0, 255, 0), al_get_display_width(display) / 2,
                al_get_display_height(display) - 50, ALLEGRO_ALIGN_CENTRE, victoryMessage);
            al_flip_display();
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (gameFinished) {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_Q: // Quit
                    saveFile = fopen("savefile.txt", "w");
                    fprintf(saveFile, "%d\n", 1);
                    fclose(saveFile);
                    gameRunning = 0;
                    break;
                case ALLEGRO_KEY_R: // Replay
                    initialize = 1;
                    break;
                default:
                    break;
                }
            }
            else {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_Q: // Quit and save current progress
                    saveFile = fopen("savefile.txt", "w");
                    fprintf(saveFile, "%d\n", 0);
                    for (int i = 0; i < EQUATION_SIZE; i++) {
                        fprintf(saveFile, "%c", equation.equation[i]);
                    }
                    fprintf(saveFile, "\n");
                    fprintf(saveFile, "%d\n", equationsNumberIndex);
                    for (int i = 0; i < equationsNumberIndex; i++) {                    
                        for (int j = 0; j < EQUATION_SIZE; j++) {
                            fprintf(saveFile, "%c", testUserInputs[i].equation[j]);
                        }
                        fprintf(saveFile, "\n");
                    }
                    for (int i = equationsNumberIndex; i < NUMBER_OF_ATTEMPTS; i++) {
                        fprintf(saveFile, "nnnnnn\n");
                    }
                    for (int i = 0; i < equationsNumberIndex; i++) {
                        for (int j = 0; j < EQUATION_SIZE; j++) {
                            fprintf(saveFile, "%c", colors2[i][j]);
                        }
                        fprintf(saveFile, "\n");
                    }
                    for (int i = equationsNumberIndex; i < NUMBER_OF_ATTEMPTS; i++) {
                        fprintf(saveFile, "nnnnnn\n");
                    }
                    for (int i = 0; i < 10; i++) {
                        fprintf(saveFile, "%c", numbersColorCues[i]);
                    }
                    fprintf(saveFile, "\n");
                    for (int i = 0; i < 4; i++) {
                        fprintf(saveFile, "%c", signsColorCues[i]);
                    }
                    fprintf(saveFile, "\n");
                    fclose(saveFile);
                    gameRunning = 0;
                    break;
                case ALLEGRO_KEY_R: // Generate new equation
                    initialize = 1;
                    break;
                case ALLEGRO_KEY_C: // See the generated equation on the bottom of the screen
                    sprintf(errorMessage, "");
                    cheatingEnabled = cheatingEnabled ? 0 : 1;
                    break;
                case ALLEGRO_KEY_PAD_0: // Inserting user input into an array
                case ALLEGRO_KEY_0:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '0';
                    }
                    break;
                case ALLEGRO_KEY_PAD_1:
                case ALLEGRO_KEY_1:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '1';
                    }
                    break;
                case ALLEGRO_KEY_PAD_2:
                case ALLEGRO_KEY_2:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '2';
                    }
                    break;
                case ALLEGRO_KEY_PAD_3:
                case ALLEGRO_KEY_3:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '3';
                    }
                    break;
                case ALLEGRO_KEY_PAD_4:
                case ALLEGRO_KEY_4:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '4';
                    }
                    break;
                case ALLEGRO_KEY_PAD_5:
                case ALLEGRO_KEY_5:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '5';
                    }
                    break;
                case ALLEGRO_KEY_PAD_6:
                case ALLEGRO_KEY_6:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '6';
                    }
                    break;
                case ALLEGRO_KEY_PAD_7:
                case ALLEGRO_KEY_7:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '7';
                    }
                    break;
                case ALLEGRO_KEY_PAD_8:
                case ALLEGRO_KEY_8:
                    if (userInputIndex < EQUATION_SIZE) {
                        if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) {
                            testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '*';
                        }
                        else {
                            testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '8';
                        }
                    }
                    break;
                case ALLEGRO_KEY_PAD_9:
                case ALLEGRO_KEY_9:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '9';
                    }
                    break;
                case ALLEGRO_KEY_PAD_PLUS:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '+';
                    }
                    break;
                case ALLEGRO_KEY_EQUALS:
                    if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT) && userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '+';
                    }
                    break;
                case ALLEGRO_KEY_PAD_MINUS:
                case ALLEGRO_KEY_MINUS:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '-';
                    }
                    break;
                case ALLEGRO_KEY_PAD_SLASH:
                case ALLEGRO_KEY_SLASH:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '/';
                    }
                    break;
                case ALLEGRO_KEY_PAD_ASTERISK:
                    if (userInputIndex < EQUATION_SIZE) {
                        testUserInputs[equationsNumberIndex].equation[userInputIndex++] = '*';
                    }
                    break;
                case ALLEGRO_KEY_BACKSPACE:
                    if (userInputIndex > 0) {
                        testUserInputs[equationsNumberIndex].equation[--userInputIndex] = '\0';
                    }
                    break;
                case ALLEGRO_KEY_PAD_ENTER:
                case ALLEGRO_KEY_ENTER:
                    switch (checkCorrectness(userInputIndex, testUserInputs[equationsNumberIndex].equation,
                        equation.result)) // Checking for errors in user input
                    {
                    case 0: // No errors were found
                        for (int i = 0; i < EQUATION_SIZE; i++) { // Detecting green elements
                            if (testUserInputs[equationsNumberIndex].equation[i] == equation.equation[i]) {
                                colors2[equationsNumberIndex][i] = 'Z'; // Coloring the rectangle green
                                if (testUserInputs[equationsNumberIndex].equation[i] >= '0' &&
                                    testUserInputs[equationsNumberIndex].equation[i] <= '9') { 
                                    // Coloring the number block green
                                    numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] = 'Z';
                                }
                                else { // Coloring the sign block green
                                    switch (testUserInputs[equationsNumberIndex].equation[i]) {
                                    case '+':
                                        signsColorCues[0] = 'Z';
                                        break;
                                    case '-':
                                        signsColorCues[1] = 'Z';
                                        break;
                                    case '*':
                                        signsColorCues[2] = 'Z';
                                        break;
                                    case '/':
                                        signsColorCues[3] = 'Z';
                                        break;
                                    default:
                                        break;
                                    }
                                }                             
                            }
                        }
                        for (int i = 0; i < EQUATION_SIZE; i++) { // Detecting yellow and grey elements                           
                            if (testUserInputs[equationsNumberIndex].equation[i] != equation.equation[i]) {
                                occurrencesInResult = 0;
                                occurrecenesSoFar = 0;
                                for (int j = 0; j < EQUATION_SIZE; j++) {
                                    if (testUserInputs[equationsNumberIndex].equation[i] == equation.equation[j]) {
                                        occurrencesInResult++;
                                    }
                                }
                                for (int j = 0; j < EQUATION_SIZE; j++) {
                                    if ((testUserInputs[equationsNumberIndex].equation[i] == 
                                        testUserInputs[equationsNumberIndex].equation[j]) && 
                                        (colors2[equationsNumberIndex][j] == 'Z' || 
                                            colors2[equationsNumberIndex][j] == 'Y')) {
                                        occurrecenesSoFar++;
                                    }
                                }
                                if (occurrencesInResult - occurrecenesSoFar > 0) {
                                    colors2[equationsNumberIndex][i] = 'Y';
                                    if (testUserInputs[equationsNumberIndex].equation[i] >= '0' &&
                                        testUserInputs[equationsNumberIndex].equation[i] <= '9') {
                                        if (numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] != 'Z') {
                                            numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] = 'Y';
                                        }
                                    }
                                    else {
                                        switch (testUserInputs[equationsNumberIndex].equation[i]) {
                                        case '+':
                                            signsColorCues[0] = signsColorCues[0] == 'Z' ? 'Z' : 'Y';
                                            break;
                                        case '-':
                                            signsColorCues[1] = signsColorCues[1] == 'Z' ? 'Z' : 'Y';
                                            break;
                                        case '*':
                                            signsColorCues[2] = signsColorCues[2] == 'Z' ? 'Z' : 'Y';
                                            break;
                                        case '/':
                                            signsColorCues[3] = signsColorCues[3] == 'Z' ? 'Z' : 'Y';
                                            break;
                                        default:
                                            break;
                                        }
                                    }                                                                     
                                }
                                else {
                                    colors2[equationsNumberIndex][i] = 'G';
                                    if (testUserInputs[equationsNumberIndex].equation[i] >= '0' &&
                                        testUserInputs[equationsNumberIndex].equation[i] <= '9') {
                                        if (numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] != 'Z' &&
                                            numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] != 'Y') {
                                            numbersColorCues[testUserInputs[equationsNumberIndex].equation[i] - '0'] = 'G';
                                        }                                   
                                    }
                                    switch (testUserInputs[equationsNumberIndex].equation[i]) {
                                    case '+':
                                        signsColorCues[0] = (signsColorCues[0] == 'Z' || signsColorCues[0] == 'Y') ? signsColorCues[0] : 'G';
                                        break;
                                    case '-':
                                        signsColorCues[1] = (signsColorCues[1] == 'Z' || signsColorCues[1] == 'Y') ? signsColorCues[1] : 'G';
                                        break;
                                    case '*':
                                        signsColorCues[2] = (signsColorCues[2] == 'Z' || signsColorCues[2] == 'Y') ? signsColorCues[2] : 'G';
                                        break;
                                    case '/':
                                        signsColorCues[3] = (signsColorCues[3] == 'Z' || signsColorCues[3] == 'Y') ? signsColorCues[3] : 'G';
                                        break;
                                    default:
                                        break;
                                    }
                                }
                            }
                        }
                        if (isCorrect(colors2[equationsNumberIndex])) { // Checking for victory
                            for (int i = 0; i < EQUATION_SIZE; i++) {
                                colors2[equationsNumberIndex][i] = 'Z';
                            }
                            sprintf(errorMessage, "");
                            sprintf(victoryMessage, "You won!\nq - exit\nr - play again\n");                      
                            gameFinished = 1;
                            cheatingEnabled = 0;
                        }
                        else {
                            if (equationsNumberIndex == NUMBER_OF_ATTEMPTS - 1) { // Game over
                                sprintf(errorMessage, "Game over - the answer was: %s",
                                    equation.equation);
                                gameFinished = 1;
                            }
                            else {
                                strcpy(errorMessage, "");
                                equationsNumberIndex++;
                                userInputIndex = 0;
                            }                            
                        }
                        break;
                    case 1: // Error - Invalid input length
                        sprintf(errorMessage, "Invalid input length, must be %d.", EQUATION_SIZE);
                        cheatingEnabled = 0;
                        break;
                    case 2: // Error - Forbidden characters
                        sprintf(errorMessage, "Forbidden characters detected.");
                        cheatingEnabled = 0;
                        break;
                    case 3: // Error - Trailing zeroes
                        sprintf(errorMessage, "Trailing zeroes detected.");
                        cheatingEnabled = 0;
                        break;
                    case 4: // Error - division by zero
                        sprintf(errorMessage, "Division by zero detected.");
                        cheatingEnabled = 0;
                        break;
                    case 5: // Error - wrong equation construction
                        sprintf(errorMessage, "Wrong equation construction.");
                        cheatingEnabled = 0;
                        break;
                    case 6: // Error - doesn't calculate to desired result
                        sprintf(errorMessage, "Must be equal to %d", equation.result);
                        cheatingEnabled = 0;
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
        }
	}
    
    // Cleaning up after Allegro
	al_destroy_display(display);
	al_destroy_font(font);
    al_uninstall_keyboard();
    al_destroy_timer(timer);
}

