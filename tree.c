#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "board.c"

#define NUMBER_OF_SEARCH 1000
#define NUMBER_OF_CHARACTERS_FOR_A_NODE 235

// モンテカルロ木のノード
typedef struct Node
{
    // 前のノード
    struct Node *parent;

    // 次のノード数
    int childCount;

    // 次のノード
    struct Node **child;

    // ターン数
    int turn;

    // アドレス
    int address;

    // 石の種類
    int rock;

    // 有効
    bool isEnable;

    // 終了
    bool isEnd;

    // 通過数
    int throughCount;

    // 引分数
    int drawCount;

    // 勝利数 CIRCLE
    int ciWinCount;

    // 勝利数 CROSS
    int crWinCount;
} Node;

void initNode(Node *node)
{
    (*node).parent = NULL;
    (*node).childCount = 0;
    (*node).child = NULL;
    (*node).turn = 0;
    (*node).address = 0;
    (*node).rock = 0;
    (*node).isEnable = true;
    (*node).isEnd = false;
    (*node).throughCount = 0;
    (*node).drawCount = 0;
    (*node).ciWinCount = 0;
    (*node).crWinCount = 0;
}

void displayNode(Node node)
{
    // printf("parent:%d\n", node.parent);
    printf("childCount:%d\n", node.childCount);
    // printf("child:%d\n", node.child);
    printf("turn:%d\n", node.turn);
    printf("address:%d\n", node.address);
    printf("rock:%d\n", node.rock);
    printf("isEnable:%d\n", node.isEnable);
    printf("isEnd:%d\n", node.isEnd);
    printf("throughCount:%d\n", node.throughCount);
    printf("drawCount:%d\n", node.drawCount);
    printf("ciWinCount:%d\n", node.ciWinCount);
    printf("crWinCount:%d\n", node.crWinCount);
}

// ノードが既に作成済みの場合インデックスを返す
int isCreated(Node *child, Node *parent)
{
    for (int i = 0; i < (*parent).childCount; i++)
    {
        bool addEq = (*(*parent).child[i]).address == (*child).address;
        bool rockEq = (*(*parent).child[i]).rock == (*child).rock;
        if (addEq && rockEq)
        {
            return i;
        }
    }
    return -1;
}

// ノードを文字列に変換
void tostringNode(char *str, struct Node node)
{
    char *comma = "";
    if (node.turn > 0)
    {
        if ((*node.parent).childCount > 1 && isCreated(&node, node.parent) > 0)
        {
            comma = ",";
        }
    }

    char *childstr = (char *)calloc(NUMBER_OF_SEARCH * NUMBER_OF_SQUARES * NUMBER_OF_SQUARES * NUMBER_OF_CHARACTERS_FOR_A_NODE, sizeof(char));
    for (int i = 0; i < node.childCount; i++)
    {
        char *chil = (char *)calloc(NUMBER_OF_SEARCH * NUMBER_OF_SQUARES * NUMBER_OF_SQUARES * NUMBER_OF_CHARACTERS_FOR_A_NODE, sizeof(char));
        tostringNode(chil, *node.child[i]);
        strcat(childstr, chil);
        free(chil);
    }

    sprintf(str, "%s{"\
        "\"turn\":%d,"\
        "\"address\":%d,"\
        "\"rock\":%d,"\
        "\"throughCount\":%d,"\
        "\"drawCount\":%d,"\
        "\"ciWinCount\":%d,"\
        "\"crWinCount\":%d,"\
        "\"childCount\":%d,"\
        "\"child\":[%s]"\
    "}",
        comma, node.turn, node.address, node.rock, 
        node.throughCount, 
        node.drawCount, node.ciWinCount, node.crWinCount,
        node.childCount, childstr);

    free(childstr);
}

// 文字列をノードに変換
void toNodeString(struct Node *node, char *str)
{
    printf("start\n");

    struct Node one;

    char *comma = "";
    char *childstr = (char *)calloc(NUMBER_OF_SEARCH * NUMBER_OF_SQUARES * NUMBER_OF_SQUARES * NUMBER_OF_CHARACTERS_FOR_A_NODE, sizeof(char));

    sscanf(str, "{"\
        "\"turn\":%d,"\
        "\"address\":%d,"\
        "\"rock\":%d,"\
        "\"throughCount\":%d,"\
        "\"drawCount\":%d,"\
        "\"ciWinCount\":%d,"\
        "\"crWinCount\":%d,"\
        "\"childCount\":%d,"\
        "\"child\":[%s]"\
    "}",
        &one.turn, &one.address, &one.rock, 
        &one.throughCount, 
        &one.drawCount, &one.ciWinCount, &one.crWinCount,
        &one.childCount, childstr);
    printf("success\n");


    printf("childstr:%s\n", childstr);
}

void displayTree(Node topNode)
{
    // 木を表示
}

void outputTree(Node tree)
{
    // 木を出力

    FILE *file;

    file = fopen("tree.json", "w");

    char *str = (char *)calloc(NUMBER_OF_SEARCH * NUMBER_OF_SQUARES * NUMBER_OF_SQUARES * NUMBER_OF_CHARACTERS_FOR_A_NODE, sizeof(char));
    tostringNode(str, tree);

    fprintf(file, "%s", str);

    free(str);

    fclose(file);

    printf("tree file created\n");
}

void inputTree()
{
    // 木を入力

    FILE *file;

    file = fopen("tree3.json", "r");

    Node *tree;

    char *str = (char *)calloc(NUMBER_OF_SEARCH * NUMBER_OF_SQUARES * NUMBER_OF_SQUARES * NUMBER_OF_CHARACTERS_FOR_A_NODE, sizeof(char));

    fscanf(file, "%s", str);

    toNodeString(tree, str);

    // printf("%s", str);

    fclose(file);
}

// ノードを現在のノードの下に作成する、作成済みの場合は、通過数を加える
// 次のノードのインデックスを返す
int deployNode(Node *child, Node *parent)
{
    int index = isCreated(child, parent);
    if (index == -1)
    {
        // childNodeにparentにparentNodeを設定
        (*child).parent = parent;

        // 次のノード数をインクリメント
        (*parent).childCount++;

        // 現在の次のノードを一時的に移す
        Node **old = (*parent).child;

        // 新しい領域を確保（２階層のうちの１階層目）
        (*parent).child = (Node **)calloc((*parent).childCount, sizeof(Node*));

        // 確保した領域に、既存のアドレスをセット
        for (int o = 0; o < (*parent).childCount - 1; o++)
        {
            (*parent).child[o] = old[o];
        }

        // 確保した領域の最後に、新規のノードをセット
        (*parent).child[(*parent).childCount - 1] = child;

        return (*parent).childCount - 1;
    }
    else
    {
        return index;
    }
}