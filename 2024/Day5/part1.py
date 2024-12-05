def parse(filename):
   with open(filename) as f:
       rules, updates = f.read().strip().split('\n\n')

   rules = [tuple(map(int, r.split('|'))) for r in rules.splitlines()]
   updates = [list(map(int, u.split(','))) for u in updates.splitlines()]
   total = 0

   for nums in updates:
       pos = {n: i for i,n in enumerate(nums)}
       if not any(x in pos and y in pos and pos[x] > pos[y] for x,y in rules):
           total += nums[len(nums)//2]

   return total
print(parse('input.txt'))
