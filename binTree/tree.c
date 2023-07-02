#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

typedef struct pair {
    TreeNode* parent;   //指向父节点
    TreeNode* theOne;   //指向包含项的节点
} Pair;

/* 创建一个新节点 
IN: 
    pItem: 指向新项的指针
RETURN: 
    返回新节点(TreeNode)的指针
*/
static TreeNode* MakeNode(const Item* pItem);

/* 
自定义的比较函数:
按字母排序
如果新项应放在左子树中，ToLeft() 函数返回true
如果新项应放在右子树中，ToRight()函数返回true

因为是中序遍历
所以ToLeft是  '<'
    ToRight是 '>'
 */
static bool ToLeft(const Item* item1, const Item* item2);
static bool ToRight(const Item* item1, const Item* item2);

//确定新节点的位置并添加新节点
static void AddNode(TreeNode* newNode, TreeNode* root);

//中序遍历二叉树: 左、中、右
static void Inorder(const TreeNode* root, void (*pfunc)(Item item));

/* 
查找项

RETURN:
typedef struct pair {
    TreeNode* parent;
    TreeNode* theOne;
} Pair;
返回的结构包含两个指针：
    theOne:  指向包含项的节点（如果未找到指定项则为NULL）；
    parent: 指向父节点（如果该节点为根节点，即没有父节点，则为NULL）。
 */
static Pair SeekItem(const Item* pItem, const Tree* pTree);

//删除节点
static void DeleteNode(TreeNode** pNode);

//删除所有节点
static void DeleteAllNodes(TreeNode* root);


void InitializeTree(Tree* pTree)
{
    pTree->root = NULL;
    pTree->tSize = 0;
}

bool TreeIsEmpty(const Tree* pTree)
{
    return pTree->root == NULL;
}

bool TreeIsFull(const Tree* pTree)
{
    return pTree->tSize == MAXITEMS;
}

int TreeItemCount(const Tree* pTree)
{
    return pTree->tSize;
}

bool AddItem(const Item* pItem, Tree* pTree)
{
    TreeNode* new_node;

//(1)检查树是否满了?
    if (TreeIsFull(pTree)) {
        fprintf(stderr, "Tree is full\n");
        return false;
    }

//(2)检查重复项
    if (SeekItem(pItem, pTree).theOne != NULL) {
        fprintf(stderr, "Duplicate item!\n");
        return false;
    }

//(3)创建新节点
    new_node = MakeNode(pItem);
    if (!new_node) {
        fprintf(stderr, "Creat New node failed!\n");
        return false;
    }
//(4)至此创建一个新节点成功,更新size
    pTree->tSize ++;

//(5)找到适合新节点的位置
    if (!pTree->root) {
        pTree->root = new_node;         //case: 根节点为空，此时初始化根节点指向新节点
    } else {
        AddNode(new_node, pTree->root); //case: 根节点不为空，在tree中添加新节点
    }


    return true;
}

bool InTree(const Item* pItem, const Tree* pTree)
{
    return SeekItem(pItem, pTree).theOne == NULL ? false : true;
}

/* 
三种情况:
(1)叶子节点(该节点没有左右子树)
(2)有一个子树的节点
(3)有俩个子树的节点
 */
bool DeleteItem(const Item* pItem, Tree* pTree)
{
    Pair look;

    look = SeekItem(pItem, pTree);
    if (look.theOne == NULL) {          //没有找到指定项
        return false;
    }

    if (look.parent == NULL) {          //要删除的item对应的是root
        DeleteNode(&pTree->root);
    } else if (look.parent->left == look.theOne) {      //要删除的是父节点的左节点
        DeleteNode(&look.parent->left);
    } else {                            //要删除的是父节点的右节点
        DeleteNode(&look.parent->right);
    }

    //更新item数量
    pTree->tSize --;

    return true;
}

void Traverse(const Tree* pTree, void (*pfunc)(Item item))
{
    if (pTree != NULL) {
        Inorder(pTree->root, pfunc);
    }
}

