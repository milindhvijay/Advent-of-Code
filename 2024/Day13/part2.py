import re
import math

def extended_gcd(a, b):
    if b == 0:
        return (a, 1, 0)
    g, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return (g, x, y)

def modinv(a, m):
    g, x, _ = extended_gcd(a, m)
    if g != 1:
        return None
    return x % m

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

def read_input():
    with open('input.txt', 'r') as f:
        content = f.read()
    blocks = content.strip().split('\n\n')
    machines = []
    for block in blocks:
        machine = parse_machine(block)
        if machine:
            machines.append(machine)
    return machines

def find_min_tokens(A_x, A_y, B_x, B_y, px, py):
    det = A_x * B_y - A_y * B_x
    if det != 0:
        a_num = px * B_y - py * B_x
        b_num = A_x * py - A_y * px
        if det < 0:
            det = -det
            a_num = -a_num
            b_num = -b_num
        if a_num % det != 0 or b_num % det != 0:
            return None
        a = a_num // det
        b = b_num // det
        if a < 0 or b < 0:
            return None
        return 3 * a + b
    if (A_x * py - A_y * px) != 0 or (B_y * px - B_x * py) != 0:
        return None
    g = math.gcd(A_x, B_x)
    if px % g != 0:
        return None
    A_x_p = A_x // g
    B_x_p = B_x // g
    px_p = px // g
    inv_Bx = modinv(B_x_p, A_x_p)
    if inv_Bx is None:
        return None
    k0 = (px_p * inv_Bx) % A_x_p
    a_initial = (px - B_x * k0) // A_x
    t_max = a_initial // B_x_p if B_x_p != 0 else 0
    coeff = A_x_p - 3 * B_x_p
    costs = []
    if coeff > 0:
        t = 0
        a = (px - B_x * (k0 + t * A_x_p)) // A_x
        b = k0 + t * A_x_p
        if a >= 0 and b >= 0:
            costs.append(3 * a + b)
    elif coeff < 0:
        t = t_max
        if t < 0:
            t = 0
        a = (px - B_x * (k0 + t * A_x_p)) // A_x
        b = k0 + t * A_x_p
        if a >= 0 and b >= 0:
            costs.append(3 * a + b)
    else:
        t = 0
        a = (px - B_x * k0) // A_x
        b = k0
        if a >= 0 and b >= 0:
            costs.append(3 * a + b)
    if not costs:
        return None
    return min(costs)

def main():
    machines = read_input()
    total = 0
    OFFSET = 10_000_000_000_000
    for m in machines:
        A_x, A_y, B_x, B_y, px, py = m
        px += OFFSET
        py += OFFSET
        cost = find_min_tokens(A_x, A_y, B_x, B_y, px, py)
        if cost is not None:
            total += cost
    print(total)

if __name__ == '__main__':
    main()

