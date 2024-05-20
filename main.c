#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 100
#define MAX_TRANSITIONS 100

struct TuringMachine {
    int numStates;
    char alphabet[MAX_ALPHABET][10];
    int numAlphabet;
    char initialState[10];
    char tape[1000];
    int headPosition;
    char transitions[MAX_TRANSITIONS][5][10];
    int numTransitions;
    char blankSymbol[10];
};

void initializeTuringMachine(struct TuringMachine *tm) {
    printf("Please Enter number of states: ");
    scanf("%d", &tm->numStates);

    printf("Enter alphabet (comma separated --> Like (a,b,...), Without # ): ");
    char alphabetInput[MAX_ALPHABET * 10];
    scanf("%s", alphabetInput);

    tm->numAlphabet = 0;
    char* token = strtok(alphabetInput, ",");
    while (token != NULL) {
        strcpy(tm->alphabet[tm->numAlphabet], token);
        tm->numAlphabet++;
        token = strtok(NULL, ",");
    }

    printf("Enter initial state -->(q0): ");
    scanf("%s", tm->initialState);

    printf("Enter tape contents: ");
    scanf("%s", tm->tape);

    printf("Enter head position: ");
    scanf("%d", &tm->headPosition);

    printf("Enter blank symbol: ");
    scanf("%s", tm->blankSymbol);

    printf("Enter number of transitions: ");
    scanf("%d", &tm->numTransitions);

    printf("Enter transitions --->(currentState,currentInputSymbol,newState,newSymbol,action (R,L,Y,N)) e.g{FROMAT q0,a,q0,#,R}:\n");
    for (int i = 0; i < tm->numTransitions; i++) {
        char transitionInput[100];
        scanf("%s", transitionInput);
        sscanf(transitionInput, "%[^,],%[^,],%[^,],%[^,],%[^,]",
               tm->transitions[i][0], // currentState
               tm->transitions[i][1], // currentSymbol
               tm->transitions[i][2], // newState
               tm->transitions[i][3], // newSymbol
               tm->transitions[i][4]  // action
        );
    }
}

void displayTape(const struct TuringMachine *tm) {
    printf("Tape: ");
    for (int i = 0; i < strlen(tm->tape); i++) {
        if (i == tm->headPosition) {
            printf("[%c]", tm->tape[i]);
        } else {
            printf(" %c ", tm->tape[i]);
        }
    }
    printf("\n");
}

void run(struct TuringMachine *tm) {
    int steps = 0;
    int tapeLength = strlen(tm->tape);
    char currentState[10];
    strcpy(currentState, tm->initialState);

    while (steps < 1000) {
        char currentSymbol = tm->tape[tm->headPosition];
        int foundTransition = 0;

        printf("Step %d:\n", steps);
        printf("Current state: %s\n", currentState);
        displayTape(tm);

        for (int i = 0; i < tm->numTransitions; i++) {

            if (strcmp(tm->transitions[i][0], currentState) == 0 &&
                (tm->transitions[i][1][0] == currentSymbol ||
                 (tm->transitions[i][1][0] == '#' && currentSymbol == tm->blankSymbol[0]))) {
                printf("Applying transition: (%s, %c) -> (%s, %c, %c)\n",
                       tm->transitions[i][0], tm->transitions[i][1][0],
                       tm->transitions[i][2], tm->transitions[i][3][0],
                       tm->transitions[i][4][0]);

                tm->tape[tm->headPosition] = tm->transitions[i][3][0];
                strcpy(currentState, tm->transitions[i][2]);
                if (tm->transitions[i][4][0] == 'R') { // move Right
                    tm->headPosition++;
                    if (tm->headPosition == tapeLength) {
                        tm->tape[tm->headPosition] = tm->blankSymbol[0]; // Extend tape with blank
                        tapeLength++;
                    }
                } else if (tm->transitions[i][4][0] == 'L') { // move Left
                    tm->headPosition--;
                    if (tm->headPosition < 0) {
                        memmove(&tm->tape[1], tm->tape, strlen(tm->tape) + 1);
                        tm->tape[0] = tm->blankSymbol[0];
                        tm->headPosition = 0;
                        tapeLength++;
                    }
                } else if (tm->transitions[i][4][0] == 'Y') { // Accept
                    printf("String Accepted!\n");
                    return;
                } else if (tm->transitions[i][4][0] == 'N') { // Reject
                    printf("String Not Accepted!\n");
                    return;
                }
                foundTransition = 1;
                break;
            }
        }

        if (!foundTransition) {
            break;
        }

        steps++;
    }

    printf("Final tape: %s\n", tm->tape);
}

