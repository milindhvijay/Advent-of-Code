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
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    if (idx == 0)
        return;
    int parent = (idx - 1) / 2;
    if (heap->nodes[parent].cost > heap->nodes[idx].cost) {
        swap(&heap->nodes[parent], &heap->nodes[idx]);
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->nodes[left].cost < heap->nodes[smallest].cost)
        smallest = left;
    if (right < heap->size && heap->nodes[right].cost < heap->nodes[smallest].cost)
        smallest = right;
    if (smallest != idx) {
        swap(&heap->nodes[smallest], &heap->nodes[idx]);
        heapify_down(heap, smallest);
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

void freeHeap(MinHeap* heap) {
    free(heap->nodes);
    free(heap);
}

void dijkstra(char map[][MAX_COLS], int rows, int cols, int start_x, int start_y, long ***cost_map, int start_dir_count, int *start_dirs) {
    MinHeap* heap = createHeap(10000);
    for(int i = 0; i < start_dir_count; i++) {
        int dir = start_dirs[i];
        HeapNode start_node;
        start_node.x = start_x;
        start_node.y = start_y;
        start_node.dir = dir;
        start_node.cost = 0;
        pushHeap(heap, start_node);
        cost_map[start_y][start_x][dir] = 0;
    }

    while(!isEmpty(heap)) {
        HeapNode current = popHeap(heap);
        if(current.cost > cost_map[current.y][current.x][current.dir])
            continue;
        int new_x = current.x + dx[current.dir];
        int new_y = current.y + dy[current.dir];
        if(new_x >=0 && new_x < cols && new_y >=0 && new_y < rows && map[new_y][new_x] != '#') {
            long new_cost = current.cost + 1;
            if(new_cost < cost_map[new_y][new_x][current.dir]) {
                cost_map[new_y][new_x][current.dir] = new_cost;
                HeapNode new_node;
                new_node.x = new_x;
                new_node.y = new_y;
                new_node.dir = current.dir;
                new_node.cost = new_cost;
                pushHeap(heap, new_node);
            }
        }
        int new_dir = (current.dir + 1) % 4;
        long turn_cost = current.cost + 1000;
        if(turn_cost < cost_map[current.y][current.x][new_dir]) {
            cost_map[current.y][current.x][new_dir] = turn_cost;
            HeapNode turn_node;
            turn_node.x = current.x;
            turn_node.y = current.y;
            turn_node.dir = new_dir;
            turn_node.cost = turn_cost;
            pushHeap(heap, turn_node);
        }
        new_dir = (current.dir + 3) % 4;
        turn_cost = current.cost + 1000;
        if(turn_cost < cost_map[current.y][current.x][new_dir]) {
            cost_map[current.y][current.x][new_dir] = turn_cost;
            HeapNode turn_node;
            turn_node.x = current.x;
            turn_node.y = current.y;
            turn_node.dir = new_dir;
            turn_node.cost = turn_cost;
            pushHeap(heap, turn_node);
        }
    }
    freeHeap(heap);
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

    long ***cost_from_start = (long ***)malloc(rows * sizeof(long **));
    for(int i = 0; i < rows; i++) {
        cost_from_start[i] = (long **)malloc(cols * sizeof(long *));
        for(int j = 0; j < cols; j++) {
            cost_from_start[i][j] = (long *)malloc(4 * sizeof(long));
            for(int d = 0; d < 4; d++) {
                cost_from_start[i][j][d] = INF;
            }
        }
    }

    long ***cost_from_end = (long ***)malloc(rows * sizeof(long **));
    for(int i = 0; i < rows; i++) {
        cost_from_end[i] = (long **)malloc(cols * sizeof(long *));
        for(int j = 0; j < cols; j++) {
            cost_from_end[i][j] = (long *)malloc(4 * sizeof(long));
            for(int d = 0; d < 4; d++) {
                cost_from_end[i][j][d] = INF;
            }
        }
    }

    int start_dirs_S[1] = {0};
    int start_dirs_E[4] = {0, 1, 2, 3};

    dijkstra(map, rows, cols, start_x, start_y, cost_from_start, 1, start_dirs_S);
    dijkstra(map, rows, cols, end_x, end_y, cost_from_end, 4, start_dirs_E);

    long minimal_cost = INF;
    for(int d = 0; d < 4; d++) {
        if(cost_from_start[end_y][end_x][d] < minimal_cost)
            minimal_cost = cost_from_start[end_y][end_x][d];
    }

    if(minimal_cost == INF) {
        printf("No path found from S to E.\n");
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                free(cost_from_start[i][j]);
                free(cost_from_end[i][j]);
            }
            free(cost_from_start[i]);
            free(cost_from_end[i]);
        }
        free(cost_from_start);
        free(cost_from_end);
        return 1;
    }

    int count = 0;
    char marked_map[MAX_ROWS][MAX_COLS];
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            marked_map[i][j] = map[i][j];
        }
    }

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(map[i][j] == '#') continue;
            for(int d1 = 0; d1 < 4; d1++) {
                if(cost_from_start[i][j][d1] == INF) continue;
                for(int d2 = 0; d2 < 4; d2++) {
                    if(cost_from_end[i][j][d2] == INF) continue;
                    if(cost_from_start[i][j][d1] + cost_from_end[i][j][d2] == minimal_cost) {
                        if(marked_map[i][j] != 'O') {
                            marked_map[i][j] = 'O';
                            count++;
                        }
                        break;
                    }
                }
                if(marked_map[i][j] == 'O') break;
            }
        }
    }
    printf("%d\n",count);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            free(cost_from_start[i][j]);
            free(cost_from_end[i][j]);
        }
        free(cost_from_start[i]);
        free(cost_from_end[i]);
    }
    free(cost_from_start);
    free(cost_from_end);

    return 0;
}

