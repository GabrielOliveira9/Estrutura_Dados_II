#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

struct Node {
  int value;
  int color;
  struct Node *left, *right, *parent;
};

typedef struct Node Node;

Node *createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->color = RED;
    newNode->left = newNode->right = newNode->parent = NULL;
};

void leftRotation(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotateRight(Node **root, Node *x) {
    Node *y = x->left;
    x->left = y->right;
    
    if (x->left != NULL)
        x->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    
    y->right = x;
    x->parent = y;
    
}

void fixViolationOnInsert(Node **root, Node *pt) {
    Node *parent_pt = NULL;
    Node *grandparent_pt = NULL;

    while (pt != *root && pt->color != BLACK && pt->parent->color == RED) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            Node *uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                if (pt == parent_pt->right) {
                    *root = rotateLeft(*root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                *root = rotateRight(*root, grand_parent_pt);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
        else {
            Node *uncle_pt = grand_parent_pt->left;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                if (pt == parent_pt->left) {
                    *root = rotateRight(*root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                *root = rotateLeft(*root, grand_parent_pt);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
    }

    (*root)->color = BLACK;
}

void insert(Node **root, int data) {
    Node *pt = createNode(data);

    Node *parent = NULL;
    Node *current = *root;

    while (current != NULL) {
        parent = current;
        if (pt->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    pt->parent = parent;

    if (parent == NULL)
        *root = pt;
    else if (pt->data < parent->data)
        parent->left = pt;
    else
        parent->right = pt;

    fixViolationOnInsert(root, pt);
}

void fixViolationOnDelete(Node **root, Node *x) {
    while (x != *root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;  // O irmão de x

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                *root = rotateLeft(*root, x->parent);
                w = x->parent->right;
            }

            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL)
                        w->left->color = BLACK;
                    w->color = RED;
                    *root = rotateRight(*root, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL)
                    w->right->color = BLACK;
                *root = rotateLeft(*root, x->parent);
                x = *root;
            }
        }
        else {
            Node *w = x->parent->left;  

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                *root = rotateRight(*root, x->parent);
                w = x->parent->left;
            }

            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL)
                        w->right->color = BLACK;
                    w->color = RED;
                    *root = rotateLeft(*root, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL)
                    w->left->color = BLACK;
                *root = rotateRight(*root, x->parent);
                x = *root;
            }
        }
    }
    
    if (x != NULL)
        x->color = BLACK;
}

Node *minNode(Node *node) {
    Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

Node *delete(Node *root, int data) {
    Node *z = search(root, data);
    if (z == NULL)
        return root;

    Node *y = z;
    Node *x = NULL;
    Color originalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        if (x != NULL) x->parent = z->parent;

        if (z->parent == NULL)
            root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
    } else if (z->right == NULL) {
        x = z->left;
        if (x != NULL) x->parent = z->parent;

        if (z->parent == NULL)
            root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
    } else {
        y = minValueNode(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent != z) {
            if (x != NULL) x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            y->right->parent = y;
        } else {
            x->parent = y;
        }

        if (z->parent == NULL)
            root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;
        
        y->parent = z->parent;
        y->color = z->color;
        y->left = z->left;
        y->left->parent = y;
    }

    if (originalColor == BLACK)
        fixDelete(&root, x);

    free(z);
    return root;
}

int main()
{
    printf("Hello World");

    return 0;
}
