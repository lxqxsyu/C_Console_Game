#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define ROW_MAP 10       //��ͼ����
#define COL_MAP 20       //��ͼ����

const char food = '#';
const char snake = '*';

enum Direction {       //�����ߵķ���
    R, //��
    L, //��
    U, //��
    D  //��
} Direction;

enum State {        //��Ϸ״̬
    ERROR_SELF, //ҧ���Լ�
    ERROR_WALL, //ײ��ǽ
    NORMAL,     //����״̬
    SUCCESS     //ͨ��
} State;


typedef struct Snake{
    size_t x; //��
    size_t y; //��
    struct Snake *next;
} Snake, *pSnake;

pSnake head = NULL;     //������ͷָ��
pSnake Food = NULL;     //����ʳ��ָ��

void showMenu();                //��ʾ�˵�

void cursorPoint(int, int);     //���λ��
void initMap();                 //��ʼ����ͼ

void printSnake();               //�������
void initSnake();               //��ʼ������

void createFood();              //����ʳ��

void controllSnake();            //�������ƶ�
void snakeMove();                //���ƶ������ݱ仯��

int isCrossWall();              //�ж��Ƿ�ײǽ
int isFood(pSnake);             //�ж��Ƿ���ʳ��
int isEatSelf(pSnake);          //�ж��Ƿ�ҧ���Լ�

int main(int argc, char const *argv[]){

    while(1){

        showMenu();

        //�Ƿ���enter
        if(GetAsyncKeyState(VK_RETURN)){ 

            system("cls"); //�����ʾ

            initMap();
            initSnake();
            createFood();

            Direction = R;  //�߳�ʼ���߷���Ϊ��
            State = NORMAL; //��Ϸ��ʼΪ����״̬

            while(1){

                controllSnake(); //���ƺ�ִ��snakeMove()

                //�ж���Ϸ״̬
                if (State == ERROR_SELF){
                        system("cls");
                        printf("���ź�����ҧ���Լ�����Ϸʧ�ܣ�\n");
                }else if (State == ERROR_WALL){
                        system("cls");
                        printf("���ź���������ǽ�ڣ���Ϸʧ�ܣ�\n");
                }else if (State == SUCCESS){
                        system("cls");
                        printf("��ϲ������ͨ�أ�����\n");
                }
            }
        }else if (GetAsyncKeyState(VK_ESCAPE)){
            exit(0);
        }
    }
    return 0;
}

int isEatSelf(pSnake newHead){ //�ж��Ƿ�ҧ���Լ�
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

int isCrossWall(){ //�ж��Ƿ�����ǽ
    if (head->x <= 0 || head->x >= ROW_MAP - 1 || head->y <= 1 || head->y >= COL_MAP - 2){
        State = ERROR_WALL;
        return 1;
    }
    return 0;
}

int isFood(pSnake pos){ //�жϸ�λ���ǲ���ʳ��
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

    if(isFood(newHead)){ //��ʳ��
        head = newHead;
        printSnake();
        createFood();
    }else if(isCrossWall() || isEatSelf(newHead)){
        free(newHead);
        newHead = NULL;
    }else{
        head = newHead;
        pSnake cur = head;
        //ɾ����β����ӡ
        while (cur->next->next != NULL){
            cursorPoint(cur->y, cur->x);
            printf("%c", snake);
            cur = cur->next;
        }

        cursorPoint(cur->y, cur->x);
        printf("%c", snake);
        cursorPoint(cur->next->y, cur->next->x);
        printf(" "); //��ӡ�ո�������ƵĻ�ϵ���β
        free(cur->next);
        cur->next = NULL;
    }
}

void controllSnake(){
    if (GetAsyncKeyState(VK_UP) && Direction != D){ //����
        Direction = U;
    }else if (GetAsyncKeyState(VK_DOWN) && Direction != U){ //����
        Direction = D;
    }else if (GetAsyncKeyState(VK_LEFT) && Direction != R){ //����
        Direction = L;
    }else if (GetAsyncKeyState(VK_RIGHT) && Direction != L){//����
        Direction = R;
    }else if (GetAsyncKeyState(VK_ESCAPE)){  //Esc�˳�
        exit(0);
    }

    snakeMove();

    Sleep(500);
}

void createFood(){
    pSnake cur = head;
    Food = (pSnake)malloc(sizeof(Snake));

    //����x~y������� k=rand()%(Y-X+1)+X;
    srand((unsigned)time(NULL));
    Food->x = rand() % (ROW_MAP - 2 - 1 + 1) + 1;
    Food->y = rand() % (COL_MAP - 3 - 2 + 1) + 2;
    Food->next = NULL;
    while (cur){//���ʳ���Ƿ��������غ�
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

    COORD pos;  //posΪ�ṹ�壬��¼����λ��
    pos.X = x; 
    pos.Y = y;  

    //��ȡ��׼�����������ƹ��Ϊpos
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

void showMenu(){
    system("color 0C");  //���ÿ���̨�����ɫ
    printf("*******************************************************\n");
    printf("*        Welcome to Snake Game!        *\n");
    printf("*                           *\n");
    printf("*       ->��ʼ��Ϸ�밴 enter��         *\n");
    printf("*       ->�˳���Ϸ�밴 esc��          *\n");
    printf("*       ->��ͣ��Ϸ�밴 space��         *\n");
    printf("*       ->ͨ���������Ҽ��������ߵ��ƶ�     *\n");
    printf("*       ->ͨ��F1������    F2������     *\n");
    printf("*******************************************************\n");
    system("pause");   //�ȴ��û�����...
}

void initMap(){
    int i = 0;
    for (i = 0; i < COL_MAP; i += 2){ //��ӡ���±߿�ÿ����ռ�����У�
        cursorPoint(i, 0);
        printf("��");
        cursorPoint(i, ROW_MAP - 1);
        printf("��");
    }
    for (i = 0; i < ROW_MAP; i++){ //��ӡ���ұ߿�
        cursorPoint(0, i);
        printf("��");
        cursorPoint(COL_MAP - 2, i);
        printf("��");
    }
}

void initSnake(){
    int initNum = 3; //����������
    
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


