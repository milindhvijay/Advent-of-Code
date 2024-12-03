def get_total_distance(left, right):
    left.sort()
    right.sort()
    return sum(abs(a - b) for a, b in zip(left, right))

left, right = [], []

with open('input.txt', 'r') as file:
    for line in file:
        line = line.strip()
        if not line:
            break
        x, y = map(int, line.split())
        left.append(x)
        right.append(y)

print(get_total_distance(left, right))
