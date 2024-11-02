#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value, priority;
    struct Node *left, *right;
} Node;

Node* new_node(int value) {
    Node* new = (Node*)malloc(sizeof(Node));
    
    if (new != NULL) {
        new->value = value;
        new->priority = rand() % 100;
        new->left = NULL;
        new->right = NULL;
    }
    
    return new;
}

Node* left_rotation(Node* node) {
    Node* aux = node->right;
    node->right = aux->left;
    aux->left = node;
    
    return aux;
}

Node* right_rotation(Node* node) {
    Node* aux = node->left;
    node->left = aux->right;
    aux->right = node;
    
    return aux;
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return new_node(value);
    }
    
    if (value < root->value) {
        root->left = insert(root->left, value);
        
        if (root->left != NULL && root->left->priority > root->priority) {
            root = right_rotation(root);
        }
    } 
    else if (value > root->value) {
        root->right = insert(root->right, value);
        
        if (root->right != NULL && root->right->priority > root->priority) {
            root = left_rotation(root);
        }
    }
    
    return root;
}

Node* remove_node(Node* root, int value) {
    if (root == NULL) return root;
    
    if (value < root->value) {
        root->left = remove_node(root->left, value);
    } 
    else if (value > root->value) {
        root->right = remove_node(root->right, value);
    } 
    else {
        if (root->left == NULL) {
            Node* aux = root->right;
            free(root);
            root = aux;
        } else if (root->right == NULL) {
            Node* aux = root->left;
            free(root);
            root = aux;
        } else if (root->left->priority < root->right->priority) {
            root = left_rotation(root);
            root->left = remove_node(root->left, value);
        } else {
            root = right_rotation(root);
            root->right = remove_node(root->right, value);
        }
    }
    
    return root;
}
