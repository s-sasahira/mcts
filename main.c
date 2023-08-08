#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tree.c"

int main(void)
{
    outputTree();

    // // モンテカルロ木
    // Node tree;
    // initNode(&tree);

    // for (int i = 0; i < NUMBER_OF_SEARCH; i++)
    // {
    //     // 盤面
    //     int **board = (int **)calloc(NUMBER_OF_SIDE, sizeof(int *));
    //     for (int o = 0; o < NUMBER_OF_SIDE; o++)
    //     {
    //         board[o] = (int *)calloc(NUMBER_OF_SIDE, sizeof(int));
    //     }

    //     displayBoard(board);

    //     Node *currentNode = &tree;

    //     // ターンナンバー
    //     int turnNumber = 0;

    //     // 石
    //     int rock = CIRCLE;

    //     // 勝った方
    //     int wonRock = NOT_FINISHED;

    //     while (wonRock == NOT_FINISHED)
    //     {
    //         // ランダムに手を選択し、進める
    //         int nextMove = generatePossiblePlace(rock, board);
    //         handOut(nextMove, rock, board);
    //         displayBoard(board);

    //         // 新たなノードを作成
    //         turnNumber++;
    //         Node node;
    //         initNode(&node);
    //         // displayNode(*currentNode);
    //         node.turn = turnNumber;
    //         node.address = nextMove;
    //         node.rock = rock;

    //         // 新たなノードを現在のノードの下に設置（まだ作成されていないノードの場合）
    //         deployNode(&node, currentNode);

    //         // 終了判定
    //         wonRock = whichWon(board);
    //         // printf("wonRock:%d\n", wonRock);
    //         if (wonRock == NOT_FINISHED)
    //         {
    //             // 次のターンへ
    //             printf("next turn\n");
    //             rock = switchRock(rock);
    //             turnNumber++;
    //             currentNode = &node;
    //         }
    //         else
    //         {
    //             // 盤面を開放
    //             for (int o = 0; o < NUMBER_OF_SIDE; o++)
    //             {
    //                 free(board[o]);
    //             }
    //             free(board);
    //             printf("break\n");
    //         }
    //     }

    //     // 結果を逆伝播する
    //     for (int j = turnNumber; j > -1; j--)
    //     {
    //         // 最後のノードのisEndをtrueに
    //         if (j == turnNumber)
    //         {
    //             (*currentNode).isEnd = true;
    //         }

    //         // 通過数と結果を加算
    //         (*currentNode).throughCount++;
    //         switch (wonRock)
    //         {
    //         case DRAW:
    //             (*currentNode).drawCount++;
    //             break;
    //         case CIRCLE_WON:
    //             (*currentNode).ciWinCount++;
    //             break;
    //         case CROSS_WON:
    //             (*currentNode).crWinCount++;
    //             break;
    //         }
    //         currentNode = (*currentNode).parent;
    //     }
    // }

    // printf("end\n");

    return 0;
}