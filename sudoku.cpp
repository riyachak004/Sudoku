//
//  sudoku.cpp
//  sudoku
//
//  Created by Riya Chakravarty on 7/5/22.
//

#include "sudoku.hpp"
#include "xcode_redirect.hpp"
#include <iostream>

using namespace std;

// REQUIRES: m (row) and n (col)
// EFFECTS: checks all permutations of possible values - backtracks if board becomes not promising

void game::genPerms(int m, int n) {
    if (m == SIZE && n == SIZE) {
        return;
    }
    
    if (!promising(m, n)) {return;}
    
    if (!sudoku[m][n].input) {
        convert_remainder(m, n);
        
        if (sudoku[m][n].possible.empty()){return;}
        
        while (!sudoku[m][n].possible.empty()){
            sudoku[m][n].val = sudoku[m][n].possible.back();
            sudoku[m][n].possible.pop_back();
            int new_n = (n + 1) % 9;
            int new_m = m;
            if (new_n == 0){
                new_m = (m + 1) % 9;
            }
            genPerms(new_m, new_n);
        }//while
    }// if
    else {
        n = (n + 1) % 9;
        if (n == 0){
            m = (m + 1) % 9;
        }
        genPerms(m,n);
    }
} // genPerms()


// MODIFIES: the sudoku cell's possible values
// EFFECTS: takes in the values in the row, col, and square and puts it in rem_vec ds
// uses set difference to find difference between {1,2,3,4,5,6,7,8,9} and rem_vec
// Values that aren't present in rem_vec are possible values for cell sudouk[m][n]

void game::convert_remainder(int m, int n) {
    sudoku[m][n].possible.clear();
    if (remainder.find(0) != remainder.end()){
        remainder.erase(0);
    }
    rem_vec = {remainder.begin(), remainder.end()};
    sort(rem_vec.begin(), rem_vec.end());
    sudoku[m][n].possible.reserve(original.size());
    set_difference(original.begin(), original.end(), rem_vec.begin(), rem_vec.end(),
                   inserter(sudoku[m][n].possible, sudoku[m][n].possible.begin()));
}

//REQUIRES: Row and col in bounds
//EFFECTS: Checks if current state promising
//RETURNS: False if duplicate values, else true

bool game::promising(int row, int col) {
    remainder.clear();
    return check_row(row) && check_col(col) && check_square(row, col);
}

//RETURNS: False if duplicate values in the row, else true
bool game::check_row(int m){
    unordered_set <int> set;
    for (int n = 0; n < 9; n++){
        if (set.find(sudoku[m][n].val) != set.end() && sudoku[m][n].val != 0){
            return false;
        }
        set.insert(sudoku[m][n].val);
        remainder.insert(sudoku[m][n].val);
    }
    return true;
}

//RETURNS: False if duplicate values in the col, else true
bool game::check_col(int n){
    unordered_set <int> set;
    for (int m = 0; m < 9; m++){
        if (set.find(sudoku[m][n].val) != set.end() && sudoku[m][n].val != 0){
            return false;
        }
        set.insert(sudoku[m][n].val);
        remainder.insert(sudoku[m][n].val);
    }
    return true;
}

//RETURNS: False if duplicate values in the square, else true
bool game::check_square (int m, int n) {
    unordered_set <int> set;
    int col = (n / 3) * 3;
    int row = (m / 3) * 3;
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++){
            if (set.find(sudoku[i][j].val) != set.end() && sudoku[i][j].val != 0){
                return false;
            }
            set.insert(sudoku[i][j].val);
            remainder.insert(sudoku[i][j].val);
        }
    }
    return true;
}

// REQUIRES: .txt file, first value is the mode (1,2). Input should be in following format:
// 2
// 0 2 4 0 0 7 0 0 0
// 6 0 0 0 0 0 0 0 0
// ...
// EFFECTS: Puzzle solves for the 0 values
void game::read_input() {
    int value = 0;
    int m = 0;
    int n = 0;
    cin >> mode;
    while (cin >> value) {
        sudoku[m][n].val = value;
        if (value != 0) {
            sudoku[m][n].input = true;
            total_inputs++;
        }//if
            n = (n + 1) % 9;
        if (n == 0) {m = (m + 1) % 9;}
    }//while
    print_grid();
}//read_input

