//
//  sudoku.hpp
//  sudoku
//
//  Created by Riya Chakravarty on 7/5/22.
//

#ifndef sudoku_hpp
#define sudoku_hpp

#include <stdio.h>
#include <vector>
#include <unordered_set>

using namespace std;

class game{
private:
    struct cell {
        int val;
        bool input = false;
        vector <int> possible;
        
    };
    cell sudoku[9][9];
    unordered_set <int> remainder;
    vector <int> rem_vec;
    vector<int> original = {1,2,3,4,5,6,7,8,9};
    int total_inputs = 0;
    
    //PART B
    int sudoku_starter[9][9];
    double temp;
    cell sudoku_final[9][9];
    vector<pair<int,int>> vec;
    
public:
    int mode;
    
    bool check_complete();
    
    bool check_row(int m);
    
    //calls row, col, and square (check) if all true, sudoku is promising
    bool promising(int m, int n);
    
    bool check_col(int n);
    
    void genPerms(int m, int n);
    
    bool check_square(int m, int n);
    
    void convert_remainder(int m, int n);
    
    void read_input();
    
    void print_grid();
    
    //checks to see if any boxes can be filled in off the bat,changes the val, and turns input to be true
    void intial();
    
    bool check_ahead(int m, int n);
    
    void print_starter_grid();
    
    //calculates the intial temperature
    void intial_temp_f();
    
    //calculates cost of starter sudoku
    int calculate_cost_ss();
    
    void calc_sd(vector<int> &final_costs);
    
    void main_loop();
    
    void generate_random_final_sudoku();
    
    int calculate_cost_ssfinal();
    
    void copy_sudoku();
    
    void switch_vals();
    
    void print_grid_final();
    
    void copy_opposite();
    
    void check_correct();
    
};


#endif /* sudoku_hpp */
