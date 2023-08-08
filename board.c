#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_SIDE 3
#define NUMBER_OF_SQUARES 9

enum MASS_STATUS {
    NON = 0,
    CIRCLE = 1,
    CROSS = 2
};

enum END_STATUS {
    NOT_FINISHED = -1,
    DRAW = 0,
    CIRCLE_WON = 1,
    CROSS_WON = 2
};

char convertSign(int mass)
{
    if (mass == CIRCLE)
    {
        return 'o';
    }
    else if (mass == CROSS)
    {
        return 'x';
    }
    else
    {
        return ' ';
    }
}

void displayBoard(int **board)
{
    printf("| |0|1|2|\n");
    printf("|0|%c|%c|%c|\n", convertSign(board[0][0]), convertSign(board[0][1]), convertSign(board[0][2]));
    printf("|1|%c|%c|%c|\n", convertSign(board[1][0]), convertSign(board[1][1]), convertSign(board[1][2]));
    printf("|2|%c|%c|%c|\n", convertSign(board[2][0]), convertSign(board[2][1]), convertSign(board[2][2]));
}

int randBetween(int max, int min)
{
    struct timeval t1;
    mingw_gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
    // srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
    return (rand()%(max - min + 1)) + min;
}

int whichWon(int **board)
{
    // 横
    for (int i = 0; i < NUMBER_OF_SIDE; i++)
    {
        int ms = board[i][0];
        for (int j = 1; j < NUMBER_OF_SIDE; j++)
        {
            if (ms == NON || ms != board[i][j])
            {
                break;
            }
            else if (j == NUMBER_OF_SIDE - 1)
            {
                return board[i][j];
            }
        }
    }

    // 縦
    for (int i = 0; i < NUMBER_OF_SIDE; i++)
    {
        int ms = board[0][i];
        for (int j = 1; j < NUMBER_OF_SIDE; j++)
        {
            if (ms == NON || ms != board[j][i])
            {
                break;
            }
            else if (j == NUMBER_OF_SIDE - 1)
            {
                return board[j][i];
            }
        }
    }

    // 斜め 左上から右下
    int ms = board[0][0];
    for (int i = 1; i < NUMBER_OF_SIDE; i++)
    {
        if (ms == NON || ms != board[i][i])
        {
            break;
        }
        else if (i == NUMBER_OF_SIDE - 1)
        {
            return board[i][i];
        }
    }

    // 斜め 右上から左下
    ms = board[0][NUMBER_OF_SIDE - 1];
    for (int i = 1; i < NUMBER_OF_SIDE; i++)
    {
        if (ms == NON || ms != board[i][NUMBER_OF_SIDE - 1 - i])
        {
            break;
        }
        else if (i == NUMBER_OF_SIDE - 1)
        {
            return board[i][NUMBER_OF_SIDE - 1 - i];
        }
    }

    // 全てのマスが埋まっている
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        if (board[i / 3][i % 3] == NON)
        {
            break;
        }
        else if (i == NUMBER_OF_SQUARES - 1)
        {
            return DRAW;
        }
    }

    // 終了条件を満たしていないとき
    return NOT_FINISHED;
}

// 石を切り替える
int switchRock(int rock)
{
    if (rock == CIRCLE)
    {
        return CROSS;
    }
    else if (rock == CROSS)
    {
        return CIRCLE;
    }
    else
    {
        return NON;
    }
}

// 指定の場所に石を置くことができるか
bool possibleMove(int address, int rock, int **board)
{
    return board[address / NUMBER_OF_SIDE][address % NUMBER_OF_SIDE] == NON;
}

// 石を置ける場所を抽出し、そこからランダムで手を選ぶ
int generatePossiblePlace(int rock, int **board)
{
    int ablePlace[NUMBER_OF_SQUARES];
    int ablePlaceCount = 0;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        if (possibleMove(i, rock, board))
        {
            ablePlace[ablePlaceCount] = i;
            ablePlaceCount++;
        }
    }
    return ablePlace[randBetween(ablePlaceCount - 1, 0)];
}

// 手を指す
void handOut(int address, int rock, int **board)
{
    board[address / NUMBER_OF_SIDE][address % NUMBER_OF_SIDE] = rock;
}