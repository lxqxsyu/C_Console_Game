#include "Snake.h"

pSnake head = NULL; //������ͷָ��
pSnake Food = NULL; //����ʳ��ָ��

int sleeptime = 500; //���ʱ�䣬���������ٶ�
int Score = 0;       //�ܷ�
int everyScore = 1;  //ÿ���÷�

//������Ϸ���õ��ķ���
const char food = '#';
const char snake = '*';

void Pos(int x, int y) //����������
{
    COORD pos;                                                      //posΪ�ṹ��
    pos.X = x;                                                      //������
    pos.Y = y;                                                      //������
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //��ȡ��׼�����������ƹ��Ϊpos
}

void Face()
{
    system("color 0C");
    printf("*******************************************************\n");
    printf("*        Welcome to Snake Game!        *\n");
    printf("*                           *\n");
    printf("*       ->��ʼ��Ϸ�밴 enter��         *\n");
    printf("*       ->�˳���Ϸ�밴 esc��          *\n");
    printf("*       ->��ͣ��Ϸ�밴 space��         *\n");
    printf("*       ->ͨ���������Ҽ��������ߵ��ƶ�     *\n");
    printf("*       ->ͨ��F1������    F2������     *\n");
    printf("*******************************************************\n");
}
void Map() //��ʼ����ͼ
{
    int i = 0;
    for (i = 0; i < COL_MAP; i += 2) //��ӡ���±߿�ÿ����ռ�����У�
    {
        Pos(i, 0);
        printf("��");
        Pos(i, ROW_MAP - 1);
        printf("��");
    }
    for (i = 0; i < ROW_MAP; i++) //��ӡ���ұ߿�
    {
        Pos(0, i);
        printf("��");
        Pos(COL_MAP - 2, i);
        printf("��");
    }
}
void PrintSnake() //��ӡ��
{
    pSnake cur = head;
    while (cur)
    {
        Pos(cur->y, cur->x);
        printf("%c", snake);
        cur = cur->next;
    }
}
void InitSnake() //��ʼ������
{
    int initNum = 3;
    int i = 0;
    pSnake cur;
    head = (pSnake)malloc(sizeof(Snake));
    head->x = 5;
    head->y = 10;
    head->next = NULL;

    cur = head;
    for (i = 1; i < initNum; i++)
    {
        pSnake newNode = (pSnake)malloc(sizeof(Snake));
        newNode->x = 5 + i;
        newNode->y = 10;
        newNode->next = NULL;
        cur->next = newNode;
        cur = cur->next;
    }

    PrintSnake();
}

void CreateFood() //�ڵ�ͼ���������һ��ʳ��
{
    pSnake cur = head;
    Food = (pSnake)malloc(sizeof(Snake));

    //����x~y������� k=rand()%(Y-X+1)+X;
    srand((unsigned)time(NULL));
    Food->x = rand() % (ROW_MAP - 2 - 1 + 1) + 1;
    Food->y = rand() % (COL_MAP - 3 - 2 + 1) + 2;
    Food->next = NULL;
    while (cur) //���ʳ���Ƿ��������غ�
    {
        if (cur->x == Food->x && cur->y == Food->y)
        {
            free(Food);
            Food = NULL;
            CreateFood();
            return;
        }
        cur = cur->next;
    }
    Pos(Food->y, Food->x);
    printf("%c", food);
}

void StartGame() //��Ϸ��ʼ����������
{
    Face();
    system("pause");

    if (GetAsyncKeyState(VK_RETURN))
    {
        system("cls");
        Pos(COL_MAP + 5, 1);
        printf("��ǰ����/ͨ�ط���:");
        Pos(COL_MAP + 20, 1);
        printf("%d/%d", Score, SUCCESS_SCORE);
        Pos(COL_MAP + 5, 2);
        printf("��ǰ��ÿ���÷�:");
        Pos(COL_MAP + 20, 2);
        printf("%d", everyScore);
        Pos(COL_MAP + 5, 3);
        printf("\n");
        Pos(COL_MAP + 5, 4);
        printf("�ٶ�Խ�� �÷�Խ��Ŷ����\n");

        Map();
        InitSnake();
        CreateFood();
    }
    else if (GetAsyncKeyState(VK_ESCAPE))
    {
        exit(0);
    }
}

int IsCrossWall() //�ж��Ƿ�����ǽ
{
    if (head->x <= 0 || head->x >= ROW_MAP - 1 || head->y <= 1 || head->y >= COL_MAP - 2)
    {
        State = ERROR_WALL;
        return 0;
    }
    return 1;
}

