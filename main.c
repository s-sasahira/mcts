#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tree.c"

int main(void)
{

    // モンテカルロ木
    Node tree;
    initNode(&tree);

    for (int i = 0; i < NUMBER_OF_SEARCH; i++)
    {
        // 盤面
        int **board = (int **)calloc(NUMBER_OF_SIDE, sizeof(int *));
        for (int o = 0; o < NUMBER_OF_SIDE; o++)
        {
            board[o] = (int *)calloc(NUMBER_OF_SIDE, sizeof(int));
        }

        // displayBoard(board);

        Node *currentNode = &tree;

        // ターンナンバー
        int turnNumber = 0;

        // 石
        int rock = CIRCLE;

        // 勝った方
        int wonRock = NOT_FINISHED;

        while (wonRock == NOT_FINISHED)
        {
            // // ランダムに手を選択し、進める
            // int nextMove = generatePossiblePlace(rock, board);

            // UCBにより、手を評価し、選択する
            createNodeFromPossiblePlace(currentNode, rock, board);
            int nextMove = ucb(*currentNode, i, rock);

            // 手を指す
            handOut(nextMove, rock, board);
            // displayBoard(board);

            // 新たなノードを作成
            turnNumber++;
            Node *node = (Node *)calloc(1, sizeof(Node));
            // printf("turn:%d\n", turnNumber);
            // printf("node:%p\n", node);
            // printf("current:%p\n", currentNode);
            initNode(node);
            (*node).turn = turnNumber;
            (*node).address = nextMove;
            (*node).rock = rock;
            

            // 新たなノードを現在のノードの下に設置（まだ作成されていないノードの場合）
            int nextIndex = deployNode(node, currentNode);

            // 現在のノードを移動
            currentNode = (*currentNode).child[nextIndex];

            // 終了判定
            wonRock = whichWon(board);
            if (wonRock == NOT_FINISHED)
            {
                // 次のターンへ
                rock = switchRock(rock);
            }
            else
            {
                // 盤面を開放
                for (int o = 0; o < NUMBER_OF_SIDE; o++)
                {
                    free(board[o]);
                }
                free(board);
            }
        }

        // 結果を逆伝播する
        for (int j = turnNumber; j > -1; j--)
        {
            // printf("backprop:%d,address:%p\n", j, currentNode);
            // 最後のノードのisEndをtrueに
            if (j == turnNumber)
            {
                (*currentNode).isEnd = true;
            }

            // 通過数と結果を加算
            (*currentNode).throughCount++;
            switch (wonRock)
            {
            case DRAW:
                (*currentNode).drawCount++;
                break;
            case CIRCLE_WON:
                (*currentNode).ciWinCount++;
                break;
            case CROSS_WON:
                (*currentNode).crWinCount++;
                break;
            }

            if ((*currentNode).turn > 0)
            {
                currentNode = (*currentNode).parent;
            }
        }
        printf("serch turn:%d\n", i);
    }

    printf("tree created\n");

    outputTree(tree);

    printf("json file created\n");

    printf("end\n");

    return 0;
}