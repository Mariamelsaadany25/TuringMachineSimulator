-# TuringMachineSimulator
A simple Turing Machine simulator written in C.

## Features

- Initialize the Turing Machine with states, alphabet, initial state, tape, and transitions.
- Run the Turing Machine and display the tape at each step.
- Encode the Turing Machine for easy visualization.
 ## Examples Usage

- Please Enter number of states: 2
- Enter alphabet (comma separated --> Like (a,b,...), Without # ): a,b
- Enter initial state -->(q0): q0
- Enter tape contents: abb##ab
- Enter head position: 0
- Enter blank symbol: #
- Enter number of transitions: 6
- Enter transitions --->(currentState,currentInputSymbol,newState,newSymbol,action (R,L,Y,N)) e.g{FROMAT q0,a,q0,#,R}:
- q0,a,q0,#,R
- q0,b,q0,#,R
- q0,#,q1,#,R
- q1,a,q1,#,R
- q1,b,q1,#,R
- q1,#,q1,#,Y
