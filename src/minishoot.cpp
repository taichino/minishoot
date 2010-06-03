#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "minishoot.h"

static int frameCnt = 0;

static int map[MAP_HEIGHT][MAP_WIDTH];
static MapObj canon;
static MapObj bullet[MAX_BULLET_NUM];
static int liveBulletNum = 0;

static MapObj enemy[MAX_ENEMY_NUM];
static int liveEnemyNum = 0;
static int score;

void debug(const char* text) {
    mvaddstr(30, 10, text);
}
void setMap(int x, int y, int type) {
    map[y][x] = type;
}
int getMap(int x, int y) {
    return map[y][x];
}
void drawMap(int x, int y, int type) {
    switch (type) {
    case MAP_TYPE_SELF:
	attrset(COLOR_PAIR(1));
	break;
    default:
	attrset(0);
    }
    mvaddch(y, x, type);
    move(0, 0);
    attrset(0);
}
void drawText(int x, int y, char* text) {
    mvaddstr(y, x, text);
}

void clearMap() {
    // blank
    for (int i = 0; i < MAP_WIDTH; i++) {
	for (int j = 0; j < MAP_HEIGHT; j++) {
	    setMap(i, j, MAP_TYPE_BLANK);
	}
    }

    // wall
    for (int i = 0; i < MAP_WIDTH; i++) {
	setMap(0, i, MAP_TYPE_WALL);
	setMap(MAP_WIDTH - 1, i, MAP_TYPE_WALL);
    }
    for (int i = 0; i < MAP_WIDTH; i++) {
	setMap(i, MAP_HEIGHT - 1, MAP_TYPE_WALL);
    }
}

void init() {
    // status
    score = 0;
    liveEnemyNum = 0;
    liveBulletNum = 0;
    
    // map
    clearMap();

    // self
    canon.x = MAP_WIDTH  / 2;
    canon.y = MAP_HEIGHT / 2;
    canon.dx = canon.dy = 0;
    canon.type = MAP_TYPE_SELF;
    canon.alive = true;
    canon.speed = 1;
}

void show() {
    // score
    char strScore[10] = {'\0'};
    sprintf(strScore, "score: %d", score);
    drawText((MAP_WIDTH - strlen(strScore)) / 2, MAP_HEIGHT + 2, strScore);
    
    // map
    for (int x = 0; x < MAP_WIDTH; x++) {
	for (int y = 0; y < MAP_HEIGHT; y++) {
	    drawMap(x, y, getMap(x, y));
	}
    }

    if (canon.alive) {
	// canon
	drawMap(canon.x, canon.y, canon.type);

	// bullet
	for (int i = 0; i < liveBulletNum; i++) {
	    MapObj obj = bullet[i];
	    if (obj.alive) {
		drawMap(obj.x, obj.y, obj.type);
	    }
	}

	// enemy
	for (int i = 0; i < liveEnemyNum; i++) {
	    MapObj obj = enemy[i];
	    if (obj.alive) {
		drawMap(obj.x, obj.y, obj.type);
	    }
	}
    }
}

void moveObj(MapObj& obj) {
    if (frameCnt % obj.speed == 0) {
	obj.x += obj.dx;
	obj.y += obj.dy;
    }
}

bool isOut(MapObj& obj) {
    return (obj.x < 0 || obj.x >= MAP_WIDTH ||
	    obj.y < 0 || obj.y >= MAP_HEIGHT);
}

void shot() {
    if (liveBulletNum < MAX_BULLET_NUM) {
	MapObj shot = {canon.x, canon.y - 1, 0, -1, MAP_TYPE_SHOT, true, 2};
	bullet[liveBulletNum] = shot;
	liveBulletNum++;
    }
}

void moveObj() {
    moveObj(canon);

    // bullet
    for (int i = 0; i < liveBulletNum; i++) {
	moveObj(bullet[i]);
    }

    // enemy
    for (int i = 0; i < liveEnemyNum; i++) {
	moveObj(enemy[i]);
    }
}

void addEnemy() {
    MapObj enem = {(rand() % (MAP_WIDTH - 2)) + 1, 0, 0, 1, MAP_TYPE_ENEM, true, 4};
    enemy[liveEnemyNum] = enem;
    liveEnemyNum++;
}

void action() {
    // 弾と敵の判定処理
    for (int i = 0; i < liveBulletNum; i++) {
	MapObj& b = bullet[i];
	if (!b.alive) {
	    continue;
	}
	for (int j = 0; j < liveEnemyNum; j++) {
	    MapObj& e = enemy[j];
	    if (b.x == e.x && abs(b.y - e.y) <= 1 && b.alive && e.alive) {
		// 倒した
		b.alive = false;
		e.alive = false;
		score++;
		break;
	    }
	}
    }

    // 敵と自機の当り判定
    for (int j = 0; j < liveEnemyNum; j++) {
	MapObj& e = enemy[j];
	if (canon.x == e.x && canon.y == e.y && e.alive) {
	    // 死んだ
	    canon.alive = false;
	    break;
	}
    }
    
    // 自機場外死亡処理
    canon.alive = canon.alive && !isOut(canon);

    // 弾の場外処理
    for (int i = 0; i < liveBulletNum; i++) {
	bullet[i].alive = bullet[i].alive && !isOut(bullet[i]);
    }

    // 敵の場外処理
    for (int i = 0; i < liveEnemyNum; i++) {
	enemy[i].alive = enemy[i].alive && !isOut(enemy[i]);
    }

    // 後処理
    int liveCount = 0;
    for (int i = 0; i < liveBulletNum; i++) {
	if (bullet[i].alive) {
	    bullet[liveCount++] = bullet[i];
	}
    }
    liveBulletNum = liveCount;
    
    liveCount = 0;
    for (int i = 0; i < liveEnemyNum; i++) {
	if (enemy[i].alive) {
	    enemy[liveCount++] = enemy[i];
	}
    }
    liveEnemyNum = liveCount;
}

void game(int ch) {
    canon.dx = canon.dy = 0;
    switch (ch) {
    case KEY_LEFT:
	canon.dx = -1;
	canon.dy = 0;
	break;
    case KEY_DOWN:
	canon.dx = 0;
	canon.dy = 1;
	break;
    case KEY_UP:
	canon.dx = 0;
	canon.dy = -1;
	break;
    case KEY_RIGHT:
	canon.dx = 1;
	canon.dy = 0;
	break;
    case 'z':
	shot();
	break;
    }

    moveObj();
    action();
    if (rand() % 10 < 1) {
	addEnemy();
    }
    show();
}

void gameover(int ch) {
    const char* text = "gameover";
    int x = (MAP_WIDTH - strlen(text)) / 2 - 1;
    int y = (MAP_HEIGHT) / 2 - 1;
    
    mvaddstr(y, x, text);
    mvaddstr(y+1, x, "r: restart");
    mvaddstr(y+2, x, "q: quit");
    move(0, 0);

    switch (ch) {
    case 'r':
	init();
	break;
    }
}

int main() {
    initscr();
    noecho();
    cbreak();

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    srand(time(NULL));
    keypad(stdscr, true);
    wtimeout(stdscr, 100); // 10フレーム毎秒    

    init();

    int ch;
    while ((ch = getch()) != 'q') {
	if (canon.alive) {
	    game(ch);
	}
	else {
	    gameover(ch);
	}
    	refresh();
	frameCnt++;
    }
    
    endwin();
    return 0;
}
