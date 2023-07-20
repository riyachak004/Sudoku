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

void game::genPerms(int m, int n) {
    int save_val = 0;
    if (check_complete()) {
        print_grid();
        exit(0);
    }
    if (!sudoku[m][n].input) {
        save_val = sudoku[m][n].val;
        sudoku[m][n].val = 0;
    }
    if (!promising(m, n)) {
        return;
    }
    if (!sudoku[m][n].input) {
        convert_remainder(m, n);
        if (sudoku[m][n].possible.empty()){
            sudoku[m][n].val = save_val;
            return;
        }
        while (!sudoku[m][n].possible.empty()){
            sudoku[m][n].val = sudoku[m][n].possible.back();
            sudoku[m][n].possible.pop_back();
          //  print_grid();
            int new_n = (n + 1) % 9;
            int new_m = m;
            if (new_n == 0){
                new_m = (m + 1) % 9;
            }
           // if (check_ahead(new_m, new_n) && check_ahead((m + 1) % 9, n)) {
                genPerms(new_m, new_n);
         //   }//if
        }//while
        if (sudoku[m][n].possible.empty()){
            sudoku[m][n].val = 0;
        }
    }// if
    else {
        n = (n + 1) % 9;
        if (n == 0){
            m = (m + 1) % 9;
        }
        genPerms(m,n);
    }
} // genPerms()

bool game::check_ahead (int m, int n){
    if (sudoku[m][n].val != 0){
        return true;
    }
    promising(m, n);
    convert_remainder(m, n);
    if (remainder.empty()){
        return false;
    }
    else {
        return true; 
    }
    
}

void game::convert_remainder(int m, int n) {
    sudoku[m][n].possible.clear();
    if (remainder.find(0) != remainder.end()){
        remainder.erase(0);
    }
    rem_vec = {remainder.begin(), remainder.end()};
    sort(rem_vec.begin(), rem_vec.end());
    sudoku[m][n].possible.reserve(original.size());
    set_difference(original.begin(), original.end(), rem_vec.begin(), rem_vec.end(), inserter(sudoku[m][n].possible, sudoku[m][n].possible.begin()));
}


bool game::check_complete () {
    for (int m = 8; m >= 0; m--) {
        for (int n = 8; n >= 0; n--){
            if (sudoku[n][m].val == 0) {return false;}
        }
    }
    return true;
}

bool game::promising(int m, int n) {
    remainder.clear();
    return check_row(m) && check_col(n) && check_square(m, n);
}

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

void game::print_grid_final() {
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
            cout << sudoku_final[i][j].val << " ";
        }//for
        cout << "\n";
        if (i % 3 == 2 && i != 8){
            cout << "-------------------------------------\n";
        }//if
    }//for
    cout << "\n";
    
}

void game::intial() {
    int n = 0;
    int m = 0;
    int count = 0;
    int val = 81 - total_inputs;
    while (count != val) {
        if (!sudoku[m][n].input) {
            promising(m, n);
            convert_remainder(m, n);
            if (sudoku[m][n].possible.size() == 1){
                sudoku[m][n].val = sudoku[m][n].possible[0];
                sudoku[m][n].input = true;
          //      print_grid();
            }
            else {
                count++;
            }
        }//if
        n = (n + 1) % 9;
        if (n == 0){
            m = (m + 1) % 9;
        }//if
    }
    print_grid();
}


//Created backtracking algorithm to solve suduko problems and incorporated a rule-based algorithm and constrain propagtion via forward progation
//adding in constraint propagtion via forward progation - basically check if the bottom and right have values
//if there are no values after the three criteria have been satisfied dump the val
//foward tracking goes to the surrounding cells and deletes the value (placed in the cell) from those cells 


//optimiation algorithm - choose the radom state, makes a stochastic move - has an acceptance probability. state gets greedier as times goes on. 
//generates random state, choose random 2 cells and flip then, then cost funciton determines if this new state was better than the old state (cost function = duplicates in rows and columns). Select a starting temperature/cooling rate. 



//collaboration easy access for mentorship/ guidancee - culture is really important to me - wolverine has Diversity Employee Resource Groups 
//value innovation and creavitity and i would like work in a place where new ideas and approaches are valued  

//risk application dev. team (RAD) risk viewing apps/handling actions in the market after trading - Using C# 
//book  club and women's club 

//node linkedin list 
//within a week 

//PART B

void game::intial_temp_f(){
    int count = 0;
    vector<int> final_costs;
    while(count != 200){
        //creates one example sudoku board
        for (int row = 0; row < 9; row+=3){
            for (int col = 0; col < 9; col+=3){
                //goes through each 3x3 finds what vals are alr inputted and what are left to input (possible_vals)
                vector<int> per_3x3;
                for (int i = row; i < 3 + row; i++) {
                    for (int j = col; j < 3 + col; j++){
                        if(sudoku[i][j].input){
                            per_3x3.push_back(sudoku[i][j].val);
                        }
                    }
                }
                        sort(per_3x3.begin(), per_3x3.end());
                        vector<int> possible_vals;
                        possible_vals.reserve(original.size());
                        set_difference(original.begin(), original.end(), per_3x3.begin(), per_3x3.end(), inserter(possible_vals, possible_vals.begin()));
                    for (int i = row; i < 3 + row; i++) {
                        for (int j = col; j < 3 + col; j++){
                            if(!sudoku[i][j].input){
                                int index = rand() % possible_vals.size();
                                sudoku_starter[i][j] = possible_vals[index];
                                possible_vals.erase(possible_vals.begin() + index);
                            }
                            else {
                                sudoku_starter[i][j] = sudoku[i][j].val;
                            }
                        }
                    }
            }
        }
        
        //print_starter_grid();
        int cost = calculate_cost_ss();
        final_costs.push_back(cost);
        count++;
    }
    calc_sd(final_costs);
}

