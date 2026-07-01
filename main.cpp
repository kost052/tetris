#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <ncurses.h>
#include "tetrominos.h"

using namespace std;
using pos = unsigned int;

constexpr int enter = 10;

constexpr pos rows = 20, columns = 10;

vector<vector<bool>> matrix(rows, vector<bool>(columns, 0)), matrix_tmp = matrix;

void show_matrix(pos x = 0, pos y = 0) {
    clear();
    move(y, x);
    for(const vector<bool> &row : matrix_tmp) {
        for(const bool &state : row) {
            if(state) {
                addstr("[]");
            } else {
                addstr(" .");
            }
        }
        y++;
        move(y, x);
    }
    refresh();
}

class Tetromino {
    public:
        int x, y;
        tetromino shape;
        bool falling = false;

        void insert(int c, int r) {
            int xov = c, yov = r;
            matrix_tmp = matrix;
            for(const vector<bool> &row : shape) {
                int i = c;
                for(const bool &state : row) {
                    if(state) {
                        if(i >= 0 && i < columns && r < rows && matrix_tmp[r][i] == 0) {
                            matrix_tmp[r][i] = state;
                        } else {
                            matrix_tmp = matrix;
                            insert(x, y);
                            return;
                        }
                    }
                    i++;
                }
                r++;
            }
            x = xov;
            y = yov;
        }

        void init() {
            insert((columns-shape.size())/2+1, 0);
        }

        void move(char dir) {
            switch(dir) {
                case 'D':
                    insert(x, y+1);
                    break;
                case 'L':
                    insert(x-1, y);
                    break;
                case 'R':
                    insert(x+1 , y);
                    break;
            }
        }
};

int main() {
    initscr();
    cbreak();
    noecho();

    bool running = true, playing = false;

    random_device rng;
    
    vector<tetromino> shuffled;

    while(running) {
        show_matrix();

        switch(getch()) {
            case enter:
                playing = true;
                break;
            case 'q':
                running = false;
                break;
        }
        
        while(playing) {
            if(shuffled.size() == 0) {
                shuffled = tetros;
                shuffle(shuffled.begin(), shuffled.end(), rng);
            }

            Tetromino tetro;
            tetro.shape = shuffled[0];

            shuffled.erase(shuffled.begin());

            tetro.falling = true;
            tetro.init();

            while(tetro.falling) {
                show_matrix();
                
                switch(getch()) {
                    case 's':
                        tetro.move('D');
                        break;
                    case 'a':
                        tetro.move('L');
                        break;
                    case 'd':
                        tetro.move('R');
                        break;
                }
            }
        }
    }
}