void DeinitTree(Tree* pTree)
{
    if (pTree != NULL) {
        DeleteAllNodes(pTree->root);
    }

    //reinit
    InitializeTree(pTree);
}

/* local function */

Pair SeekItem(const Item* pItem, const Tree* pTree)
{
    Pair look;

    look.parent = NULL;                 //初始化为NULL(root没有父节点)
    look.theOne = pTree->root;          //初始化指向根节点

    if (!look.theOne) {
        return look;                    //root节点为NULL，树还没有初始化，直接return
    }

    while (look.theOne) {
        if (ToLeft(pItem, &(look.theOne->item))) {
            look.parent = look.theOne;            //更新父节点
            look.theOne = look.theOne->left;      //更新子节点为左子树
        } else if (ToRight(pItem, &(look.theOne->item))) {
            look.parent = look.theOne;
            look.theOne = look.theOne->right;
        } else {
            break;    //既不在左子树也不在右子树，那么当前项就是匹配的
        }
    }

    return look;
}

TreeNode* MakeNode(const Item* pItem)
{
    TreeNode* new_node = NULL;

    new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node) {
        new_node->item = *pItem;

        //新建的Node没有子节点
        new_node->left = NULL;
        new_node->right = NULL;
    }

    return new_node;
}

bool ToLeft(const Item* item1, const Item* item2)
{
    int ret;

    //先比较name，再比较kind
    if ((ret = strcmp(item1->petname, item2->petname)) < 0) {
        return true;
    } else if (ret == 0 && strcmp(item1->petkind, item2->petkind) < 0) {
        return true;
    }

    return false;       //遇到了重复项，即name和kind都相同
}

bool ToRight(const Item* item1, const Item* item2)
{
    int ret;

    if ((ret = strcmp(item1->petname, item2->petname)) > 0) {
        return true;
    } else if (ret == 0 && strcmp(item1->petkind, item2->petkind) > 0) {
        return true;
    }

    return false;       //遇到了重复项，即name和kind都相同
}

void AddNode(TreeNode* newNode, TreeNode* root)
{
    if (ToLeft(&newNode->item, &root->item)) {
        if (!root->left) {                      //递归结束条件: 找到一个空的左子树
            //左子树为空，让左节点指针指向新项
            root->left = newNode;
        } else {
            AddNode(newNode, root->left);       //递归处理
        }
    } else if (ToRight(&newNode->item, &root->item)) {
        if (!root->right) {                     //递归结束条件: 找到一个空的右子树
            //右子树为空，让右节点指针指向新项
            root->right = newNode;
        } else {
            AddNode(newNode, root->right);      //递归处理
        }
    } else {
        fprintf(stderr, "error in AddNode\n");
        exit(1);
    }
}

/* 
pNode: 指向待删除节点父节点指针的地址
 */
void DeleteNode(TreeNode** pNode)
{
    TreeNode* temp;
    TreeNode* cur = *pNode;

    if (cur->left == NULL) {               //没有左节点
        temp = cur;
        cur = cur->right;                   //指向右节点
        free(temp);
    } else if (cur->right == NULL) {       //没有右节点
        temp = cur;
        cur = cur->left;                    //指向左节点
        free(temp);
    } else {                 //左、右节点都有的情况
        for (temp = cur->left; temp->right != NULL; temp = temp->right) {
            continue;
        }

        //链接待删除节点右子树
        temp->right = cur->right;
        temp = cur;

        cur = cur->left;              //左子树链接到被删除节点的父节点上
        free(temp);
    }
}

void Inorder(const TreeNode* root, void (*pfunc)(Item item))
{
    if (root != NULL) {
        Inorder(root->left, pfunc);
        pfunc(root->item);              //回调函数
        Inorder(root->right, pfunc);
    }
}

void DeleteAllNodes(TreeNode* root)
{
    TreeNode* temp;

    if (root != NULL) {
        temp = root->right;

        DeleteAllNodes(root->left);         //删除左节点资源
        free(root);                         //删除父节点资源
        DeleteAllNodes(temp);               //删除右节点资源
    }
}