#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

/**
 * Huffman tree node
 */
struct min_heap_node {
    char data;
    unsigned freq;
    struct min_heap_node *left, *right;
};

/**
 * Collection of Huffman nodes
 */
struct min_heap {
    // current size of the heap
    int size;

    unsigned capacity;

    // array of min heap node pointers
    struct min_heap_node **array;
};

/**
 * Util function to allocate a new min heap(huffman) node
 */
struct min_heap_node *new_node(char data, unsigned freq) {
    struct min_heap_node *node =
        (struct min_heap_node *)malloc(sizeof(struct min_heap_node));
    node->data = data;
    node->left = node->right = NULL;
    node->freq = freq;

    return node;
}

/**
 * Util function to create min heap with capacity
 */
struct min_heap *create_min_heap(unsigned cap) {
    struct min_heap *heap = (struct min_heap *)malloc(sizeof(struct min_heap));

    heap->size = 0;
    heap->capacity = cap;
    heap->array = (struct min_heap_node **)malloc(
        heap->capacity * sizeof(struct min_heap_node *));

    return heap;
}

void swap_min_heap_nodes(struct min_heap_node **left,
                         struct min_heap_node **right) {
    struct min_heap_node *tmp = *left;
    *left = *right;
    *right = tmp;
}

void min_heapify(struct min_heap *heap, int idx) {
    int smallest = idx;
    int left_idx = idx * 2 + 1;
    int right_idx = idx * 2 + 2;

    if (left_idx < heap->size &&
        heap->array[left_idx]->freq < heap->array[smallest]->freq) {
        smallest = left_idx;
    }
    if (right_idx < heap->size &&
        heap->array[right_idx]->freq < heap->array[smallest]->freq) {
        smallest = right_idx;
    }

    if (smallest != idx) {
        // actual swap
        swap_min_heap_nodes(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

int is_size_one(struct min_heap *heap) { return (heap->size == 1); }

struct min_heap_node *extract_min(struct min_heap *heap) {
    struct min_heap_node *tmp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    --heap->size;
    min_heapify(heap, 0);

    return tmp;
}

void insert_min_heap(struct min_heap *heap, struct min_heap_node *node) {
    ++heap->size;
    int idx = heap->size - 1;

    while (idx && node->freq < heap->array[(idx - 1) / 2]->freq) {
        heap->array[idx] = heap->array[(idx - 1) / 2];
        idx = (idx - 1) / 2;
    }

    heap->array[idx] = node;
}

void build_min_heap(struct min_heap *heap) {
    int n = heap->size - 1;
    int idx;
    for (idx = (n - 1) / 2; idx >= 0; --idx) {
        min_heapify(heap, idx);
    }
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

int is_leaf_node(struct min_heap_node *node) {
    return !(node->left) && !(node->right);
}

struct min_heap *create_and_build_min_heap(char data[], int freq[], int size) {
    struct min_heap *heap = create_min_heap(size);

    for (int i = 0; i < size; ++i) {
        heap->array[i] = new_node(data[i], freq[i]);
    }

    heap->size = size;
    build_min_heap(heap);
    return heap;
}

/**
 * Main function that builds the Huffman tree
 */
struct min_heap_node *build_huffman_tree(char data[], int freq[], int size) {
    struct min_heap_node *left, *right, *parent;

    // Step 1: create a min heap of capacity equal to `size`
    struct min_heap *heap = create_and_build_min_heap(data, freq, size);

    while (!is_size_one(heap)) {
        // Step 2: extract 2 minimum freq nodes from heap
        left = extract_min(heap);
        right = extract_min(heap);

        // Step 3: create a new _internal_ node with freq equal to sum of `left`
        // and `right`
        parent = new_node('$', left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        insert_min_heap(heap, parent);
    }

    return extract_min(heap);
}

void print_codes(struct min_heap_node *root, int arr[], int top) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        print_codes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        print_codes(root->right, arr, top + 1);
    }

    if (is_leaf_node(root)) {
        // if this is a leaf node, then it contains one of the input characters,
        // ie. a real node, not an internal one
        printf("%c: ", root->data);
        print_arr(arr, top);
    }
}

int count(char data[], char c) {
    int res = 0;
    int idx = 0;
    while (data[idx]) {
        if (data[idx] == c) ++res;
        ++idx;
    }

    return res;
}

void huffman_codes(char data[], int freq[], int size) {
    // Construct Huffman tree
    struct min_heap_node *root = build_huffman_tree(data, freq, size);

    int arr[MAX_TREE_HT];
    int top = 0;
    print_codes(root, arr, top);
}

int main(int argc, char *argv[]) {
    // argument format: char0:freq0;char1:freq1;char2:freq2:...
    if (argc < 2 || !argv[1]) return 0;
    char *input = argv[1];
    int size = count(input, ':');

    int freq[size];
    char data[size];
    int idx = 0;

    char *token = strtok(input, ";");
    while (token) {
        char d = token[0];
        data[idx] = d;
        int f;
        sscanf(&token[2], "%d", &f);
        freq[idx] = f;
        ++idx;

        token = strtok(NULL, ";");
    }

    if (size != idx) return 0;

    print_arr(freq, size);

    huffman_codes(data, freq, size);

    return 0;
}
