#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <malloc.h>
#include <assert.h>
//��ʶ��ͼ��С
#define ROW_MAP 10       //��ͼ����
#define COL_MAP 20       //��ͼ����
#define SUCCESS_SCORE 10 //ͨ�ط���
enum Direction           //�����ߵķ���
{
    R, //��
    L, //��
    U, //��
    D  //��
} Direction;

enum State
{
    ERROR_SELF, //ҧ���Լ�
    ERROR_WALL, //ײ��ǽ
    NORMAL,     //����״̬
    SUCCESS     //ͨ��
} State;

typedef struct Snake
{
    size_t x; //��
    size_t y; //��
    struct Snake *next;
} Snake, *pSnake;

void StartGame();
void RunGame();
void EndGame();

#endif