// EFFECTS: prints out the current grid
void game::print_grid() {
    cout << "GRID:";
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            if (j % 3 == 0 && j != 0) {
                cout << "|| ";
            }
            else if (j != 0){
                cout << "| ";
            }
            else {
                cout << " ";
            }
            cout << sudoku[i][j].val << " ";
        }//for
        cout << "\n";
        if (i % 3 == 2 && i != 8){
            cout << "-------------------------------------\n";
        }//if
    }//for
    cout << "\n";
}

// MODIFIES: Sudoku board by filling in values
// EFFECTS: checks what are the possible values for a cell (looks at row, col, square), if only 1 possible value
// fills in the cell with that value
void game::intial() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int m = 0; m <= SIZE; m++){
            for (int n = 0; n <= SIZE; n++){
                if (!sudoku[m][n].input) {
                    promising(m, n);
                    convert_remainder(m, n);
                    if (sudoku[m][n].possible.size() == 1){
                        sudoku[m][n].val = sudoku[m][n].possible[0];
                        sudoku[m][n].input = true;
                        changed = true;
                    }
                    if (sudoku[m][n].possible.size() == 0){
                        cout << "ERROR with cell (" << m << "," << n << ")";
                        exit(1);
                    }
                }//if
            }
        }
    }
    print_grid();
}

//PART B

//EFFECTS: generates random sudoku board - mode 1 determines values needed in 3x3, mode 2 randomly places them
void game::create_sudoku(int row, int col, int mode, vector<int>& per_3x3, vector<int>& possible_vals){
    for (int i = row; i < 3 + row; i++) {
        for (int j = col; j < 3 + col; j++){
            if(sudoku[i][j].input && mode == 1){
                per_3x3[sudoku[i][j].val]++;
            }
            if (!sudoku[i][j].input && mode == 2){
                int index = rand() % possible_vals.size();
                sudoku[i][j].val = possible_vals[index];
                possible_vals.erase(possible_vals.begin() + index);
            }
        }
    }
}
 
//REQUIRES: number of iterations, default is 1
//EFFECTS: creates X number of sudoku boards to get starting temperature or starting board
void game::intial_temp_f(int iterations){
    int count = 0;
    vector<int> final_costs;
    while(count != iterations){
        for (int row = 0; row < 9; row+=3){
            for (int col = 0; col < 9; col+=3){
                //goes through each 3x3 finds what numbers are alr input and what are left (possible_vals)
                vector<int> per_3x3(10, 0);
                vector<int> possible_vals;
                create_sudoku(row, col, 1, per_3x3, possible_vals); 
                
                for (int index = 1; index < 10; index++){
                    if (per_3x3[index] == 0){
                        possible_vals.push_back(index); 
                    }
                }
                create_sudoku(row, col, 2, per_3x3, possible_vals); 
            }
        }
        int cost = calculate_cost_ss();
        final_costs.push_back(cost);
        count++;
    }
    //only wanted one board created
    if (iterations == 1){
        return;
    }
    //after generating 200 boards
    calc_sd(final_costs);
}

//EFFECTS: calculates the sd of the final costs vector (costs of 200 boards
void game::calc_sd(vector<int> &final_costs){
    double sum = 0;
    for (auto &vals:final_costs){
            sum += vals;
        }

    double mean = sum/final_costs.size();
    double sum2 = 0;

    for (auto &vals:final_costs){
        sum2 += pow((vals-mean),2);
    }
    
    double sd = sum2/final_costs.size();
    temp = sqrt(sd);
}

