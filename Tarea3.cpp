// tic-tac-toe 
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <ctime>
#include <cstdlib>

using namespace std;

// Lanzada cuando set() encuentra una input ilegal
struct InputException { };

class State
{
public:

  // Juagdores
  enum Players { P2 = -1, P1 = 1 };
  
  // tamaño de casillas por lado (se supoe que se escala a NxN)
  static const int  N = 3;
  
  // numero de cuadrados
  static const int  SIZE = N*N;
  // como se muestran las piezas
  // P1, empty, P2
  static const array<char, 3> DISP;

  // Se inicializa el tablero vacio, P1 para mover
  State()
  {
    sq = { {} };
    to_move = P1;
    filled = 0; // numero de casillas ocupadas
  }

  // retorna true si el tablero esta lleno
  bool full() const
  {
    return filled >= SIZE;
  }
  
  // inicializa el estado a partir de una cadena (P1 para mover)
  // lanza InputException cuando se encuentra un error en s
  void set(const string & s)
  {
    // crear un input stream a partir de la cadena, ahora puedes usar >>, etc.
    // como para cin
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
        cout << "\t" << DISP[sq[y][x] + 1];
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
  
  // return value at square (x, y)
  int get(int x, int y) const
  {
    return sq[y][x];
  }

private:

  //P1 o P2 mueve
  int to_move;
  array<array<signed char, N>, N> sq;
  int filled; // numero de casillas ocupadas
};

//como se muestran las piezas en el tablero
// P1, empty, P2
const array<char, 3> State::DISP = {{ 'o', '-', 'x' }};

// Retorna P1, P2 o 0 si no hay ganador aun
int check_winner(const State& st)
{
  
  int N = State::N; //Para no escribir lo mismo todo el rato.

  //if val=0 continue porque al estar vacía no hay ganador,
  //if val!=0 se revisa si todas las filas o columnas son iguales a val,
  //si hay != break xq !gano si all= val, gano=true y return val= winner

  // Revisa filas
  for (int y = 0; y < N; ++y) {
    int val = st.get(0, y);
    
    if (val == 0){
      continue; //Continue significa que se "salta" ekl resto del codigo y pasa a la siguiente fila(for);
    }
    
    bool gano = true;

    for (int x = 1; x < N; ++x) {
      if (st.get(x, y) != val) {
        gano = false; break; //Break significa que se sale del for y no se siguen revisando las casillas de esa fila. 
      }
    }
    if (gano) return val;
  }

  // Revisa columnas
  for (int x = 0; x < N; ++x) {
    int val = st.get(x, 0);
    if (val == 0) continue;
    bool gano = true;
    for (int y = 1; y < N; ++y) {
      if (st.get(x, y) != val) { gano = false; break; }
    }
    if (gano) return val;
  }

  // Revisa diagonales (desde la esquina up-izquierda y  up-derecha)
  int val = st.get(0, 0);
  if (val != 0) {
    bool gano = true;
    for (int i = 1; i < N; ++i) {
      if (st.get(i, i) != val) {
        gano = false;
        break;
      }
    }
    if (gano) return val;
  }

  val = st.get(0, N - 1);
  if (val != 0) {
    bool gano = true;
    for (int i = 1; i < N; ++i) {
      if (st.get(i, N - 1 - i) != val) {
        gano = false;
        break;
      }
    }
    if (gano) return val;
  }

  return 0; // no hay ganador aún
}

//Funicon de evaluar para Minimax
int evaluate(const State &st){
  int winner = check_winner(st);

  if (winner == State::P1) {
    return 10; // P1 win
  } 
  else if (winner == State::P2) {
    return -10; // P2 win
  }
  else {
    return 0; // empate-no winner
  }
}




int menuInicial(){

  cout << "\n\t===========================\n";
  cout << "\n\t   Bienvenido a TicTacToe  \n";
  cout << "\n\t===========================\n\n";
  cout << endl;
  cout << "Seleccione el tipo de jugador para X: \n";
  cout << "1. Humano\n";
  cout << "2. Computadora\n";
  cout << "Seleccione: ";
  int tipoJP;
  cin >> tipoJP;
  return tipoJP;
}

//Jugador aleatorio (return x,y)
void randomMove(const State &st, int &x, int &y){

  //casillas disponibles
  vector<pair<int,int>> availableMoves;
  
  for (int i = 0; i < State::N; ++i) {
    for (int j = 0; j < State::N; ++j) {
      if (st.get(i, j) == 0) {
        availableMoves.emplace_back(i, j);
      }
    }
  }

  // Seleccionar una casilla disponible al azar
  if (!availableMoves.empty()) {
    int idx = rand() % availableMoves.size();
    x = availableMoves[idx].first;
    y = availableMoves[idx].second;
  }
  else {
    x = -1; // No hay movimientos disponibles
    y = -1;
  }
}

int main()
{

  /*
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
    //construye el estado a partir de la cadena
    //tambien captura la excepcion e imprime el mensaje de error
    try {
      State st;
      st.set(s);
      cout<<"Antes de mover"<<endl;
      st.print();
      cout<<"Despues de mover. Status: "<<st.make_move(0,0)<<endl;
      st.print();
      cout<<"==============================="<<endl;
      cout << endl;
    }
    catch (InputException & e) {
      cerr << "corrupt input: " << s << endl;
    }
  }
  */

}