void game::calc_sd(vector<int> &final_costs){
    double sum = 0;
    for (auto vals:final_costs){
            sum += vals;
        }

    double mean = sum/final_costs.size();
    double sum2 = 0;

    for (auto vals:final_costs){
        sum2 += pow((vals-mean),2);
    }
    
    double sd = sum2/final_costs.size();
    temp = sqrt(sd);
    //cout << "TEMP: " << temp;
}

int game::calculate_cost_ss(){
    int cost = 0;
    for (int i = 0; i < 9; i++){
        unordered_set <int> nums;
        for (int j = 0; j < 9; j++){
            if (nums.find(sudoku_starter[i][j]) == nums.end()){
                nums.insert(sudoku_starter[i][j]);
            }
            else {
                cost++;
            }
        }
    }
    for (int i = 0; i < 9; i++){
        unordered_set <int> nums;
        for (int j = 0; j < 9; j++){
            if (nums.find(sudoku_starter[j][i]) == nums.end()){
                nums.insert(sudoku_starter[j][i]);
            }
            else {
                cost++;
            }
        }
    }
    return cost;
}


int game::calculate_cost_ssfinal(){
    int cost = 0;
    for (int i = 0; i < 9; i++){
        unordered_set <int> nums;
        for (int j = 0; j < 9; j++){
            if (nums.find(sudoku[i][j].val) == nums.end()){
                nums.insert(sudoku[i][j].val);
            }
            else {
                cost++;
            }
        }
    }
    for (int i = 0; i < 9; i++){
        unordered_set <int> nums;
        for (int j = 0; j < 9; j++){
            if (nums.find(sudoku[j][i].val) == nums.end()){
                nums.insert(sudoku[j][i].val);
            }
            else {
                cost++;
            }
        }
    }
    return cost;
}


void game::print_starter_grid(){
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
            cout << sudoku_starter[i][j] << " ";
        }//for
        cout << "\n";
        if (i % 3 == 2 && i != 8){
            cout << "-------------------------------------\n";
        }//if
    }//for
    cout << "\n";
    
}




void game::main_loop(){
    generate_random_final_sudoku();
    copy_sudoku();
    int current_cost = 0;
    int prev_cost = calculate_cost_ssfinal();
    int stuck_count = 0;
    
    while (true){
        int count = 0;
        while (count < total_inputs){
            if (prev_cost == 0){
                print_grid_final();
               // copy_opposite();
                return;
            }
            switch_vals();
            current_cost = calculate_cost_ssfinal();
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
      //  cout << "TEMP: " << temp;
       // cout << "COST: " << prev_cost << "\n";
        //print_grid_final();
    }
}

void game::copy_sudoku() {
    for(int vals = 0; vals < 9; vals++){
        for (int other_vals = 0; other_vals < 9; other_vals++){
            sudoku_final[vals][other_vals].val = sudoku[vals][other_vals].val;
            sudoku_final[vals][other_vals].input = sudoku[vals][other_vals].input;
        }
    }
   // print_grid_final();
}

void game::copy_opposite() {
    for(int vals = 0; vals < 9; vals++){
        for (int other_vals = 0; other_vals < 9; other_vals++){
            sudoku[vals][other_vals].val = sudoku_final[vals][other_vals].val;
            sudoku[vals][other_vals].input = sudoku_final[vals][other_vals].input;
        }
    }
    print_grid_final();
}

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

void game::generate_random_final_sudoku(){
    vector<int> final_costs;
        //creates one example sudoku board
        for (int row = 0; row < 9; row+=3){
            for (int col = 0; col < 9; col+=3){
                //goes through each 3x3 finds what vals are alr inputted and what are left to input (possible_vals)
                vector<int> per_3x3;
                for (int i = row; i < 3 + row; i++) {
                    for (int j = col; j < 3 + col; j++){
                        if(sudoku[i][j].input){
                            per_3x3.push_back(sudoku[i][j].val);
                        }
                    }
                }
                        sort(per_3x3.begin(), per_3x3.end());
                        vector<int> possible_vals;
                        possible_vals.reserve(original.size());
                        set_difference(original.begin(), original.end(), per_3x3.begin(), per_3x3.end(), inserter(possible_vals, possible_vals.begin()));
                    for (int i = row; i < 3 + row; i++) {
                        for (int j = col; j < 3 + col; j++){
                            if(!sudoku[i][j].input){
                                int index = rand() % possible_vals.size();
                                sudoku[i][j].val = possible_vals[index];
                                possible_vals.erase(possible_vals.begin() + index);
                            }
                        }
                    }
            }
        }
    //print_grid();
}

void game::check_correct(){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (!promising(i, j)){
                cout << "ERROR HERE: " << i << ", " << j << "\n";
            }
        }
    }
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
        sudoku.intial_temp_f();
        sudoku.main_loop();
    }
    //rule based algorithm
    
    
    
}