//REQUIRES: sudoku board filled in
//EFFECTS: calculates the cost (duplicate in rows + cols)  
int game::calculate_cost_ss(){
    int cost = 0;
    for (int i = 0; i < 9; i++){
        vector<int> nums_row(10,0);
        vector<int> nums_col(10,0);
        for (int j = 0; j < 9; j++){
            if (nums_row[sudoku[i][j].val] == 0){
                nums_row[sudoku[i][j].val]++;
            }
            else {
                cost++;
            }
            if (nums_col[sudoku[j][i].val] == 0){
                nums_col[sudoku[j][i].val]++;
            }
            else {
                cost++;
            }
        }
    }
    return cost;
}

//EFFECTS: calculates initial sudoku board, conducts switching, checking cost of current and next sudoku boards, determines if new board should be accepted
void game::main_loop(){
    intial_temp_f();
    print_grid();
    copy_sudoku();
    int current_cost = 0;
    int prev_cost = calculate_cost_ss();
    int stuck_count = 0;
    
     while (true){
         int count = 0;
         while (count < total_inputs){
             if (prev_cost == 0){
                 print_grid();
                 return;
             }
             switch_vals();
             current_cost = calculate_cost_ss();
             double expo = (prev_cost - current_cost)/temp;
             double P = exp(expo);
             double R = (float) rand()/RAND_MAX;
        
             if (current_cost < prev_cost) {
                 stuck_count = 0;
                 prev_cost = current_cost;
                 swap(sudoku_final[vec[0].first][vec[0].second].val, sudoku_final[vec[1].first][vec[1].second].val);
             }
            
             else {
                 stuck_count++;
                 if (P > R){
                     prev_cost = current_cost;
                     swap(sudoku_final[vec[0].first][vec[0].second].val, sudoku_final[vec[1].first][vec[1].second].val);
                 }
                 else {
                     swap(sudoku[vec[0].first][vec[0].second].val, sudoku[vec[1].first][vec[1].second].val);
                 }
             }
            
             if (stuck_count > 160){
                 temp +=0.001;
             }
             count++;
         }
         temp *= 0.99;
         copy_sudoku(1);
     }
}

//EFFECTS: copies sudoku from next to current, or current to next
void game::copy_sudoku(int mode) {
    for(int vals = 0; vals < 9; vals++){
        for (int other_vals = 0; other_vals < 9; other_vals++){
            if (mode == 0){
                sudoku_final[vals][other_vals].val = sudoku[vals][other_vals].val;
                sudoku_final[vals][other_vals].input = sudoku[vals][other_vals].input;
            }
            else {
                sudoku[vals][other_vals].val = sudoku_final[vals][other_vals].val;
                sudoku[vals][other_vals].input = sudoku_final[vals][other_vals].input;
            }
        }
    }
}

//MODIFIES: sudoku grid
//EFFECTS: swaps the values of two randomly chosen cells in the same 3x3 grid
void game::switch_vals(){
    vec.clear();
    while(vec.size() != 2){
        vec.clear();
        int m = (rand() % 3) * 3;
        int n = (rand() % 3) * 3;
        int count = 0;
        while (vec.size() != 2 && count < 9){
            int row_1 = (rand() % 3) + m;
            int col_1 = (rand() % 3) + n;
            if (!sudoku[row_1][col_1].input){
                vec.push_back({row_1,col_1});
            }
            count++;
        }
    }
    swap(sudoku[vec[0].first][vec[0].second].val, sudoku[vec[1].first][vec[1].second].val);
}

//EFFECTS: checks if sudoku board is correctly solved
void game::check_correct(){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (!promising(i, j)){
                cout << "ERROR HERE: " << i << ", " << j << "\n";
            }
        }
    }
    cout << "all solved!";
}

int main(int argc, char * argv[]) {
    game sudoku;
     xcode_redirect(argc, argv);
    sudoku.read_input();
    if (sudoku.mode == 1){
        sudoku.intial();
        sudoku.genPerms(0,0);
    }
    else {
        sudoku.intial_temp_f(200);
        sudoku.main_loop();
    }
    //rule based algorithm
    sudoku.check_correct();
}
