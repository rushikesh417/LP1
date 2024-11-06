#include <iostream>
#include <vector>
using namespace std;

bool check_if_position_is_safe(vector<vector<int>> chessboard, int r, int c)
{
    int board_size = chessboard.size();
    int curr_row = -1;
    int curr_col = -1;

    for (int i = 0; i < board_size; i++)
    {
        if (chessboard[r][i] || chessboard[i][c])
        {
            return false;
        }

        curr_row = r - i;
        curr_col = c - i;
        if (((curr_row >= 0) && (curr_col >= 0) && (curr_row < board_size) && (curr_col < board_size)) && (chessboard[curr_row][curr_col]))
        {
            return false;
        }

        curr_row = r - i;
        curr_col = c + i;
        if (((curr_row >= 0) && (curr_col >= 0) && (curr_row < board_size) && (curr_col < board_size)) && (chessboard[curr_row][curr_col]))
        {
            return false;
        }

        curr_row = r + i;
        curr_col = c + i;
        if (((curr_row >= 0) && (curr_col >= 0) && (curr_row < board_size) && (curr_col < board_size)) && (chessboard[curr_row][curr_col]))
        {
            return false;
        }

        curr_row = r + i;
        curr_col = c - i;
        if (((curr_row >= 0) && (curr_col >= 0) && (curr_row < board_size) && (curr_col < board_size)) && (chessboard[curr_row][curr_col]))
        {
            return false;
        }
    }

    return true;
}

bool try_different_positions(vector<vector<int>> &chessboard, int queens_on_board)
{
    int board_size = chessboard.size();
    if (board_size == queens_on_board)
    {
        return true;
    }

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            bool is_safe = check_if_position_is_safe(chessboard, i, j);
            if (is_safe)
            {
                chessboard[i][j] = 1;
                
                if (try_different_positions(chessboard, queens_on_board + 1))
                {
                    return true;
                }
                 
                    chessboard[i][j] = 0;
                
            }
        }
    }

    return false;
}

vector<vector<int>> get_final_queens_position(int board_size, int start_row, int start_col)
{
    vector<vector<int>> chessboard(board_size, vector<int>(board_size, 0));
    chessboard[start_row][start_col] = 1;
    int queens_on_board = 1;
    bool has_found = try_different_positions(chessboard, queens_on_board);
    return chessboard;
}

void display_chessboard(vector<vector<int>> chessboard)
{
    int n = chessboard.size();
    int queen_cnt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (chessboard[i][j])
            {
                queen_cnt++;
            }
        }
    }

    if (queen_cnt != n)
    {
        cout << "Queen combination cannot be found" << endl;
        return;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << chessboard[i][j] << "   ";
        }
        cout << endl;
    }

    cout << "-------------------------------------" << endl;
}

int main()
{
    int board_size = 5;
    int start_row = 2;
    int start_col = 2;

    vector<vector<int>> final_queen_positions = get_final_queens_position(board_size, start_row, start_col);

    display_chessboard(final_queen_positions);

    return 0;
}





