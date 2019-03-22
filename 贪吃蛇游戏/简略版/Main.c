#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define ROW_MAP 10       //地图的行
#define COL_MAP 20       //地图的列

const char food = '#';
const char snake = '*';

enum Direction {       //蛇行走的方向
    R, //右
    L, //左
    U, //上
    D  //下
} Direction;

enum State {        //游戏状态
    ERROR_SELF, //咬到自己
    ERROR_WALL, //撞到墙
    NORMAL,     //正常状态
    SUCCESS     //通关
} State;


typedef struct Snake{
    size_t x; //行
    size_t y; //列
    struct Snake *next;
} Snake, *pSnake;

pSnake head = NULL;     //定义蛇头指针
pSnake Food = NULL;     //定义食物指针

void showMenu();                //显示菜单

void cursorPoint(int, int);     //光标位置
void initMap();                 //初始化地图

void printSnake();               //输出蛇身
void initSnake();               //初始化蛇身

void createFood();              //创建食物

void controllSnake();            //控制蛇移动
void snakeMove();                //蛇移动（数据变化）

int isCrossWall();              //判断是否撞墙
int isFood(pSnake);             //判断是否是食物
int isEatSelf(pSnake);          //判断是否咬到自己

int main(int argc, char const *argv[]){

    while(1){

        showMenu();

        //是否按了enter
        if(GetAsyncKeyState(VK_RETURN)){ 

            system("cls"); //清除显示

            initMap();
            initSnake();
            createFood();

            Direction = R;  //蛇初始行走方向为右
            State = NORMAL; //游戏初始为正常状态

            while(1){

                controllSnake(); //控制后执行snakeMove()

                //判断游戏状态
                if (State == ERROR_SELF){
                        system("cls");
                        printf("很遗憾，蛇咬到自己，游戏失败！\n");
                }else if (State == ERROR_WALL){
                        system("cls");
                        printf("很遗憾，蛇碰到墙壁，游戏失败！\n");
                }else if (State == SUCCESS){
                        system("cls");
                        printf("恭喜您，已通关！！！\n");
                }
            }
        }else if (GetAsyncKeyState(VK_ESCAPE)){
            exit(0);
        }
    }
    return 0;
}

int isEatSelf(pSnake newHead){ //判断是否咬到自己
    pSnake cur = head;
    assert(newHead);
    while (cur){
        if (cur->x == newHead->x && cur->y == newHead->y){
            State = ERROR_SELF;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

int isCrossWall(){ //判断是否碰到墙
    if (head->x <= 0 || head->x >= ROW_MAP - 1 || head->y <= 1 || head->y >= COL_MAP - 2){
        State = ERROR_WALL;
        return 1;
    }
    return 0;
}

int isFood(pSnake pos){ //判断该位置是不是食物
    assert(pos);
    if (pos->x == Food->x && pos->y == Food->y){
        return 1;
    }
    return 0;
}

void snakeMove(){
    pSnake newHead = (pSnake)malloc(sizeof(Snake));

    if (Direction == R){
        newHead->x = head->x;
        newHead->y = head->y + 1;
        newHead->next = head;
    }else if (Direction == L){
        newHead->x = head->x;
        newHead->y = head->y - 1;
        newHead->next = head;
    }else if (Direction == U){
        newHead->x = head->x - 1;
        newHead->y = head->y;
        newHead->next = head;
    }else if (Direction == D){
        newHead->x = head->x + 1;
        newHead->y = head->y;
        newHead->next = head;
    }

    if(isFood(newHead)){ //是食物
        head = newHead;
        printSnake();
        createFood();
    }else if(isCrossWall() || isEatSelf(newHead)){
        free(newHead);
        newHead = NULL;
    }else{
        head = newHead;
        pSnake cur = head;
        //删除蛇尾并打印
        while (cur->next->next != NULL){
            cursorPoint(cur->y, cur->x);
            printf("%c", snake);
            cur = cur->next;
        }

        cursorPoint(cur->y, cur->x);
        printf("%c", snake);
        cursorPoint(cur->next->y, cur->next->x);
        printf(" "); //打印空格来覆盖频幕上的蛇尾
        free(cur->next);
        cur->next = NULL;
    }
}

void controllSnake(){
    if (GetAsyncKeyState(VK_UP) && Direction != D){ //向下
        Direction = U;
    }else if (GetAsyncKeyState(VK_DOWN) && Direction != U){ //向上
        Direction = D;
    }else if (GetAsyncKeyState(VK_LEFT) && Direction != R){ //向右
        Direction = L;
    }else if (GetAsyncKeyState(VK_RIGHT) && Direction != L){//向左
        Direction = R;
    }else if (GetAsyncKeyState(VK_ESCAPE)){  //Esc退出
        exit(0);
    }

    snakeMove();

    Sleep(500);
}

void createFood(){
    pSnake cur = head;
    Food = (pSnake)malloc(sizeof(Snake));

    //产生x~y的随机数 k=rand()%(Y-X+1)+X;
    srand((unsigned)time(NULL));
    Food->x = rand() % (ROW_MAP - 2 - 1 + 1) + 1;
    Food->y = rand() % (COL_MAP - 3 - 2 + 1) + 2;
    Food->next = NULL;
    while (cur){//检查食物是否与蛇身重合
        if (cur->x == Food->x && cur->y == Food->y){
            free(Food);
            Food = NULL;
            createFood();
            return;
        }
        cur = cur->next;
    }
    cursorPoint(Food->y, Food->x);
    printf("%c", food);
}

void cursorPoint(int x, int y){

    COORD pos;  //pos为结构体，记录坐标位置
    pos.X = x; 
    pos.Y = y;  

    //读取标准输出句柄来控制光标为pos
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

void showMenu(){
    system("color 0C");  //设置控制台输出颜色
    printf("*******************************************************\n");
    printf("*        Welcome to Snake Game!        *\n");
    printf("*                           *\n");
    printf("*       ->开始游戏请按 enter键         *\n");
    printf("*       ->退出游戏请按 esc键          *\n");
    printf("*       ->暂停游戏请按 space键         *\n");
    printf("*       ->通过上下左右键来控制蛇的移动     *\n");
    printf("*       ->通过F1键减速    F2键加速     *\n");
    printf("*******************************************************\n");
    system("pause");   //等待用户输入...
}

void initMap(){
    int i = 0;
    for (i = 0; i < COL_MAP; i += 2){ //打印上下边框（每个■占用两列）
        cursorPoint(i, 0);
        printf("■");
        cursorPoint(i, ROW_MAP - 1);
        printf("■");
    }
    for (i = 0; i < ROW_MAP; i++){ //打印左右边框
        cursorPoint(0, i);
        printf("■");
        cursorPoint(COL_MAP - 2, i);
        printf("■");
    }
}

void initSnake(){
    int initNum = 3; //创建三个蛇
    
    head = (pSnake)malloc(sizeof(Snake));
    head->x = 5;
    head->y = 10;
    head->next = NULL;
    
    pSnake cur = head;
    for (int i = 1; i < initNum; i++){
        pSnake newNode = (pSnake)malloc(sizeof(Snake));
        newNode->x = 5 + i;
        newNode->y = 10;
        newNode->next = NULL;
        cur->next = newNode;
        cur = cur->next;
    }

    printSnake();
}

void printSnake(){
    pSnake cur = head;
    while (cur){
        cursorPoint(cur->y, cur->x);
        printf("%c", snake);
        cur = cur->next;
    }
}


