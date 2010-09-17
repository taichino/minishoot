
#define MAP_WIDTH  20
#define MAP_HEIGHT 15

#define MAP_TYPE_BLANK ' '
#define MAP_TYPE_WALL  '#'
#define MAP_TYPE_SELF  '@'
#define MAP_TYPE_SHOT  '^'
#define MAP_TYPE_ENEM  'Q'

#define MAX_BULLET_NUM 3
#define MAX_ENEMY_NUM 20

typedef struct _obj {
    int x, y;
    int dx, dy;
    int type;
    int alive;
    int speed;
} MapObj;

void debug(const char* text);
void setMap(int x, int y, int type);
int getMap(int x, int y);
void drawMap(int x, int y, int type);
void drawText(int x, int y, char* text);
void clearMap();
void init();
void show();
void moveObj(MapObj& obj);
bool isOut(MapObj& obj);
void shot();
void moveObj();
void addEnemy();
void action();
void game(int ch);
void gameover(int ch);

