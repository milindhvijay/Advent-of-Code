def get_map(path):
    with open(path, 'r') as f:
        return [list(line.strip()) for line in f if line.strip()]

def find_start(grid):
    R, C = len(grid), len(grid[0])
    dir_map = {'^': (-1, 0), '>': (0, 1), 'v': (1, 0), '<': (0, -1)}
    dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    for r in range(R):
        for c in range(C):
            if grid[r][c] in dir_map:
                return r, c, dirs.index(dir_map[grid[r][c]])
    return -1, -1, -1

def simulate_path(grid, start_r, start_c, start_d):
    R, C = len(grid), len(grid[0])
    dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]
    r, c, d = start_r, start_c, start_d
    path = set()
    visited_states = set()

    while True:
        path.add((r, c))
        state = (r, c, d)

        if state in visited_states:
            return True, path
        visited_states.add(state)

        nr, nc = r + dirs[d][0], c + dirs[d][1]
        if not (0 <= nr < R and 0 <= nc < C):
            return False, path

        if grid[nr][nc] == '#':
            d = (d + 1) % 4
        else:
            r, c = nr, nc

    return False, path

def count_loop_positions(grid):
    R, C = len(grid), len(grid[0])
    start_r, start_c, start_d = find_start(grid)
    is_loop_orig, original_path = simulate_path(grid, start_r, start_c, start_d)
    loops = 0

    for r in range(R):
        for c in range(C):
            if grid[r][c] == '.' and (r, c) != (start_r, start_c):
                grid[r][c] = '#'
                is_loop, path = simulate_path(grid, start_r, start_c, start_d)
                if is_loop and path != original_path:
                    loops += 1
                grid[r][c] = '.'

    return loops

if __name__ == '__main__':
    grid = get_map('input.txt')
    result = count_loop_positions(grid)
    print(result)

