#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 200
#define MAX_QUEUE 40000
#define MAX_CHEAT_STEPS 20

typedef struct {
    int x, y;
} Point;

char maze[MAX_SIZE][MAX_SIZE];
int rows = 0, cols = 0;
Point start, end;
Point queue[MAX_QUEUE];
bool visited[MAX_SIZE][MAX_SIZE];
int distances[MAX_SIZE][MAX_SIZE];
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, 1, -1};

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

void find_distances() {
    memset(distances, -1, sizeof(distances));
    memset(visited, false, sizeof(visited));
    int front = 0, rear = 0;
    queue[rear++] = start;
    distances[start.x][start.y] = 0;
    visited[start.x][start.y] = true;

    while (front < rear) {
        Point cur = queue[front++];
        int steps = distances[cur.x][cur.y];
        for (int d = 0; d < 4; d++) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (is_valid(nx, ny) && maze[nx][ny] == '.' && !visited[nx][ny]) {
                distances[nx][ny] = steps + 1;
                visited[nx][ny] = true;
                queue[rear++] = (Point){nx, ny};
            }
        }
    }
}

int cheat_count = 0;

void check_cheats(int si, int sj, int steps) {
    memset(visited, false, sizeof(visited));
    int front = 0, rear = 0;
    queue[rear++] = (Point){si, sj};
    visited[si][sj] = true;

    for (int cheat_steps = 0; cheat_steps < MAX_CHEAT_STEPS; cheat_steps++) {
        int level_size = rear - front;
        if (level_size == 0) break;
        while (level_size--) {
            Point cur = queue[front++];
            for (int d = 0; d < 4; d++) {
                int nx = cur.x + dx[d];
                int ny = cur.y + dy[d];
                if (is_valid(nx, ny) && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    queue[rear++] = (Point){nx, ny};
                    if (distances[nx][ny] != -1 && distances[nx][ny] >= steps + 100 + cheat_steps) {
                        cheat_count++;
                    }
                }
            }
        }
    }
}

int solve() {
    cheat_count = 0;
    find_distances();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (distances[i][j] != -1) {
                check_cheats(i, j, distances[i][j]);
            }
        }
    }
    return cheat_count;
}

int main() {
    read_maze();
    printf("%d\n", solve());
    return 0;
}

