with open('input.txt', 'r') as f:
    lines = f.readlines()
    grid = []
    movements = ''
    reading_map = True
    for line in lines:
        if reading_map:
            if not line.strip():
                reading_map = False
            else:
                row = list(line.strip())
                grid.append(row)
        else:
            movements += line.strip()

width = len(grid[0])
height = len(grid)
robot = [0, 0]

for y in range(height):
    for x in range(width):
        if grid[y][x] == '@':
            robot = [x, y]

def get_obj(grid, coord):
    return grid[coord[1]][coord[0]]

def move(grid, robot, dir):
    nr, nc = robot[1] + dir[0], robot[0] + dir[1]
    if grid[nr][nc] == '.':
        grid[robot[1]][robot[0]] = '.'
        grid[nr][nc] = '@'
        robot[0] = nc
        robot[1] = nr
    elif grid[nr][nc] == 'O':
        boxes = []
        tr, tc = nr, nc
        while tr < height and tc < width and grid[tr][tc] == 'O':
            boxes.append((tr, tc))
            tr += dir[0]
            tc += dir[1]
        if tr < height and tc < width and grid[tr][tc] == '.':
            for br, bc in reversed(boxes):
                grid[br][bc] = '.'
                grid[br + dir[0]][bc + dir[1]] = 'O'
            grid[robot[1]][robot[0]] = '.'
            grid[nr][nc] = '@'
            robot[0] = nc
            robot[1] = nr

for m in movements:
    if m == '<': move(grid, robot, [0, -1])
    elif m == '>': move(grid, robot, [0, 1])
    elif m == '^': move(grid, robot, [-1, 0])
    elif m == 'v': move(grid, robot, [1, 0])

result = 0
for y in range(height):
    for x in range(width):
        if grid[y][x] == 'O':
            result += y * 100 + x
print(result)
