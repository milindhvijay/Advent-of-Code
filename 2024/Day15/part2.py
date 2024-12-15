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
                row = []
                for ch in line.strip():
                    if ch == '#':
                        row += ['#', '#']
                    elif ch == 'O':
                        row += ['[', ']']
                    elif ch == '.':
                        row += ['.', '.']
                    elif ch == '@':
                        row += ['@', '.']
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
    pending = {(robot[0], robot[1])}
    visited = set()
    obj_dist = {}

    while pending:
        pos = pending.pop()
        visited.add(pos)
        target = (pos[0] + dir[0], pos[1] + dir[1])
        t_obj = get_obj(grid, target)

        if t_obj == '#':
            return

        if t_obj == '[':
            if target not in visited:
                pending.add(target)
            if (target[0] + 1, target[1]) not in visited:
                pending.add((target[0] + 1, target[1]))
        elif t_obj == ']':
            if (target[0] - 1, target[1]) not in visited:
                pending.add((target[0] - 1, target[1]))
            if target not in visited:
                pending.add(target)

        dist = max([(pos[0] - robot[0]) * dir[0], (pos[1] - robot[1]) * dir[1]])
        if dist not in obj_dist:
            obj_dist[dist] = []
        obj_dist[dist].append(pos)

    if obj_dist:
        for i in range(max(obj_dist.keys()), -1, -1):
            for pos in obj_dist[i]:
                target = [pos[0] + dir[0], pos[1] + dir[1]]
                obj = get_obj(grid, pos)
                if grid[target[1]][target[0]] == '.':
                    grid[target[1]][target[0]] = obj
                    grid[pos[1]][pos[0]] = '.'
                    if obj == '@':
                        robot[0] = target[0]
                        robot[1] = target[1]

for m in movements:
    if m == '<': move(grid, robot, [-1, 0])
    elif m == '>': move(grid, robot, [1, 0])
    elif m == '^': move(grid, robot, [0, -1])
    elif m == 'v': move(grid, robot, [0, 1])

result = 0
for y in range(height):
    for x in range(width):
        if grid[y][x] == '[':
            result += y * 100 + x
print(result)
