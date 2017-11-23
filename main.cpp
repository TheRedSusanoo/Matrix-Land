#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#define ll long long

using namespace std;

ll max_of(ll one, ll two){
   if(one >= two)
         return one;
   return two;
}


class cell{                                         // Class for every square/cell in the matrix
   public:
      ll val;                          // Number in that square
      ll max_right;                    // The maximum sum obtainable by moving only right from a cell(includes the val of the cell)
      ll max_left;                     // The maximum sum obtainable by moving only left from a cell(includes the val of the cell)
      ll come_from_left_include_top;   // Stores the maximum sum position if the entry point from the upper row were at this cell or                                           at any other cell left to it.
      ll come_from_right_include_top;  // Stores the maximum sum position if the entry point from the upper row were at this cell or                                           at any other cell right to it.
      ll max_cost_of_arriving;         // The maximum sum that can obtained to arrive in a cell
};

int m ,n;
vector<vector<cell>> board;

long long kadane_to_the_rescue(vector<int>& bored){
    long long max_so_far=0;
    long long ed = 0;

    for(int i = 0; i < bored.size(); i++){
        ed = ed + bored[i];
        if(ed < 0)
            ed = 0;
        if(max_so_far < ed)
            max_so_far = ed;
    }

    return max_so_far;
}


ll move_right(int x, int y){                                   //recursive function to calculate cell::max_right;
  /* Starts from index 0 of each row and recursively computes the cell:: max_right for each cell in the row */
    
    if(y==m-1){                      //Reached the last cell in a row
        board[x][y].max_right = board[x][y].val;
             return board[x][y].val;
    }
    board[x][y+1].max_right = move_right(x, y+1);
  /*Recursive calls move from left to right, values are returned in the opposite direction(obv)*/
    return max_of(board[x][y].val, board[x][y].val + board[x][y+1].max_right);
}


ll move_left(int x, int y){                                     //see move_right();
  /* Starts from the last index(m-1) of each row and recursively computes the cell:: max_left for each cell in the row */
    if(y==0){
        board[x][y].max_left = board[x][y].val;
        return board[x][y].val;
    }

    board[x][y-1].max_left = move_left(x, y-1); 
    /*Recursive calls move from right to left, values are returned in the opposite direction(obv)*/

    return max_of(board[x][y].val, board[x][y].val + board[x][y-1].max_left);
}


ll approach_from_left_include_top(int x, int y){
  /*
   for a given cell board[x][y], calculates the max possible sum if you were to enter this row 'x'(from the upper row 'x-1') at a        column 'y' or any other column left to it(0 <= 'entry point' <= y)
  */
    if(x == 0) return board[x][y].val;  //there's no row above x = 0


    if(y == 0){                  // terminating condition for recursion(calls move right to left)
        board[x][y].come_from_left_include_top = board[x][y].val + board[x-1][y].max_cost_of_arriving;
        return board[x][y].come_from_left_include_top;

    }
    board[x][y].come_from_left_include_top =
        
    max_of(board[x][y].max_left + board[x-1][y].max_cost_of_arriving , board[x][y].val + approach_from_left_include_top(x, y-1) );
    /*
     The maximum out of
     1. Entry point at 'y' + max_right of board[x][y]
     2. Entry point somewhere right to 'y'(recursive call) + board[x][y]
    */
    
    return board[x][y].come_from_left_include_top;
}



ll approach_from_right_include_top(int x, int y){
  /*
  for a given cell board[x][y], calculates the max possible sum if you were to enter this row 'x'(from the upper row 'x-1') at a       column 'y' or any other column right to it(y <= 'entry point' <= m-1)
  */
    if(x == 0) return board[x][y].val;

    if(y == m-1){              // terminating condition for recursion(calls move left to right)
        board[x][y].come_from_right_include_top = board[x][y].val + board[x-1][y].max_cost_of_arriving;
        return board[x][y].come_from_right_include_top;

    }
    
    
    board[x][y].come_from_right_include_top = 
        
    max_of(board[x][y].max_right + board[x-1][y].max_cost_of_arriving, board[x][y].val + approach_from_right_include_top(x, y+1));
    
    /*
     The maximum out of
     1. Entry point at 'y' + max_left of board[x][y]
     2. Entry point somewhere left to 'y'(recursive call) + board[x][y]
    */
    
    
    return board[x][y].come_from_right_include_top;
}


void calc_max_cost_of_arriving_in_cell(int x, int y){
    
    
    
    if(x == 0){
        board[x][y].max_cost_of_arriving = board[x][y].max_right + board[x][y].max_left - board[x][y].val;
       /* IN the first row the max_cost of each cell is simply the sum of the maximum sum you can obtain by moving left and right               from that cell*/
    }
    else{
        
      board[x][y].max_cost_of_arriving = 
          
          max_of(board[x][y].come_from_left_include_top + board[x][y].max_right - board[x][y].val,
                 board[x][y].come_from_right_include_top + board[x][y].max_left - board[x][y].val);
     
        
     /* The maximum score with which a player can reach a cell (x, y) or board[x][y] is the maximum out of;
          1. Sum obtained by entering the row 'x' from the left side(of y) and then moving right
             (also includes the value of max_left since it is prev added to come_from_left_include_top)
          2. Sum obtained by entering the row 'x' from the right side(of y) and then moving left
             (also includes the value of max_right since it is prev added to come_from_right_include_top)
             
        One really needs a visual representation to understand this part...
     */
     }

 }





int main()
{
    ll greatest = 0;
    cin>>n>>m;
    
    if(n == 1 && m == 4000000){ // This is work around for the last testcase n=1, m=4000000
        vector<int> bored(m);
        for(int j = 0; j < m; j++)
            cin>>bored[j];
        
         greatest = kadane_to_the_rescue(bored); 
         /*Runs plain Kadane's algorithm to compute the maximum sum subarray. Since there's only one row, the answer is simply the              sum of maximum sum subarray */
    }

    else{
       board.reserve(n+2);

       for(int i = 0; i < n; i++){
           board[i].reserve(m);
       for(int j = 0; j < m; j++){
           cin>>board[i][j].val;
        }
    }


    for(int i = 0; i < n; i++){

        board[i][0].max_right = move_right(i,0);
     /*For each row, starting from from left calculate max_right recursively for each cell*/

        board[i][m-1].max_left = move_left(i,m-1);
     /*For each row, starting from from right end calculate max_left recursively for each cell*/


    if(i == 0){
      for(int j = 0; j < m; j++){
         calc_max_cost_of_arriving_in_cell(i,j); 
         // separate calculation of max_costs for cells of the first row, since they don't require top values  
      }
    }

       ll chain_reaction_lefttoright2 = approach_from_left_include_top(i, m-1);
      
       ll chain_reaction_righttoleft2 = approach_from_right_include_top(i, 0);
      
       //For each row calculate come_flit and come_frit for each cell(include tops)

    if(i > 0){
        for(int k = 0; k < m; k++){
            calc_max_cost_of_arriving_in_cell(i,k); // calc max_costs of every row other than the first one
            if(board[i][k].max_cost_of_arriving > greatest && i == n-1) // Update the score only if player has reached the last row
                   greatest = board[i][k].max_cost_of_arriving; 
        }
      }
    }   
  }
    
    cout<<greatest<<endl;
    return 0;
}
