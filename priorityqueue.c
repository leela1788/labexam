#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure to represent a Priority Queue
struct PriorityQueue {
    int* heap;
    int capacity;
    int size;
};

// Function to create a new Priority Queue
struct PriorityQueue* createPriorityQueue(int capacity) {
    struct PriorityQueue* pq = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
    if (!pq) {
        perror("Memory allocation failed");
        exit(1);
    }
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (int*)malloc(sizeof(int) * capacity);
    if (!pq->heap) {
        perror("Memory allocation failed");
        exit(1);
    }
    return pq;
}

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the min-heap property after insertion
void heapifyUp(struct PriorityQueue* pq, int index) {
    int* heap = pq->heap;
    while (index > 0 && heap[index] < heap[(index - 1) / 2]) {
        swap(&heap[index], &heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// Function to insert an element with a given priority into the Priority Queue
void insert(struct PriorityQueue* pq, int key) {
    if (pq->size == pq->capacity) {
        printf("Priority Queue is full, cannot insert.\n");
        return;
    }

    pq->heap[pq->size] = key;
    heapifyUp(pq, pq->size);
    pq->size++;
}

// Function to maintain the min-heap property after deletion
void heapifyDown(struct PriorityQueue* pq, int index) {
    int* heap = pq->heap;
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && heap[left] < heap[smallest])
        smallest = left;

    if (right < pq->size && heap[right] < heap[smallest])
        smallest = right;

    if (smallest != index) {
        swap(&heap[index], &heap[smallest]);
        heapifyDown(pq, smallest);
    }
}

// Function to remove and return the element with the highest priority (minimum value)
int extractMin(struct PriorityQueue* pq) {
    if (pq->size == 0) {
        printf("Priority Queue is empty.\n");
        return INT_MIN;
    }

    int min = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);

    return min;
}

// Function to check if the Priority Queue is empty
int isEmpty(struct PriorityQueue* pq) {
    return pq->size == 0;
}

int main() {
    int capacity;
    printf("Enter the capacity of the Priority Queue: ");
    scanf("%d", &capacity);

    struct PriorityQueue* pq = createPriorityQueue(capacity);

    int choice;
    while (1) {
        printf("\n1. Insert element\n2. Extract minimum element\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int key;
                printf("Enter the element to insert: ");
                scanf("%d", &key);
                insert(pq, key);
                break;
            }
            case 2: {
                int min = extractMin(pq);
                if (min != INT_MIN) {
                    printf("Extracted minimum element: %d\n", min);
                }
                break;
            }
            case 3:
                free(pq->heap);
                free(pq);
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
