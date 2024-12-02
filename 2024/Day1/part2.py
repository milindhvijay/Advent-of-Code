def get_similarity(left, right):
   freq = {}
   for num in right:
       if num in freq:
           freq[num] += 1
       else:
           freq[num] = 1
   return sum(num * freq.get(num, 0) for num in left)

left, right = [], []

while True:
   line = input().strip()
   if not line:
       break
   x, y = map(int, line.split())
   left.append(x)
   right.append(y)

print(get_similarity(left, right))
