def parse(filename):
    with open(filename) as f:
        rules, updates = f.read().strip().split('\n\n')

    rules = [tuple(map(int, r.split('|'))) for r in rules.splitlines()]
    updates = [list(map(int, u.split(','))) for u in updates.splitlines()]
    total = 0

    for nums in updates:
        pos = {n: i for i, n in enumerate(nums)}
        if any(x in pos and y in pos and pos[x] > pos[y] for x, y in rules):
            graph = {}
            degree = {}
            numset = set(nums)

            for x, y in rules:
                if x in numset and y in numset:
                    if x not in graph:
                        graph[x] = []
                    graph[x].append(y)
                    degree[y] = degree.get(y, 0) + 1
                    if x not in degree:
                        degree[x] = 0

            queue = [n for n in nums if degree.get(n, 0) == 0]
            sorted_nums = []

            while queue:
                current = queue.pop(0)
                sorted_nums.append(current)
                for neighbour in graph.get(current, []):
                    degree[neighbour] -= 1
                    if degree[neighbour] == 0:
                        queue.append(neighbour)

            total += sorted_nums[len(sorted_nums) // 2]

    return total
print(parse('input.txt'))
