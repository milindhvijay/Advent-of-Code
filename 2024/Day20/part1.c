#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 200
#define MAX_QUEUE 40000

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point pos;
    int steps;
} QueueItem;

char maze[MAX_SIZE][MAX_SIZE];
int rows = 0, cols = 0;
Point start, end;
QueueItem queue[MAX_QUEUE];
bool visited[MAX_SIZE][MAX_SIZE];
int distances[MAX_SIZE][MAX_SIZE];
int dist_to_end[MAX_SIZE][MAX_SIZE];
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

bool is_valid(int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

void read_maze() {
    FILE *fp = fopen("input.txt", "r");
    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        strcpy(maze[rows], line);
        for (int j = 0; j < strlen(line); j++) {
            if (maze[rows][j] == 'S') {
                start = (Point){rows, j};
                maze[rows][j] = '.';
            } else if (maze[rows][j] == 'E') {
                end = (Point){rows, j};
                maze[rows][j] = '.';
            }
        }
        rows++;
    }
    cols = strlen(maze[0]);
    fclose(fp);
}

void find_distances(Point start_pos, int dist[MAX_SIZE][MAX_SIZE]) {
    memset(visited, false, sizeof(visited));
    memset(dist, -1, sizeof(int) * MAX_SIZE * MAX_SIZE);
    int front = 0, rear = 0;
    queue[rear++] = (QueueItem){start_pos, 0};
    visited[start_pos.x][start_pos.y] = true;
    dist[start_pos.x][start_pos.y] = 0;

    while (front < rear) {
        QueueItem current = queue[front++];
        for (int i = 0; i < 4; i++) {
            int new_x = current.pos.x + dx[i];
            int new_y = current.pos.y + dy[i];
            if (!is_valid(new_x, new_y)) continue;
            if (maze[new_x][new_y] == '.' && !visited[new_x][new_y]) {
                queue[rear++] = (QueueItem){(Point){new_x, new_y}, current.steps + 1};
                visited[new_x][new_y] = true;
                dist[new_x][new_y] = current.steps + 1;
            }
        }
    }
}

int solve() {
    find_distances(start, distances);
    find_distances(end, dist_to_end);
    int normal_path = distances[end.x][end.y];
    if (normal_path == -1) return 0;
    int count = 0;

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (maze[x][y] != '#') continue;
            for (int d1 = 0; d1 < 4; d1++) {
                int start_x = x + dx[d1];
                int start_y = y + dy[d1];
                if (!is_valid(start_x, start_y) || maze[start_x][start_y] != '.') continue;
                if (distances[start_x][start_y] == -1) continue;

                for (int d2 = 0; d2 < 4; d2++) {
                    int end_x = x + dx[d2];
                    int end_y = y + dy[d2];
                    if (!is_valid(end_x, end_y) || maze[end_x][end_y] != '.') continue;
                    if (dist_to_end[end_x][end_y] == -1) continue;
                    int total = distances[start_x][start_y] + abs(end_x - start_x) + abs(end_y - start_y) + dist_to_end[end_x][end_y];
                    if (normal_path - total >= 100) {
                        count++;
                        goto next_wall;
                    }
                }
            }
            next_wall:;
        }
    }
    return count;
}

int main() {
    read_maze();
    printf("%d\n", solve());
    return 0;
}