void encodeTM(struct TuringMachine *tm) {
    char* states[MAX_STATES];
    char* symbols[MAX_ALPHABET];
    char* actions[4] = {"1", "11", "111", "1111"}; // R, L, Y, N


    for (int i = 0; i < tm->numStates; i++) {
        states[i] = (char*)malloc((i + 2) * sizeof(char));
        memset(states[i], '1', i + 1);
        states[i][i + 1] = '\0';
    }


    for (int i = 0; i < tm->numAlphabet; i++) {
        symbols[i] = (char*)malloc((i + 2) * sizeof(char));
        memset(symbols[i], '1', i + 1);
        symbols[i][i + 1] = '\0';
    }


    symbols[tm->numAlphabet] = (char*)malloc((tm->numAlphabet + 2) * sizeof(char));//For encoding blank Symbol
    memset(symbols[tm->numAlphabet], '1', tm->numAlphabet + 1);
    symbols[tm->numAlphabet][tm->numAlphabet + 1] = '\0';

    // Mapping States name for easy encoding
    char* stateMap[MAX_STATES][2];
    for (int i = 0; i < tm->numStates; i++) {
        stateMap[i][0] = (char*)malloc(10 * sizeof(char));
        stateMap[i][1] = states[i];
        sprintf(stateMap[i][0], "q%d", i);
    }

    printf("Encoded TM transitions:\n");
    for (int i = 0; i < tm->numTransitions; i++) {
        char* currentState = NULL;
        char* currentSymbol = NULL;
        char* newState = NULL;
        char* newSymbol = NULL;

        // find current state
        for (int j = 0; j < tm->numStates; j++) {
            if (strcmp(tm->transitions[i][0], stateMap[j][0]) == 0) {
                currentState = stateMap[j][1];
                break;
            }
        }

        // find current symbol
        for (int j = 0; j < tm->numAlphabet; j++) {
            if (tm->transitions[i][1][0] == tm->alphabet[j][0]) {
                currentSymbol = symbols[j];
                break;
            }
        }
        if (currentSymbol == NULL && tm->transitions[i][1][0] == tm->blankSymbol[0]) {
            currentSymbol = symbols[tm->numAlphabet];
        }

        //find new state
        for (int j = 0; j < tm->numStates; j++) {
            if (strcmp(tm->transitions[i][2], stateMap[j][0]) == 0) {
                newState = stateMap[j][1];
                break;
            }
        }

        // Find the new symbol
        for (int j = 0; j < tm->numAlphabet; j++) {
            if (tm->transitions[i][3][0] == tm->alphabet[j][0]) {
                newSymbol = symbols[j];
                break;
            }
        }
        if (newSymbol == NULL && tm->transitions[i][3][0] == tm->blankSymbol[0]) {
            newSymbol = symbols[tm->numAlphabet];
        }

        char action = tm->transitions[i][4][0];
        char* actionEncoded = actions[0]; // R=1
        if (action == 'L') {
            actionEncoded = actions[1]; // L=11
        } else if (action == 'Y') {
            actionEncoded = actions[2]; // Y=111
        } else if (action == 'N') {
            actionEncoded = actions[3]; // N=1111
        }


        printf("%s0%s0%s0%s0%s", currentState, currentSymbol, newState, newSymbol, actionEncoded);

        // separate the transition functions by 00 except the last one
        if (i < tm->numTransitions - 1) {
            printf("00");
        }
    }
    printf("\n");

    // Free allocated memory
    for (int i = 0; i < tm->numStates; i++) {
        free(states[i]);
        free(stateMap[i][0]);
    }
    for (int i = 0; i < tm->numAlphabet + 1; i++) {
        free(symbols[i]);
    }
}

int main() {
    struct TuringMachine tm;
    initializeTuringMachine(&tm);
    run(&tm);
    encodeTM(&tm);
    return 0;
}
//  Some examples
// Example:1 erase till second blank
/*Please Enter number of states: 2
Enter alphabet (comma separated --> Like (a,b,...), Without # ): a,b
Enter initial state -->(q0): q0
Enter tape contents: abb##ab
Enter head position: 0
Enter blank symbol: #
Enter number of transitions: 6
Enter transitions --->(currentState,currentInputSymbol,newState,newSymbol,action (R,L,Y,N)) e.g{FROMAT q0,a,q0,#,R}:
q0,a,q0,#,R
q0,b,q0,#,R
q0,#,q1,#,R
q1,a,q1,#,R
q1,b,q1,#,R
q1,#,q1,#,Y
Step 0:
Current state: q0
Tape: [a] b  b  #  #  a  b
Applying transition: (q0, a) -> (q0, #, R)
Step 1:
Current state: q0
Tape:  # [b] b  #  #  a  b
Applying transition: (q0, b) -> (q0, #, R)
Step 2:
Current state: q0
Tape:  #  # [b] #  #  a  b
Applying transition: (q0, b) -> (q0, #, R)
Step 3:
Current state: q0
Tape:  #  #  # [#] #  a  b
Applying transition: (q0, #) -> (q1, #, R)
Step 4:
Current state: q1
Tape:  #  #  #  # [#] a  b
Applying transition: (q1, #) -> (q1, #, Y)
String Accepted!
Encoded TM transitions:
1010101110100101101011101001011101101110100110101101110100110110110111010011011101101110111*/

//Example 2:
/*Please Enter number of states: 3
Enter alphabet (comma separated --> Like (a,b,...), Without # ): a,b
Enter initial state -->(q0): q0
Enter tape contents: abb
Enter head position: 0
Enter blank symbol: #
Enter number of transitions: 6
Enter transitions --->(currentState,currentInputSymbol,newState,newSymbol,action (R,L,Y,N)) e.g{FROMAT q0,a,q0,#,R}:
q0,a,q0,a,R
q0,b,q0,b,R
q0,#,q1,#,R
q1,a,q1,a,R
q1,b,q1,b,R
q1,#,q2,#,Y
Step 0:
Current state: q0
Tape: [a] b  b
Applying transition: (q0, a) -> (q0, a, R)
Step 1:
Current state: q0
Tape:  a [b] b
Applying transition: (q0, b) -> (q0, b, R)
Step 2:
Current state: q0
Tape:  a  b [b]
Applying transition: (q0, b) -> (q0, b, R)
Step 3:
Current state: q0
Tape:  a  b  b [#]
Applying transition: (q0, #) -> (q1, #, R)
Step 4:
Current state: q1
Tape:  a  b  b  # [#]
Applying transition: (q1, #) -> (q2, #, Y)
String Accepted!
Encoded TM transitions:
10101010100101101011010010111011011101001101011010100110110110110100110111011101110111*/
