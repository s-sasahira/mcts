#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <tchar.h>
#include <math.h>
#include "board.c"
#include "parson.c"

#define NUMBER_OF_SEARCH 10
#define NUMBER_OF_CHARACTERS_FOR_A_NODE 235

#define JSON2STRUCT_STR(_json, _struct, _key, _size)                           \
    do                                                                         \
    {                                                                          \
        strncpy(_struct._key, json_object_dotget_string(_json, #_key), _size); \
    } while (0);

#define JSON2STRUCT_NUM(_json, _struct, _key)                   \
    do                                                          \
    {                                                           \
        _struct._key = json_object_dotget_number(_json, #_key); \
    } while (0);

#define JSON2STRUCT_BOOL(_json, _struct, _key)                   \
    do                                                           \
    {                                                            \
        _struct._key = json_object_dotget_boolean(_json, #_key); \
    } while (0);

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
JSON_Object* convertJsonObject(JSON_Object *jsonObject, struct Node node)
{
    json_object_set_number(jsonObject, "turn", node.turn);
    json_object_set_number(jsonObject, "address", node.address);
    json_object_set_number(jsonObject, "rock", node.rock);
    json_object_set_number(jsonObject, "throughCount", node.throughCount);
    json_object_set_number(jsonObject, "drawCount", node.drawCount);
    json_object_set_number(jsonObject, "ciWinCount", node.ciWinCount);
    json_object_set_number(jsonObject, "crWinCount", node.crWinCount);
    json_object_set_number(jsonObject, "childCount", node.childCount);
    JSON_Value *childJsonValue = json_value_init_array();
    JSON_Array *childJsonArray = json_value_get_array(childJsonValue);
    for (int o = 0; o < node.childCount; o++)
    {
        JSON_Value *achildJsonValue = json_value_init_object();
        JSON_Object *achildJsonObject = json_value_get_object(achildJsonValue);
        achildJsonObject = convertJsonObject(achildJsonObject, *node.child[o]);
        json_array_append_value(childJsonArray, achildJsonValue);
    }
    json_object_dotset_value(jsonObject, "child", childJsonValue);
    return jsonObject;
}

void outputTree(Node tree)
{
    // 木を出力

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    root_object = convertJsonObject(root_object, tree);

    json_serialize_to_file(root_value, "output2.json");

    json_value_free(root_value);

    printf("tree file created\n");
}



// jsonオブジェクトを、node構造体に変換
struct Node* convertNode(JSON_Object *json)
{
    struct Node *node = (struct Node *)calloc(1, sizeof(Node));
    JSON2STRUCT_NUM(json, (*node), turn);
    JSON2STRUCT_NUM(json, (*node), address);
    JSON2STRUCT_NUM(json, (*node), rock);
    JSON2STRUCT_NUM(json, (*node), throughCount);
    JSON2STRUCT_NUM(json, (*node), drawCount);
    JSON2STRUCT_NUM(json, (*node), ciWinCount);
    JSON2STRUCT_NUM(json, (*node), crWinCount);
    JSON2STRUCT_NUM(json, (*node), childCount);
    // printf("childc:%d\n", (*node).childCount);
    JSON_Array *childJson = json_object_get_array(json, "child");
    (*node).child = (struct Node **)calloc((*node).childCount, sizeof(Node*));
    for (int o = 0; o < (*node).childCount; o++)
    {
        (*node).child[o] = (struct Node *)calloc(1, sizeof(Node));
    }
    for (int i = 0; i < (*node).childCount; i++)
    {
        // printf("pointer[%d]:%p\n", i, node.child[i]);
        (*node).child[i] = convertNode(json_array_get_object(childJson, i));
        (*(*node).child[i]).parent = node;
        // printf("ppointer[%d]:%p\n", i, (*node.child[i]).parent);
    }
    return node;
}

// jsonファイルの解析
void jsonParse(Node **node)
{
    JSON_Value *root_value = json_parse_file("./tree.json");
    JSON_Object *root = json_object(root_value);

    *node = convertNode(root);

    json_value_free(root_value);

    printf("jsonParse finished\n");
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
        free(child);
        // printf("newNode free\n");
        return index;
    }
}

// UCBアルゴリズム
int ucb(struct Node node, int t, int rock)
{
    int selected_arm = 0;
    double max_ucb = 0.0;

    for (int i = 0; i < node.childCount; i++)
    {
        if ((*node.child[i]).throughCount == 0)
        {
            selected_arm = i;
            break;
        }

        int winCount;
        switch (rock)
        {
        case CIRCLE:
            winCount = (*node.child[i]).ciWinCount;
            break;
        case CROSS:
            winCount = (*node.child[i]).ciWinCount;
            break;
        }

        double avg_reward = ((double)winCount + (double)(*node.child[i]).drawCount * 0.5) / (double)(*node.child[i]).throughCount;

        double ucb_value = avg_reward + sqrt(2 * log(t) / (*node.child[i]).throughCount);

        if (ucb_value > max_ucb)
        {
            max_ucb = ucb_value;
            selected_arm = i;
        }
    }
    return selected_arm;
}

// 現在指すことができる手のノードを作成する
void createNodeFromPossiblePlace(struct Node *node, int rock, int **board)
{
    int ablePlace[NUMBER_OF_SQUARES];
    int ablePlaceCount = getPossiblePlace(ablePlace, rock, board);

    for (int i = 0; i < ablePlaceCount; i++)
    {
        Node *newNode = (Node *)calloc(1, sizeof(Node));
        initNode(newNode);
        (*newNode).turn = (*node).turn + 1;
        (*newNode).address = ablePlace[i];
        (*newNode).rock = rock;

        deployNode(newNode, node);
    }
}
