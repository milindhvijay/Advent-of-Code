def is_safe(report):
   def check_diffs(arr):
       diffs = [arr[i+1] - arr[i] for i in range(len(arr)-1)]
       return all(1 <= d <= 3 for d in diffs) or all(-3 <= d <= -1 for d in diffs)

   if check_diffs(report):
       return True

   return any(check_diffs(report[:i] + report[i+1:]) for i in range(len(report)))

reports = []
while True:
   line = input().strip()
   if not line:
       break
   reports.append([int(x) for x in line.split()])

print(sum(1 for r in reports if is_safe(r)))
