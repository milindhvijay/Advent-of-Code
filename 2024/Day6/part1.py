def get_map(path):
    with open(path, 'r') as f:
        return [list(line.strip()) for line in f if line.strip()]

def count_visited(grid):
    R, C = len(grid), len(grid[0])
    dir_map = {'^': (-1, 0), '>': (0, 1), 'v': (1, 0), '<': (0, -1)}
    dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    start = next((i, j) for i in range(R) for j in range(C) if grid[i][j] in dir_map)
    r, c = start
    d = dirs.index(dir_map[grid[r][c]])
    seen = {(r, c)}

    while True:
        nr, nc = r + dirs[d][0], c + dirs[d][1]
        if not (0 <= nr < R and 0 <= nc < C):
            break

        if grid[nr][nc] == '#':
            d = (d + 1) % 4
            if len(seen) > 1 and (r, c) in seen and grid[r][c] in dir_map:
                break
        else:
            r, c = nr, nc
            seen.add((r, c))

    return len(seen)

if __name__ == '__main__':
    print(count_visited(get_map('input.txt')))

