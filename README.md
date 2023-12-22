# Sudoku

Sudoku puzzle solver with two modes.

**Input:**  This algorithm reads input from a file formatted as such 
2

0 2 4 0 0 7 0 0 0

6 0 0 0 0 0 0 0 0

...
where the first value represents the mode and the 0's in the puzzle are values to solve for. 

**Mode 1:** backtracking algorithm 
Finds all possible values in a given cell and systematically checks if the new state is a "promising" puzzle, if so algorithm will continue to next cell, and if not algorithm will "back track" to the next possible value.


**Mode 2:**  optimiation algorithm using simulated annealing

Overview: chooses a random state, makes a stochastic move, has an acceptance probability - note that state gets greedier as times goes on and must select a starting temperature and cooling rate.Temperature selected by taking sd of the costs of 200 random starting sudoku boards. Cooling rate is 0.001.


Process: system generates random state (sudoku board), choose random 2 cells in the same 3x3 grid and swaps them. Cost function is calculated for the before and after. We use the cost function to determine if this new state was better than the old state (cost function = duplicates in rows and columns). Intially, as long as cost of new state is less than current state, system will prefer new state. Over time, the algorithm becomes greedier. 
