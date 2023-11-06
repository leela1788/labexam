#include <stdio.h>
#include <stdlib.h>

#define M 3 // Change M to set the order of the B-tree

// Structure for a B-tree node
struct BTreeNode {
    int keys[M - 1];
    struct BTreeNode* children[M];
    int numKeys;
    int isLeaf;
};

// Function to create a new B-tree node
struct BTreeNode* createNode() {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->numKeys = 0;
    newNode->isLeaf = 1;
    for (int i = 0; i < M; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to insert a key into the B-tree
struct BTreeNode* insert(struct BTreeNode* root, int key) {
    // If the root is NULL, create a new root
    if (root == NULL) {
        struct BTreeNode* newNode = createNode();
        newNode->keys[0] = key;
        newNode->numKeys = 1;
        return newNode;
    }

    // Find the child to insert the key
    int i = root->numKeys - 1;
    if (root->isLeaf) {
        // If the current node is a leaf, insert the key into it
        while (i >= 0 && key < root->keys[i]) {
            root->keys[i + 1] = root->keys[i];
            i--;
        }
        root->keys[i + 1] = key;
        root->numKeys++;
    } else {
        // If the current node is not a leaf, find the child and insert the key into it
        while (i >= 0 && key < root->keys[i]) {
            i--;
        }
        int childIndex = i + 1;
        root->children[childIndex] = insert(root->children[childIndex], key);

        // Check if the child split and fix the parent node
        if (root->children[childIndex]->numKeys == M - 1) {
            struct BTreeNode* newChild = createNode();
            struct BTreeNode* child = root->children[childIndex];
            for (int j = 0; j < M - 1; j++) {
                newChild->keys[j] = child->keys[j + M / 2];
                newChild->numKeys++;
                child->keys[j + M / 2] = 0;
                child->numKeys--;
            }
            newChild->isLeaf = child->isLeaf;

            // Move child pointers to the right
            for (int j = M - 1; j >= childIndex + 1; j--) {
                root->children[j + 1] = root->children[j];
            }
            root->children[childIndex + 1] = newChild;

            // Move child keys to the right
            for (int j = root->numKeys - 1; j >= childIndex; j--) {
                root->keys[j + 1] = root->keys[j];
            }
            root->keys[childIndex] = child->keys[M / 2 - 1];
            root->numKeys++;
        }
    }

    // If the root split, create a new root
    if (root->numKeys == M - 1) {
        struct BTreeNode* newRoot = createNode();
        newRoot->children[0] = root;
        newRoot->isLeaf = 0;
        struct BTreeNode* child = root;
        root = newRoot;

        struct BTreeNode* newChild = createNode();
        for (int j = 0; j < M - 1; j++) {
            newChild->keys[j] = child->keys[j + M / 2];
            newChild->numKeys++;
            child->keys[j + M / 2] = 0;
            child->numKeys--;
        }
        newChild->isLeaf = child->isLeaf;

        // Move child pointers to the right
        for (int j = M - 1; j >= 1; j--) {
            root->children[j] = root->children[j - 1];
        }
        root->children[0] = child;
        root->children[1] = newChild;

        // Move child keys to the right
        for (int j = root->numKeys - 1; j >= 0; j--) {
            root->keys[j + 1] = root->keys[j];
        }
        root->keys[0] = child->keys[M / 2 - 1];
        root->numKeys++;
    }

    return root;
}

// Function to search for a key in the B-tree
int search(struct BTreeNode* root, int key) {
    int i = 0;
    while (i < root->numKeys && key > root->keys[i]) {
        i++;
    }
    if (i < root->numKeys && key == root->keys[i]) {
        return 1;
    }
    if (root->isLeaf) {
        return 0;
    }
    return search(root->children[i], key);
}

// Function to display the B-tree in-order
void display(struct BTreeNode* root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->numKeys; i++) {
            display(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        display(root->children[i]);
    }
}

int main() {
    struct BTreeNode* root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 12);
    root = insert(root, 30);
    root = insert(root, 7);
    root = insert(root, 17);

    printf("In-order traversal of the B-tree: ");
    display(root);
    printf("\n");

    int searchKey = 12;
    if (search(root, searchKey))
        printf("%d is found in the B-tree.\n", searchKey);
    else
        printf("%d is not found in the B-tree.\n", searchKey);

    return 0;
}