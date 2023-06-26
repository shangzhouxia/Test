#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>

#define     MAXITEMS    10
#define     SLEN        20

typedef struct item {
    char petname[SLEN];
    char petkind[SLEN];
} Item;

typedef struct treenode {
    Item item;
    struct treenode* left;          //指向左子树
    struct treenode* right;         //指向右子树
} TreeNode;

typedef struct tree {
    TreeNode* root;                 //指向根节点指针
    int tSize;                      //树的项数
} Tree;


/* 
初始化树为空:
    根节点root为NULL
    size = 0
 */
void InitializeTree(Tree* pTree);


/* 
确定树是否为空:
    root是否为空
 */
bool TreeIsEmpty(const Tree* pTree);

/* 
树是否已满？
    预定义的size是否已经达到？
 */
bool TreeIsFull(const Tree* pTree);

/* 
返回树中的Node个数
 */
int TreeItemCount(const Tree* pTree);


/* 
添加Item
 */
bool AddItem(const Item* pItem, Tree* pTree);

/* 
在树中查找Item
 */
bool InTree(const Item* pItem, const Tree* pTree);

/* 
在树中删除Item
该函数要知道待删除的父节点
以便在删除子节点后更新父节点指向子节点的指针
 */
bool DeleteItem(const Item* pItem, Tree* pTree);

void Traverse(const Tree* pTree, void (*pfunc)(Item item));

void DeinitTree(Tree* pTree);

#endif