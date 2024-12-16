#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000
#define INF 1000000000

const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

typedef struct {
    int x, y, dir;
    long cost;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->nodes = malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent].cost > heap->nodes[idx].cost) {
            swapNodes(&heap->nodes[parent], &heap->nodes[idx]);
            idx = parent;
        }
        else break;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < heap->size && heap->nodes[left].cost < heap->nodes[smallest].cost)
            smallest = left;
        if (right < heap->size && heap->nodes[right].cost < heap->nodes[smallest].cost)
            smallest = right;
        if (smallest != idx) {
            swapNodes(&heap->nodes[smallest], &heap->nodes[idx]);
            idx = smallest;
        }
        else break;
    }
}

void pushHeap(MinHeap* heap, HeapNode node) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = realloc(heap->nodes, sizeof(HeapNode) * heap->capacity);
    }
    heap->nodes[heap->size] = node;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode popHeap(MinHeap* heap) {
    HeapNode top = heap->nodes[0];
    heap->size--;
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        heapify_down(heap, 0);
    }
    return top;
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

int main() {
    char map[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    char line[MAX_COLS + 10];
    int start_x = -1, start_y = -1;
    int end_x = -1, end_y = -1;

    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            map[i][j] = '#';
        }
    }

    FILE *fp = fopen("input.txt", "r");
    while(fgets(line, sizeof(line), fp)) {
        int len = strlen(line);
        while(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            len--;
        }
        if(len > cols)
            cols = len;
        if(rows >= MAX_ROWS)
            break;
        for(int j = 0; j < len && j < MAX_COLS; j++) {
            map[rows][j] = line[j];
            if(line[j] == 'S') {
                start_x = j;
                start_y = rows;
            }
            if(line[j] == 'E') {
                end_x = j;
                end_y = rows;
            }
        }
        rows++;
    }
    fclose(fp);

    long *cost_map = malloc(rows * cols * 4 * sizeof(long));
    for(int i = 0; i < rows * cols * 4; i++) cost_map[i] = INF;

    MinHeap* heap = createHeap(10000);
    HeapNode start_node = {start_x, start_y, 0, 0};
    pushHeap(heap, start_node);
    cost_map[(start_y * cols + start_x) * 4 + 0] = 0;

    while(!isEmpty(heap)) {
        HeapNode current = popHeap(heap);
        if(current.x == end_x && current.y == end_y) {
            printf("%ld\n", current.cost);
            free(cost_map);
            free(heap->nodes);
            free(heap);
            return 0;
        }
        if(current.cost > cost_map[(current.y * cols + current.x) * 4 + current.dir])
            continue;
        int new_x = current.x + dx[current.dir];
        int new_y = current.y + dy[current.dir];
        if(new_x >=0 && new_x < cols && new_y >=0 && new_y < rows && map[new_y][new_x] != '#') {
            long new_cost = current.cost + 1;
            int idx = (new_y * cols + new_x) * 4 + current.dir;
            if(new_cost < cost_map[idx]) {
                cost_map[idx] = new_cost;
                HeapNode new_node = {new_x, new_y, current.dir, new_cost};
                pushHeap(heap, new_node);
            }
        }
        for(int turn = 1; turn <= 3; turn +=2) {
            int new_dir = (current.dir + turn) % 4;
            long turn_cost = current.cost + 1000;
            int idx = (current.y * cols + current.x) * 4 + new_dir;
            if(turn_cost < cost_map[idx]) {
                cost_map[idx] = turn_cost;
                HeapNode turn_node = {current.x, current.y, new_dir, turn_cost};
                pushHeap(heap, turn_node);
            }
        }
    }

    free(cost_map);
    free(heap->nodes);
    free(heap);
    return 0;
}

