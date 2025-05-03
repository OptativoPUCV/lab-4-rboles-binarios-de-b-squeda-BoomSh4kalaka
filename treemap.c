#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;

    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *aux = tree->root;
    TreeNode *parent = NULL;

    while (aux != NULL) {
        parent = aux;
        if (tree->lower_than(key, aux->pair->key) == 1) {
            aux = aux->left;
        } else if (tree->lower_than(key, aux->pair->key) == 0) {
            if (is_equal(tree, key, aux->pair->key) == 1) {
                return;
            } else {
                aux = aux->right;
            }
        }
    }

    TreeNode * new = createTreeNode(key, value);
    if (new == NULL) return;

    new->parent = parent;
    if (parent == NULL) {
        tree->root = new;
    } else if (tree->lower_than(key, parent->pair->key) == 1) {
        parent->left = new;
    } else {
        parent->right = new;
    }

    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left != NULL && node->right != NULL) {
        TreeNode *minRight = minimum(node->right);
        node->pair->key = minRight->pair->key;
        node->pair->value = minRight->pair->value;
        removeNode(tree, minRight);
    }
    else {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child;
            if (child != NULL) {
                child->parent = NULL;
            }
        }
        else {
            if (node == node->parent->left) {
                node->parent->left = child;
            } 
            else {
                node->parent->right = child;
            }
            if (child != NULL) {
                child->parent = node->parent;
            }
        }
    }
    free(node->pair);
    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
        }
        else if (tree->lower_than(key, aux->pair->key) == 0)
        {
            if (is_equal(tree, key, aux->pair->key) == 1)
            {
                tree->current = aux;
                return aux->pair;
            }
            else
            {
                aux = aux->right;
            }
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    TreeNode *ub_node = NULL;
    while (aux != NULL) {
        if (tree->lower_than(key, aux->pair->key) == 1) {
            ub_node = aux;
            aux = aux->left;
        } else {
            if (is_equal(tree, key, aux->pair->key) == 1) {
                return aux->pair;
            }
            aux = aux->right;
        }
    }
    return ub_node ? ub_node->pair : NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while (aux->left != NULL) {
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if (aux->right != NULL) {
        aux = aux->right;
        while (aux->left != NULL) {
            aux = aux->left;
        }
        tree->current = aux;
        return aux->pair;
    }
    else {
        while (aux->parent != NULL && aux == aux->parent->right) {
            aux = aux->parent;
        }
        tree->current = aux->parent;
        if (tree->current != NULL) {
            return tree->current->pair;
        }
    }
    return NULL;
}
