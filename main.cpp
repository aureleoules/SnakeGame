#include <iostream>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>   // for nanosleep
#include <unistd.h> // for usleep
using namespace std;

bool gameOver;
const int width = 40;
const int height = 20;
int x,y, fruitsX, fruitsY, score;
int tailX[100], tailY[100];
int nTail = 0;
enum eDirection {STOP=0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitsX = rand() % width;
    fruitsY = rand() % height;
    score = 0;
       
}

void Draw() {
    // system("cls");
    system("clear");
    for(int i = 0; i < width + 2; i++) {
        cout << "#";
    }
    cout << endl;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(j == 0)
                cout << "#";

            if(i == y && j == x) {
                cout << "O";
            } else if(i == fruitsY && j == fruitsX) {
                cout << "F";
            } else {
                bool print = false;
                for(int k = 0; k < nTail; k++) {
                    if(tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if(!print) {
                    cout << " ";
                }
            }
            if(j == width - 1) {
                cout << "#";
            }
        }
        cout << endl;
    }
    for(int i = 0; i < width + 2; i++) {
        cout << "#";
    }
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input() {
    if(kbhit()) {
        switch(getchar()) {
            case 'q':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'z': 
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
    if(x > width || x < 0 || y > height || y < 0) {
        gameOver = true;
    }
    for(int i = 0; i < nTail; i++) {
        if(tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }
    if(x == fruitsX && y == fruitsY) {
        score+= 10;
        fruitsX = rand() % width;
        fruitsY = rand() % height;
        nTail++;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for(int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
}

void sleep_ms(int milliseconds) {
    #ifdef WIN32
        Sleep(milliseconds);
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        usleep(milliseconds * 1000);
    #endif
}

int main() {
    Setup();
    while(!gameOver) {
        Draw();
        Input();
        Logic();
        sleep_ms(100);
    }
    return 0;
}