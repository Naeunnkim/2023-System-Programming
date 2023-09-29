#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define DINO_HEIGHT              13
#define CACTUS_HEIGHT            5
#define CACTUS_SPEED_ORIGINAL    3
#define CACTUS_SPEED_MIDDLE      5
#define CACTUS_SPEED_FAST        7
#define GRAVITY                  3
#define CEILING                  0

#define SPACE                    32 //JUMP
#define ENTER                    10 //QUIT

void DrawDinosaur(int);
void DrawCactus(int, int);
int _kbhit(void);
int GetKeyDown(void);
bool CheckCollision(int, int, int);

int _kbhit(void)
{
        struct timeval tv = { 0L, 0L };
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(0, &fds);

        return select(1, &fds, NULL, NULL, &tv);
}

int GetKeyDown(void)
{
        if(_kbhit()) { //입력이 있는 경우
                return getch(); //키보드 값을 반환
        }
}

bool CheckCollision(int dinosaurHead, int cactusHead, int cactusPos) {
        int dinoBottom = dinosaurHead + DINO_HEIGHT;
        int cactusRight = cactusPos + 4;

        if(dinoBottom>=cactusHead && dinosaurHead <= cactusHead+CACTUS_HEIGHT && cactusPos <= 15) {
                return true;
        }

        else { return false; }
}

int main(int argc, char* argv[], char* env[])
{
        initscr();

        int winWidth = 0, winHeight = 0;
        int dinosaurHead = 0;
        int cactusHead = 0, cactusPos = 0;
        bool isJumping = false;
        bool isBottom = true;
        int getKey = 0;

        bool touched = false; //장애물에 닿음
        bool isCleared = false; //넘기 성공
        int score = 0;

        getmaxyx(stdscr, winHeight, winWidth);

        dinosaurHead = winHeight - DINO_HEIGHT;
        cactusHead = winHeight - CACTUS_HEIGHT;
        cactusPos = winWidth - 20;

        while(true) {

                getKey = GetKeyDown();
                if(SPACE == getKey && isBottom) {
                        isJumping = true;
                        isBottom = false;
                }else if(ENTER == getKey || touched) {
                        break; //while문을 나가는거, 바람직한 종료를 위함
                }

                if(isJumping) {
                        dinosaurHead -= GRAVITY;
                }else {
                        dinosaurHead += GRAVITY;
                }

                if(winHeight <= (dinosaurHead + DINO_HEIGHT)) {
                        dinosaurHead = winHeight - DINO_HEIGHT; //바닥 뚫고 내려가지 않게 잡아두기
                        isBottom = true;
                }

                if(dinosaurHead <= CEILING) {
                        isJumping = false; //jump 끝
                        score ++;
                }

                if(CheckCollision(dinosaurHead, cactusHead, cactusPos)) {
                        touched = true;
                }

                if(score<5) {
                        cactusPos -= CACTUS_SPEED_ORIGINAL;
                }else if(score>=5 && score<10) {
                        cactusPos -= CACTUS_SPEED_MIDDLE;
                }else{
                        cactusPos -= CACTUS_SPEED_FAST;
                }

                if(cactusPos <= 0) {
                        cactusPos = winWidth - 20;
                }

                //스코어 출력
                move(1,1);
                printw("Score: %d", score);
                refresh();

                clear();
                DrawDinosaur(dinosaurHead);
                DrawCactus(cactusHead, cactusPos);
                usleep(100000);
                refresh();
        }

        endwin();

        return 0;
}

void DrawDinosaur(int dinosaurHead)
{
        move(dinosaurHead, 0);
        static bool legDraw = true;

        printw("        &&&&&&& \n");
        printw("       && &&&&&&\n");
        printw("       &&&&&&&&&\n");
        printw("&      &&&      \n");
        printw("&&     &&&&&&&  \n");
        printw("&&&   &&&&&     \n");
        printw(" &&  &&&&&&&&&& \n");
        printw(" &&&&&&&&&&&    \n");
        printw("  &&&&&&&&&&    \n");
        printw("    &&&&&&&&    \n");
        printw("     &&&&&&     \n");

        if (legDraw) {
                printw("     &    &&&     \n");
                printw("     &&           ");
                legDraw = false;
        }else {
                printw("     &&&  &       \n");
                printw("          &&      ");
                legDraw = true;
        }
}

void DrawCactus(int cactusHead, int cactusPos)
{
        move(cactusHead, cactusPos);
        printw("$$$$");
        move(cactusHead+1, cactusPos);
        printw(" $$ ");
        move(cactusHead+2, cactusPos);
        printw(" $$ ");
        move(cactusHead+3, cactusPos);
        printw(" $$ ");
        move(cactusHead+4, cactusPos);
        printw(" $$ ");
}

