def get_total_distance(left, right):
    left.sort()
    right.sort()
    return sum(abs(a - b) for a, b in zip(left, right))

left, right = [], []

while True:
    line = input().strip()
    if not line:
        break
    x, y = map(int, line.split())
    left.append(x)
    right.append(y)

print(get_total_distance(left, right))
