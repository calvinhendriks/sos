#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sortedcontainer.h"

int data_compare(data* d1, data* d2) {
    assert(d1);
    assert(d2);
    if(d1->age < d2->age) return -1;
    if(d1->age > d2->age) return 1;
    return strcmp(d1->name, d2->name);
}

// Do not change
void data_print(data* d, FILE* f) {
    fprintf(f, "%i %s", d->age, d->name);
}

data* data_new(int age, char const* name) {
    data* d = (data*)malloc(sizeof(data));
    d->age = age;
    strncpy(d->name, name, NAME_LENGTH);
    return d;
}

void data_delete(data* d) {
    free(d);
}

node* node_new(data* d) {
    // Implement this
    node* n = (node*)malloc(sizeof(node));
    n->data = d;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void node_delete(node* n) {
    // Implement this
    free(n);
}

sortedcontainer* sortedcontainer_new() {
    sortedcontainer* d = (sortedcontainer*)malloc(sizeof(sortedcontainer));
    d->root = NULL;
    return d;
}

void sortedcontainer_insert(sortedcontainer* sc, data* data) {
    if(data == NULL){
        return;
    }
    node* n = node_new(data);
    // Implement this
    if(sc->root == NULL) {
        sc->root = n;
    } else {
        node* temp = sc->root;
        while (true){
            if(data_compare(data, temp->data) < 0){
                if(temp->left == NULL){
                    temp->left = n;
                    break;
                } else{
                    temp = temp->left;
                }
            } else if(data_compare(data, temp->data) > 0){
                if(temp->right == NULL){
                    temp->right = n;
                    break;
                } else{
                    temp = temp->right;
                }
            } else {
                data_delete(data);
                node_delete(n);
                break;

            }
        }
    }
}

node* findMinimum(node* node){
    while(node->left != NULL){
        node = node->left;
    }
    return node;
}

node* findParent(sortedcontainer* sc, node* n){
    node* temp = sc->root;
    node* parent = sc->root;
    while(true){
        if(data_compare(n->data, temp->data) < 0){
            parent = temp;
            temp = temp->left;
        } else if(data_compare(n->data, temp->data) > 0){
            parent = temp;
            temp = temp->right;
        } else {
            return parent;
        }
    }
}

// in sortedcontainer.h the function is defined as data* data and we changed this to nodeData to be able to say data* minData.

int sortedcontainer_erase(sortedcontainer* sc, data *nodeData) {
    // Implement this
    if(sortedcontainer_contains(sc, nodeData) != 1){
        return 0;
    }

    node* toDelete = sc->root;
    node* parent = NULL;

    // for case of 2 children
    node* min = NULL;

    while (true) {
        if (data_compare(nodeData, toDelete->data) < 0) {
            if (toDelete->left == NULL) {
                return 0;
            } else {
                parent = toDelete;
                toDelete = toDelete->left;
            }
        } else if (data_compare(nodeData, toDelete->data) > 0) {
            if (toDelete->right == NULL) {
                return 0;
            } else {
                parent = toDelete;
                toDelete = toDelete->right;
            }
        } else {
            // set amount of children
            int children = 0;
            if ((toDelete->left && !toDelete->right) || (!toDelete->left && toDelete->right)) {
                children = 1;
            } else if (toDelete->left && toDelete->right) {
                children = 2;
            }
            switch (children) {
                //Case 0: No to be deleted has 0 children (leaf node)
                case 0:
                    if(toDelete == sc->root){
                        sc->root = NULL;
                        data_delete(toDelete->data);
                        node_delete(toDelete);
                        return 1;
                    }
                    // if the node to delete is on the left side of the parent
                    if (data_compare(toDelete->data, parent->data) < 0) {
                        parent->left = NULL;
                        data_delete(toDelete->data);
                        node_delete(toDelete);
                        return 1;
                    } else {
                        // if the node to delete is ont he right side of the parent
                        parent->right = NULL;
                        data_delete(toDelete->data);
                        node_delete(toDelete);
                        return 1;
                    }
                //Case 1: node to be deleted has 1 child 
                case 1:
                    
                    if(toDelete == sc->root){
                        if (toDelete->left == NULL) {
                            sc->root = toDelete->right;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        } else {
                            sc->root = toDelete->left;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        }
                    }
                    // if the node to delete is on the left side of the parent
                    if (data_compare(toDelete->data, parent->data) < 0) {
                        if (toDelete->left == NULL) {
                            // set the parent right node to the right child of the node to delete
                            parent->left = toDelete->right;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        } else {
                            // set the parent right node to the left child of the node to delete
                            parent->left = toDelete->left;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        }
                    } else {
                        // if the node to delete is ont he right side of the parent
                        if (toDelete->left == NULL) {
                            // set the parent right node to the right child of the node to delete
                            parent->right = toDelete->right;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        } else {
                            // set the parent right node to the left child of the node to delete
                            parent->right = toDelete->left;
                            data_delete(toDelete->data);
                            node_delete(toDelete);
                            return 1;
                        }
                    }
                //Case 2: Node to be deleted has 2 children.
                case 2:
                    min = findMinimum(toDelete->right);
                    if(toDelete->right == min){
                        //If the minimum value of the right subtree is directly connected to the node to be deleted.
                        data_delete(toDelete->data);
                        toDelete->data = min->data;
                        toDelete->right = min->right;
                        node_delete(min);
                        return 1;
                    } else if(min->right){
                        //If the minimum node of the rigth subtree has any children (can only be right children, because it is already minimum)
                        node* x = findParent(sc, min);
                        x->left = min->right;
                        data_delete(toDelete->data);
                        toDelete->data = min->data;
                        node_delete(min);
                        return 1;
                    } else {
                        //If the minimum node has no children.
                        node* x = findParent(sc, min);
                        x->left = NULL;
                        data_delete(toDelete->data);
                        toDelete->data = min->data;
                        node_delete(min);
                        return 1;
                    }
            }
        }
    }
}

int sortedcontainer_contains(sortedcontainer* sc, data* data) {
    // Implement this
    if(data == NULL){
        return -1;
    }
    if(sc->root == NULL) {
        return 0;
    } else {
        node* temp = sc->root;
        while (true){
            if(data_compare(data, temp->data) < 0){
                if(temp->left == NULL){
                    return 0;
                } else{
                    temp = temp->left;
                }
            } else if(data_compare(data, temp->data) > 0){
                if(temp->right == NULL){
                    return 0;
                } else{
                    temp = temp->right;
                }
            } else {
                return 1;
            }
        }
    }
}

// Do not change
static void node_printtree(node* n, int level, FILE* printFile) {
    fprintf(printFile, "%*s ", level, "");
    if(n) {
        data_print(n->data, printFile);
        fprintf(printFile, "\n");
        node_printtree(n->left, level+1, printFile);
        node_printtree(n->right, level+1, printFile);
    } else {
        fprintf(printFile, "(nil)\n");
    }
}

// Do not change
void sortedcontainer_print(sortedcontainer* sc, FILE* printFile) {
    node_printtree(sc->root, 0, printFile);
}

static void node_deletetree(node* n) {
    if(n) {
        node* left = n->left;
        node* right = n->right;
        data_delete(n->data);
        node_delete(n);
        node_deletetree(left);
        node_deletetree(right);
    }
}

void sortedcontainer_delete(sortedcontainer* sc) {
    node_deletetree(sc->root);
    free(sc);
}

static int node_issortedtree(node* n, data* min, data* max) {
    if(n == NULL) return 1;

    if(min && data_compare(n->data, min) < 0) return 0;
    if(max && data_compare(max, n->data) < 0) return 0;

    return node_issortedtree(n->left, min, n->data) && node_issortedtree(n->right, n->data, max);
}

int sortedcontainer_issorted(sortedcontainer* sc) {
    return node_issortedtree(sc->root, NULL, NULL);
}
