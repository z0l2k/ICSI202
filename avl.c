#include "DS.h"

Elm* newNode(int x) {
    Elm* p = (Elm*)malloc(sizeof(Elm));
    p->x = x;
    p->height = p->len = 1;
    p->L = p->R = NULL;
    return p;
}
Elm* rotate(Elm* p, int direction) {
    if (!p) return NULL;
    Elm* q = direction == 1 ? p->L : p->R;
    Elm* T2 = direction == 1 ? q->R : q->L;
    
    if (direction == 1) {
        q->R = p; 
        p->L = T2;
    } else {
        q->L = p; 
        p->R = T2;
    }

    p->height = max(height(p->L), height(p->R)) + 1;
    q->height = max(height(q->L), height(q->R)) + 1;
    p->len = size(p); 
    q->len = size(q);
    
    return q;
}

Elm* insert(Elm* p, int x) {
    if (!p) 
        return newNode(x);
    if (x < p->x) 
        p->L = insert(p->L, x);
    else if (x > p->x) 
        p->R = insert(p->R, x);
    else 
        return p;
    p->height = 1 + max(height(p->L), height(p->R));
    p->len++; 
    int balance = balancefactor(p);
    if (balance > 1 && x < p->L->x) 
        return rotate(p, 1); // Right rotation
    if (balance > 1 && x > p->L->x) {
        p->L = rotate(p->L, 0); // Left rotation
        return rotate(p, 1); // Right rotation
    }
    if (balance < -1 && x > p->R->x) 
        return rotate(p, 0); // Left rotation
    if (balance < -1 && x < p->R->x) {
        p->R = rotate(p->R, 1); // Right rotation
        return rotate(p, 0); // Left rotation
    }
    return p;
}

void avl_put(AVL* ptree, int x) {
    if (!ptree->root) 
        ptree->root = newNode(x);
    else 
        ptree->root = insert(ptree->root, x);
}

Elm* get(Elm* p, int x) {
    if (!p || x == p->x) 
        return p;
    if (x > p->x) 
        return get(p->R, x);
    else 
        return get(p->L, x);
}

Elm* avl_get(const AVL* ptree, int x) { 
    return ptree->root ? get(ptree->root, x) : NULL; 
}

Elm* del(Elm* p, int x) {
    if (!p) 
        return p;
    if (x < p->x) 
        p->L = del(p->L, x);
    else if (x > p->x) 
        p->R = del(p->R, x);
    else {
        Elm* temp;
        if (!p->L) { 
            temp = p->R; 
            free(p); 
            return temp; 
        }
        else if (!p->R) { 
            temp = p->L; 
            free(p); 
            return temp; 
        }
        temp = p->R;
        while (temp->L) 
            temp = temp->L;
        p->x = temp->x;
        p->R = del(p->R, temp->x);
    }
    p->height = 1 + max(height(p->L), height(p->R));
    p->len--; 
    if (!p) 
        return p;
    int balance = balancefactor(p);
    if (balance > 1 && balancefactor(p->L) >= 0) 
        return rotate(p, 1); // Right rotation
    if (balance > 1 && balancefactor(p->L) < 0) {
        p->L = rotate(p->L, 0); // Left rotation
        return rotate(p, 1); // Right rotation
    }
    if (balance < -1 && balancefactor(p->R) <= 0) 
        return rotate(p, 0); // Left rotation
    if (balance < -1 && balancefactor(p->R) > 0) {
        p->R = rotate(p->R, 1); // Right rotation
        return rotate(p, 0); // Left rotation
    }
    return p;
}
int size(Elm* p) {
    return p ? (size(p->L) + 1 + size(p->R)) : 0;
}

int height(Elm* p) { 
    return p ? p->height : 0; 
}

int max(int a, int b) { 
    return (a > b) ? a : b; 
}

int balancefactor(Elm* p) { 
    return p ? height(p->L) - height(p->R) : 0; 
}

void avl_del(AVL* ptree, int x) { 
    ptree->root = del(ptree->root, x); 
}

int min(Elm* p) {
    Elm* temp = p;
    while (temp->L) 
        temp = temp->L;
    return temp->x;
}

Elm* delMin(Elm* p) {
    if (!p->L) 
        return p->R;
    p->L = delMin(p->L);
    p->len = size(p);
    return p;
}

int avl_delMin(AVL* ptree) {
    int temp = min(ptree->root);
    ptree->root = delMin(ptree->root);
    return temp;
}

void inorder(Elm* p) {
    if (!p) 
        return;
    inorder(p->L);
    printf("%d %d %d\n", p->x, p->len, p->height);
    inorder(p->R);
}

void avl_inorder(const AVL* ptree) { 
    inorder(ptree->root); 
}

int avl_size(const AVL* ptree, int x) {
    Elm* p = avl_get(ptree, x);
    return p ? p->len : -1;
}

int avl_height(const AVL* ptree, int x) {
    Elm* p = avl_get(ptree, x);
    return p ? p->height : -1;
}
