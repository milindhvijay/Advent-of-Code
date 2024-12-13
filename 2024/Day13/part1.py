import re

def parse_machine(block):
    lines = block.strip().split('\n')
    if len(lines) < 3:
        return None
    match_a = re.match(r'Button A:\s*X\+(\d+),\s*Y\+(\d+)', lines[0])
    if not match_a:
        return None
    a1, a2 = map(int, match_a.groups())
    match_b = re.match(r'Button B:\s*X\+(\d+),\s*Y\+(\d+)', lines[1])
    if not match_b:
        return None
    b1, b2 = map(int, match_b.groups())
    match_p = re.match(r'Prize:\s*X=(\d+),\s*Y=(\d+)', lines[2])
    if not match_p:
        return None
    px, py = map(int, match_p.groups())
    return (a1, a2, b1, b2, px, py)

def read_input(filename):
    with open(filename, 'r') as f:
        content = f.read()
    blocks = content.strip().split('\n\n')
    machines = []
    for block in blocks:
        machine = parse_machine(block)
        if machine:
            machines.append(machine)
    return machines

def find_min_tokens(a1, a2, b1, b2, px, py):
    min_cost = None
    for a in range(0, 101):
        rem_x = px - a * a1
        rem_y = py - a * a2
        if rem_x < 0 or rem_y < 0:
            continue
        if b1 == 0:
            if rem_x != 0:
                continue
            possible_b_x = set(range(0, 101))
        else:
            if rem_x % b1 != 0:
                continue
            b_x = rem_x // b1
            if not (0 <= b_x <= 100):
                continue
            possible_b_x = {b_x}
        if b2 == 0:
            if rem_y != 0:
                continue
            possible_b_y = set(range(0, 101))
        else:
            if rem_y % b2 != 0:
                continue
            b_y = rem_y // b2
            if not (0 <= b_y <= 100):
                continue
            possible_b_y = {b_y}
        possible_b = possible_b_x & possible_b_y
        if possible_b:
            b = possible_b.pop()
            cost = a * 3 + b * 1
            if min_cost is None or cost < min_cost:
                min_cost = cost
    return min_cost

def main():
    machines = read_input('input.txt')
    total_tokens = 0
    for machine in machines:
        a1, a2, b1, b2, px, py = machine
        min_cost = find_min_tokens(a1, a2, b1, b2, px, py)
        if min_cost is not None:
            total_tokens += min_cost
    print(total_tokens)

if __name__ == '__main__':
    main()

