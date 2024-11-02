#include <stdio.h>
#include <stdlib.h>

#define MAX_DEGREE 3

typedef struct node {
    int values[2 * MAX_DEGREE - 1];
    struct node *children[2 * MAX_DEGREE];
    int children_count;
    int is_leaf;
} node;

node* create_node(int is_leaf) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->is_leaf = is_leaf;
    new_node->children_count = 0;
    return new_node;
}

void split_child(node *parent_node, int child_index, node *full_child) {
    node *new_child = create_node(full_child->is_leaf);
    new_child->children_count = MAX_DEGREE - 1;

    for (int j = 0; j < MAX_DEGREE - 1; j++) {
        new_child->values[j] = full_child->values[j + MAX_DEGREE];
    }

    if (!full_child->is_leaf) {
        for (int j = 0; j < MAX_DEGREE; j++) {
            new_child->children[j] = full_child->children[j + MAX_DEGREE];
        }
    }

    full_child->children_count = MAX_DEGREE - 1;

    for (int j = parent_node->children_count; j >= child_index + 1; j--) {
        parent_node->children[j + 1] = parent_node->children[j];
    }

    parent_node->children[child_index + 1] = new_child;

    for (int j = parent_node->children_count - 1; j >= child_index; j--) {
        parent_node->values[j + 1] = parent_node->values[j];
    }

    parent_node->values[child_index] = full_child->values[MAX_DEGREE - 1];
    parent_node->children_count++;
}

void insert_non_full(node* current_node, int value) {
    int index = current_node->children_count - 1;

    if (current_node->is_leaf) {
        while (index >= 0 && value < current_node->values[index]) {
            current_node->values[index + 1] = current_node->values[index];
            index--;
        }

        current_node->values[index + 1] = value;
        current_node->children_count++;
    } 
    else {
        while (index >= 0 && value < current_node->values[index]) {
            index--;
        }
        index++;

        if (current_node->children[index]->children_count == 2 * MAX_DEGREE - 1) {
            split_child(current_node, index, current_node->children[index]);

            if (value > current_node->values[index]) {
                index++;
            }
        }
        insert_non_full(current_node->children[index], value);
    }
}

void insert(node** root, int value) {
    node* root_node = *root;

    if (root_node->children_count == 2 * MAX_DEGREE - 1) {
        node* new_root = create_node(0);
        *root = new_root;
        new_root->children[0] = root_node;
        split_child(new_root, 0, root_node);
        insert_non_full(new_root, value);
    } 
    else {
        insert_non_full(root_node, value);
    }
}
