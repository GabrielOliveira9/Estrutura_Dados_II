#include <stdio.h>
#include <stdlib.h>

// GABRIEL DE OLIVEIRA MAZZEGA SANTOS - CC6M
struct node {
  int value;
  struct node *left_son;
  struct node *right_son;
};

struct node *create_node(int value) {
  struct node *new_node = malloc(sizeof(struct node));
  if (new_node == NULL) {
    return NULL;
  };
  new_node->value = value;
  new_node->left_son = NULL;
  new_node->right_son = NULL;

  return new_node;
};

struct node *insert_in_tree(struct node *root, int value) {
    if (root == NULL) {
      return create_node(value);
    } 
    if (value < root->value) {
      root->left_son = insert_in_tree(root->left_son, value);
    } else if (value > root->value) {
      root->right_son = insert_in_tree(root->right_son, value);
    }
    return root;
}

struct node *search_binary_tree(struct node *root, int value) {
  if (root == NULL) {
    return NULL;
  }
  else if (root->value == value) {
    return root;
  }
  else if (value > root->value) {
    return search_binary_tree(root->right_son, value);
  }
  return search_binary_tree(root->left_son, value);
}

struct node *get_minimum(struct node *root) {
  if (root->left_son == NULL) {
    return root;
  } 
  return get_minimum(root->left_son);
}

int delete_from_tree(struct node *root, int value) {
  if (root == NULL) {
    return 0;
  } else if (root->value == value) {
    free(root);
    delete_from_tree(root->left_son, root->left_son->value);
    delete_from_tree(root->right_son, root->right_son->value);
    return 1;
  } else if (root->value > value) {
    delete_from_tree(root->left_son, value);
  } else {
    delete_from_tree(root->right_son, value);
  }
  return 0;
}

int main(void) {
  printf("Hello World\n");
  return 0;
}
