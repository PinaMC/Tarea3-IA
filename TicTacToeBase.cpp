// tic-tac-toe 
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

using namespace std;

// thrown when set() encounters an illegal input
struct InputException { };

class State
{
public:

  // players  
  enum Players { P2 = -1, P1 = 1 };
  // edge length
  static const int  N = 3;
  // number of squares
  static const int  SIZE = N*N;
  // how pieces are displayed ... see below
  // P1, empty, P2
  static const array<char, 3> DISP;

  // initialize empty board
  // P1 to move
  State()
  {
    sq = { {} };
    to_move = P1;
    filled = 0;
  }

  // return true if board is full
  bool full() const
  {
    return filled >= SIZE;
  }
  
  // initialize state from string (P1 to move)
  // throw InputExceptiuon when you encounter
  // an error in s
  void set(const string & s)
  {
    // create input stream from string, now you can use >>, etc.
    // like for cin
    istringstream is(s);
    char c;

    to_move = P1;
    filled = 0;

    for (int y=0; y < N; ++y) {
      for (int x=0; x < N; ++x) {
        is >> c;
        if (!is) {
          throw InputException();
        }
        if (c == DISP[1+P1])
        { 
            sq[y][x] = P1; ++filled; 
        }
        else if (c == DISP[1+P2]) 
        {
            sq[y][x] = P2; ++filled; 
        }
        else
        {
            sq[y][x] = 0;
        }
      }
    }
    is >> c;
    if (is) {
      // trailing character(s)
      throw InputException();
    }
  }
  
  // print state to cout
  // format:
  //
  //  xox
  //  o-x
  //  xxo
  //  x (8)
  //
  // last line: player to move, number of filled squares
  // followed by new-line
  void print() const
  {
    for (int y=0; y < N; ++y) {
      for (int x=0; x < N; ++x) {
        cout << DISP[sq[y][x] + 1];
      }
      cout << endl;
    }
    // print player to move and #filled squares
    cout << DISP[to_move + 1]
         << " (" << filled << ")"
         << endl;
  }
  

  // make move (x, y) for player to_move
  // and return true iff move is legal
  // pre-condition: x, y within range
  bool make_move(int x, int y)
  {
    assert(x >= 0 && x < N && y >= 0 && y < N);
    auto &c = sq[y][x];
    if (c) {
      return false; // already occupied
    }
    
    c = to_move;
    to_move = - to_move;
    ++filled;
    return true;
  }

  // return player to move
  int get_to_move() const
  {
    return to_move;
  }
  
private:

  // P1 or P2 to move  
  int to_move;

  // squares (2d array)
  // stores P1,0,P2 values
  // bounds are checked in debug mode
  // and sq[y][x] = 0 works
  array<array<signed char, N>, N> sq;

  // number of non-empty squares
  int filled;           
};

// how pieces are displayed ...
// P1, empty, P2
const array<char, 3> State::DISP = {{ 'o', '-', 'x' }};


      
int main()
{
  std::vector<string> boards
  {
    "--- \
     --- \
     ---", // x draws with (0,0)

    "--- \
     -o- \
     ---", // x draws with (0,0)

    "--- \
     --o \
     ---", // x draws with (0,1)

    "-oo \
     ooo \
     ooo", // a player already won
  
    "xox \
     oxo \
     oxo", // draw

    "--o \
     -oo \
     ---", // x loses

    "oxo \
     xx- \
     oox", // x wins with (2,1)

    "-xo \
     oox \
     xoo", // x draws with (0,0)

    "--- \
     -x- \
     ---"  // x wins with (0,0)
  };

  for (const string & s : boards) {
    // construct state from string
    // also catch exception and print
    // error message
    try {
      State st;
      st.set(s);
      cout<<"Before move"<<endl;
      st.print();
      cout<<"After move. Status: "<<st.make_move(0,0)<<endl;
      st.print();
      cout<<"==============================="<<endl;
      cout << endl;
    }
    catch (InputException & e) {
      cerr << "corrupt input: " << s << endl;
    }
  }
}