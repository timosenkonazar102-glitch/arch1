#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int ROWS = 10;
const int COLS = 10;
int total_numbers = 8;

int board[ROWS][COLS] = {
    {1,  0, 0, 0,  0, 0,  0, 0, 2, 3}, 
    {0,  0, 0, 0,  0, 0,  0, 4, 0, 0}, 
    {0,  2, 0, 5,  0, 3,  0, 0, 0, 1}, 
    {0,  6, 0, 4,  0, 0,  0, 0, 6, 0}, 
    {0,  0, 0, 0,  0, 0,  0, 0, 0, 0}, 
    {0,  7, 0, 0,  0, 7,  0, 5, 0, 0}, 
    {0,  0, 0, 0,  0, 0,  0, 8, 0, 0}, 
    {0,  0, 8, 0,  0, 0,  0, 0, 0, 0}, 
    {0,  0, 0, 0,  0, 0,  0, 0, 0, 0}, 
    {0,  0, 0, 0,  0, 0,  0, 0, 0, 0}  
};

struct Point { int r, c; };
Point start_points[25], end_points[25];
Point current_heads[25];

int search_order[] = {2, 3, 4, 5, 6, 7, 8, 1}; 

bool is_inside(int r, int c) { return r >= 0 && r < ROWS && c >= 0 && c < COLS; }

void flood_fill(int r, int c, bool visited[ROWS][COLS], int target_num) {
    visited[r][c] = true;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i], nc = c + dc[i];
        if (is_inside(nr, nc) && !visited[nr][nc]) {
            int val = board[nr][nc];
            if (val == 0 || val == target_num) {
                flood_fill(nr, nc, visited, target_num);
            }
        }
    }
}

bool has_dead_ends(int order_idx) {
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    int current_num = search_order[order_idx];

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 0) {
                int free_neighbors = 0;
                for (int i = 0; i < 4; i++) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (is_inside(nr, nc)) {
                        int val = board[nr][nc];
                        if (val == 0 || val == -current_num) free_neighbors++;
                        else if (val > 0) {
                            for (int j = order_idx; j < total_numbers; j++) {
                                if (search_order[j] == val) { free_neighbors++; break; }
                            }
                        }
                    }
                }
                if (free_neighbors < 2) return true;
            }
        }
    }

    bool visited[ROWS][COLS];
    for (int i = order_idx; i < total_numbers; i++) {
        int num = search_order[i];
        Point p1 = current_heads[num];
        Point p2 = end_points[num];

        for(int r=0; r<ROWS; r++) for(int c=0; c<COLS; c++) visited[r][c] = false;
        
        flood_fill(p1.r, p1.c, visited, num);
        
        if (!visited[p2.r][p2.c]) return true; 
    }

    return false;
}

void print_board() {
    cout << "\nsolution find\n\n";
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] > 0) cout << "  " << board[r][c] << " ";
            else if (board[r][c] < 0) cout << " ." << abs(board[r][c]) << " ";
            else cout << "  . ";
        }
        cout << "\n";
    }
}

bool solve_game(int order_idx, int r, int c) {
    int current_number = search_order[order_idx];
    int target_r = end_points[current_number].r;
    int target_c = end_points[current_number].c;

    if (abs(r - target_r) + abs(c - target_c) == 1) {
        int next_idx = order_idx + 1;
        if (next_idx >= total_numbers) {
            for (int i = 0; i < ROWS; i++)
                for (int j = 0; j < COLS; j++)
                    if (board[i][j] == 0) return false;
            return true;
        }

        int next_number = search_order[next_idx];
        if (solve_game(next_idx, start_points[next_number].r, start_points[next_number].c)) return true;
        return false;
    }

    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; i++) {
        int next_r = r + dr[i], next_c = c + dc[i];
        if (is_inside(next_r, next_c) && board[next_r][next_c] == 0) {
            board[next_r][next_c] = -current_number;
            
            current_heads[current_number] = {next_r, next_c}; 

            if (!has_dead_ends(order_idx)) {
                if (solve_game(order_idx, next_r, next_c)) return true;
            }
            board[next_r][next_c] = 0;
        }
    }
    current_heads[current_number] = {r, c};
    return false;
}

int main() {
    bool found_first[25] = {false};
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int val = board[r][c];
            if (val > 0) {
                if (!found_first[val]) { 
                    start_points[val] = {r, c}; 
                    current_heads[val] = {r, c}; 
                    found_first[val] = true; 
                }
                else { end_points[val] = {r, c}; }
            }
        }
    }

    cout << "solving, pls wait\n";
    int first_num = search_order[0];
    if (solve_game(0, start_points[first_num].r, start_points[first_num].c)) {
        print_board();
    } else {
        cout << "No solution found\n";
    }
    return 0;
}