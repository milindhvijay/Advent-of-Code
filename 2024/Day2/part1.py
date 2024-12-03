def is_safe(report):
   if len(report) <= 1:
       return True
   first_diff = report[1] - report[0]
   valid = range(1, 4) if first_diff > 0 else range(-3, 0)
   return all(b - a in valid for a, b in zip(report, report[1:]))

safe_reports = 0

with open('input.txt', 'r') as file:
    for line in file:
        line = input().strip()
        if not line:
            break
        report = [int(x) for x in line.split()]
        if is_safe(report):
            safe_reports += 1

print(safe_reports)