int IsEatSelf(pSnake newHead) //�ж��Ƿ�ҧ���Լ�
{
    pSnake cur = head;
    assert(newHead);
    while (cur)
    {
        if (cur->x == newHead->x && cur->y == newHead->y)
        {
            State = ERROR_SELF;
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

int IsFood(pSnake pos) //�жϸ�λ���ǲ���ʳ��
{
    assert(pos);
    if (pos->x == Food->x && pos->y == Food->y)
    {
        return 1;
    }
    return 0;
}

void SnakeMove() //���ƶ�һ��
{
    pSnake newHead = NULL;
    newHead = (pSnake)malloc(sizeof(Snake));

    if (Direction == R)
    {
        newHead->x = head->x;
        newHead->y = head->y + 1;
        newHead->next = head;
    }
    else if (Direction == L)
    {
        newHead->x = head->x;
        newHead->y = head->y - 1;
        newHead->next = head;
    }
    else if (Direction == U)
    {
        newHead->x = head->x - 1;
        newHead->y = head->y;
        newHead->next = head;
    }
    else if (Direction == D)
    {
        newHead->x = head->x + 1;
        newHead->y = head->y;
        newHead->next = head;
    }

    if (IsFood(newHead))
    {
        head = newHead;
        PrintSnake();
        CreateFood();
        Score += everyScore;
        Pos(COL_MAP + 20, 1);
        printf("%d/%d", Score, SUCCESS_SCORE);
        if (Score >= SUCCESS_SCORE)
        {
            State = SUCCESS;
        }
    }
    else
    {
        if (IsCrossWall() && IsEatSelf(newHead))
        {
            pSnake cur = NULL;
            head = newHead;
            cur = head;
            //ɾ����β����ӡ
            while (cur->next->next != NULL)
            {
                Pos(cur->y, cur->x);
                printf("%c", snake);
                cur = cur->next;
            }

            Pos(cur->y, cur->x);
            printf("%c", snake);
            Pos(cur->next->y, cur->next->x);
            printf(" "); //��ӡ�ո�������ƵĻ�ϵ���β
            free(cur->next);
            cur->next = NULL;
        }
        else
        {
            free(newHead);
            newHead = NULL;
        }
    }
}

void Pause()
{
    while (1)
    {
        Sleep(sleeptime);
        if (GetAsyncKeyState(VK_SPACE))
        {
            break;
        }
    }
}

void ControlSnake() //�ü��̿�����Ϸ
{
    if (GetAsyncKeyState(VK_UP) && Direction != D)
    {
        Direction = U;
    }
    else if (GetAsyncKeyState(VK_DOWN) && Direction != U)
    {
        Direction = D;
    }
    else if (GetAsyncKeyState(VK_LEFT) && Direction != R)
    {
        Direction = L;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && Direction != L)
    {
        Direction = R;
    }
    else if (GetAsyncKeyState(VK_F1))
    {
        if (sleeptime != 500)
        {
            sleeptime = 500;
            everyScore = 1;
            Pos(COL_MAP + 20, 2);
            printf("%d", everyScore);
        }
    }
    else if (GetAsyncKeyState(VK_F2))
    {
        if (sleeptime != 300)
        {
            sleeptime = 300;
            everyScore = 2;
            Pos(COL_MAP + 20, 2);
            printf("%d", everyScore);
        }
    }
    else if (GetAsyncKeyState(VK_SPACE))
    {
        Pause();
    }
    else if (GetAsyncKeyState(VK_ESCAPE))
    {
        exit(0);
    }
}

void StateGame() //�ж���Ϸʧ�ܻ�ɹ�
{
    if (State == ERROR_SELF)
    {
        system("cls");
        printf("���ź�����ҧ���Լ�����Ϸʧ�ܣ�\n");
    }
    else if (State == ERROR_WALL)
    {
        system("cls");
        printf("���ź���������ǽ�ڣ���Ϸʧ�ܣ�\n");
    }
    else if (State == SUCCESS)
    {
        system("cls");
        printf("��ϲ������ͨ�أ�����\n");
    }
}
void RunGame()
{
    Direction = R;  //�߳�ʼ���߷���Ϊ��
    State = NORMAL; //��Ϸ��ʼΪ����״̬
    while (1)
    {
        ControlSnake();
        SnakeMove();
        if (State != NORMAL)
        {
            StateGame();
            break;
        }
        Sleep(sleeptime);
    }
}

void EndGame() //�ͷ������ָ�Ĭ��ֵ
{
    pSnake cur = head;
    while (cur)
    {
        pSnake del = cur;
        cur = cur->next;
        free(del);
        del = NULL;
    }
    head = NULL;
    if (Food != NULL)
    {
        free(Food);
        Food = NULL;
    }
    Score = 0;
    everyScore = 1;
    sleeptime = 500;
}