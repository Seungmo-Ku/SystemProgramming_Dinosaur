#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define DINO_HEIGHT	13
#define CACTUS_HEIGHT	5

#define SPACE		32
#define ENTER		10

void DrawDinosaur(int);
void DrawCactus(int, int);
void DrawScore(int, int, int);
int _kbhit(void);
int GetKeyDown();

int _kbhit(void)
{
	struct timeval tv =  { 0L, 0L };
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(0, &fds);

	return select(1, &fds, NULL, NULL, &tv);
}

int GetKeyDown()
{
	if(_kbhit()){
		return getch();
	}

	return 0;
}

void DrawDinosaur(int dinosaurHead)
{
	move(dinosaurHead, 0);
	static bool legDraw = true;

	printw("        &&&&&&& ");
	move(dinosaurHead + 1, 0);
	printw("       && &&&&&&");
	move(dinosaurHead + 2, 0);
	printw("       &&&&&&&&&");
	move(dinosaurHead + 3, 0);
	printw("&      &&&      ");
	move(dinosaurHead + 4, 0);
	printw("&&     &&&&&&&  ");
	move(dinosaurHead + 5, 0);
	printw("&&&   &&&&&     ");
	move(dinosaurHead + 6, 0);
	printw(" &&  &&&&&&&&&&&"); //16 
	move(dinosaurHead + 7, 0);
	printw(" &&&&&&&&&&&    ");
	move(dinosaurHead + 8, 0);
	printw("  &&&&&&&&&&    ");
	move(dinosaurHead + 9, 0);
	printw("    &&&&&&&&    ");
	move(dinosaurHead + 10, 0);
	printw("     &&&&&&     ");

	if (legDraw) {
		move(dinosaurHead + 11, 0);
		printw("     &    &&&     ");
		move(dinosaurHead + 12, 0);
		printw("     &&           ");
		legDraw = false;
	}
	else {
		move(dinosaurHead + 11, 0);
		printw("     &&&  &       ");
		move(dinosaurHead + 12, 0);
		printw("          &&      ");
		legDraw = true;
	}
}

void DrawCactus(int cactusHead, int cactusPos)
{
	move(cactusHead, cactusPos);
	printw("$$$$");
	move(cactusHead + 1, cactusPos);
	printw(" $$ ");
	move(cactusHead + 2, cactusPos);
	printw(" $$ ");
	move(cactusHead + 3, cactusPos);
	printw(" $$ ");
	move(cactusHead + 4, cactusPos);
	printw(" $$ ");
}

void DrawScore(int score, int x, int y) {
	move(y + 2, x - 20);
	printw("score : %d", score);
}

int main(int argc, char* argv[])
{
	initscr();

	int winWidth = 0, winHeight = 0;
	int dinosaurHead = 0;
	int cactusHead = 0, cactusPos = 0;
	int getKey = 0;
	int score = 0, acc = 0; //score, acceleration
	bool isJumping = false;
	bool isBottom = true;
	bool isHit = false;

	getmaxyx(stdscr, winHeight, winWidth);

	dinosaurHead = winHeight - DINO_HEIGHT;
	cactusHead = winHeight - CACTUS_HEIGHT;

	cactusPos = winWidth - 20;
	
	while(true){
		getKey = GetKeyDown();

		if(SPACE == getKey && isBottom){
			isJumping = true;
			isBottom = false;
		}else if(ENTER == getKey){
			break;
		}

		if(isJumping){
			dinosaurHead -= 3;
		}else{
			dinosaurHead += 3;
		}

		if(winHeight <= (dinosaurHead + DINO_HEIGHT)){
			dinosaurHead = winHeight - DINO_HEIGHT;
			isBottom = true;
		}

		if(dinosaurHead <= 0){
			isJumping = false;
		}
		
		for(int i = 0; i < 6; i ++) {
			if(dinosaurHead + 8 == cactusHead && 12 - i == cactusPos) {
				isHit = true;
				break;
			}
			else if(dinosaurHead + 12 == cactusHead && 5 + i == cactusPos) {
				isHit = true;
				break;
			} else if(dinosaurHead + 11 == cactusHead && 5 + i == cactusPos) {
                                 isHit = true;
                                 break;
                         }
                         else if(dinosaurHead + 12 == cactusHead && 5 - i == cactusPos) {
                                 isHit = true;
                                 break;
                         }
                         else if(dinosaurHead + 11 == cactusHead && 5 - i == cactusPos) {
                                 isHit = true;
                                 break;
                         }

		}

		cactusPos -= 3 + acc;
		
		if(cactusPos <= 0){
			cactusPos = winWidth - 20;
			if((++score) % 3 == 0)
				acc ++;
			
		}

		

		if(isHit)
			break;
		

		clear();
		DrawCactus(cactusHead, cactusPos);
		DrawDinosaur(dinosaurHead);
		DrawScore(score, winWidth, 0);
		usleep(100000);
		refresh();
	}
	move(winHeight / 2, winWidth / 2 - 10);
	printw("game over. score : %d", score);
	move(winHeight / 2 + 1, winWidth / 2 - 10);
	printw("press space to end the game");
	refresh();
	getKey = GetKeyDown();
	while(getKey != SPACE)
		getKey = GetKeyDown();
	endwin();

	return 0;
}
