#include <array>
#include <iostream>
#include <vector>

int state = -1;

enum GameOver { LINE, COLUMN, DIAGONAL, DRAW };
enum Player { ONE = 1, TWO = 2 };

using namespace std;

using Line = std::array<std::array<int, 2>, 3>;
using Table = std::array<Line, 3>;

void init_table(Table &table) {
  int index = 1;
  for (int i = 0; i < table.size(); ++i) {
    Line line = table[i];
    for (int y = 0; y < line.size(); ++y) {
      table[i][y] = {index, 0};
      ++index;
    }
  }
};

bool areSameLine(Line line, int n) {
  unordered_map<int, int> m; // hash map to store the frequency of every
                             // element
  for (int i = 0; i < n; i++) {
    m[line[i][1]]++;
  }
  if (m.size() == 1)
    return true;
  else
    return false;
}

bool areSameColumn(Table table, int n) {
  unordered_map<int, int> m; // hash map to store the frequency of every
                             // element
  for (int i = 0; i < n; i++) {
    m[table[i][0][1]]++;
  }
  if (m.size() == 1)
    return true;
  else
    return false;
}

bool areSameDiagonal(Table table, int n) {
  unordered_map<int, int> m; // hash map to store the frequency of every
                             // element
  for (int i = 0; i < n; i++) {
    m[table[i][i][1]]++;
  }
  if (m.size() == 1)
    return true;
  else
    return false;
}

bool check_if_win(Table table) {
  for (int i = 0; i < table.size(); ++i) {
    unordered_map<int, int> lines_val;
    Line line = table[i];
    int n = sizeof(line) / sizeof(line[i]);
    for (int y = 0; y < line.size(); ++y) {
      // check line
      if (table[i][y][1] != 0 && areSameLine(line, n)) {
        state = GameOver::LINE;
        return true;
      }
      if (table[y][i][1] != 0 && areSameColumn(table, n)) {
        state = GameOver::COLUMN;
        return true;
      }
      if (table[i][i][1] != 0 && areSameDiagonal(table, n)) {
        state = GameOver::DIAGONAL;
        return true;
      }
    }
  }
  return false;
}

int show_player_turn(int turn) {
  cout << "Au tour du joueur ";
  if (turn % 2 == 0) {
    cout << Player::ONE;
  } else {
    cout << Player::TWO;
  }
  cout << ": ";
  return 1;
};

bool is_empty_box(Table table, int input) {
  bool empty_box = true;
  for (int i = 0; i < table.size(); ++i) {
    Line line = table[i];
    for (int y = 0; y < line.size(); ++y) {
      if (table[i][y][0] == input && table[i][y][1] != 0) {
        empty_box = false;
        break;
      }
    }
  }
  return empty_box;
};

bool is_full_box(Table table) {
  int box = 0;

  int size_line = sizeof(table[0]) / sizeof(table[0][0]);
  int size_column = sizeof(table) / sizeof(table[0]);
  int total_size = size_line * size_column;

  for (int i = 0; i < table.size(); ++i) {
    for (int y = 0; y < table[i].size(); ++y) {
      if (table[i][y][1] != 0) {
        ++box;
      }
    }
  }
  if (box == total_size) {
    state = GameOver::DRAW;
    return true;
  }
  return false;
}

void input_validator(Table table, int turn, int &input) {

  while (show_player_turn(turn)) {

    if (!(cin >> input)) {
      cout << "Entrée invalide. Veuillez saisir un nombre entier." << endl;
      cin.clear(); // Efface les erreurs
      cin.ignore(std::numeric_limits<std::streamsize>::max(),
                 '\n'); // Vide le tampon

      continue;
    }

    if (input < 1 || input > 9) {
      cout << "Valeur non valable, veuillez entrer un nombre entre 1 et 9."
           << endl;
      continue;
    }
    if (!is_empty_box(table, input)) {
      cout << "Le champs est déjà pris, veuillez entre un autre champs" << endl;
      continue;
    }
    break;
  }
}

void set_table(Table &table, int input, int turn) {
  for (int i = 0; i < table.size(); ++i) {
    Line line = table[i];
    for (int y = 0; y < line.size(); ++y) {
      if (table[i][y][0] == input) {
        if (turn % 2 == 0) {
          table[i][y][1] = Player::ONE;
        } else {
          table[i][y][1] = Player::TWO;
        }
        break;
      }
    }
  }
}
void show_table(Table table) {
  cout << "-------------" << endl;
  for (int i = 0; i < table.size(); ++i) {
    Line line = table[i];
    cout << "| ";
    for (int y = 0; y < line.size(); ++y) {
      if (table[i][y][1] == 0) {
        cout << table[i][y][0];
      } else if (table[i][y][1] == Player::ONE) {
        cout << "X";
      } else if (table[i][y][1] == Player::TWO) {
        cout << "O";
      }
      cout << " | ";
    }
    cout << endl << "-------------" << endl;
  }
  cout << endl;
}

int current_player(int turn) {
  if (turn % 2 == 0) {
    return Player::ONE;
  }
  return Player::TWO;
}

int main() {

  while (true) {

    int turn = 0;
    int input = 0;
    state = -1;

    Table table;
    init_table(table);

    cout << endl << "---------- Grille en cours ----------" << endl;
    cout << endl << "---------- Joueur 1: [X] ------------" << endl;
    cout << "---------- Joueur 2: [0] ------------" << endl << endl;
    cout << "---------- Nombre 1-9: champs à remplir" << endl;

    show_table(table);

    while (state == -1) {
      input_validator(table, turn, input);
      set_table(table, input, turn);
      show_table(table);
      cout << "Tour du joueur " << current_player(turn) << " terminé !" << endl;
      // TODO VERIFIER SI TOUT EST PLEIN
      if (check_if_win(table) || is_full_box(table)) {
        switch (state) {
        case GameOver::LINE:
          cout << "Le joueur " << current_player(turn)
               << " a gagné en faisant << une ligne >>" << endl;
          break;
        case GameOver::COLUMN:
          cout << "Le joueur " << current_player(turn)
               << " a gagné en faisant << une colonne >>" << endl;
          break;
        case GameOver::DIAGONAL:
          cout << "Le joueur " << current_player(turn)
               << " a gagné en faisant << une diagonale >>" << endl;
          break;
        default:
          cout << "Egalité !" << endl;
          break;
        }
      }
      ++turn;
    }

    while (true) {
      cout << "Recommencer ? [o/n] :";
      char response;
      if (!(cin >> response)) {
        cout << "Je n'ai pas compris votre réponse." << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),
                   '\n'); // Vide le tampon
        continue;
      }
      if (response == 'n') {
        break;
      } else if (response == 'o') {
        state = -1;
        break;
      } else {
        cout << "Réponse invalide." << endl;
        continue;
      }
    }
    if (state != -1) {
      break;
    }
  }
  cout << "Merci d'avoir joué !" << endl;
  return 0;
}
