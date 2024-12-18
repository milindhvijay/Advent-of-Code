const fs = require("fs");

const coords = fs
  .readFileSync("input.txt", "utf8")
  .trim()
  .split("\n")
  .map((l) => {
    const [x, y] = l.split(",").map(Number);
    return { x, y };
  });

function hasPath(blocked) {
  const q = [{ x: 0, y: 0 }];
  const seen = new Set(["0,0"]);
  const dirs = [
    [0, 1],
    [0, -1],
    [1, 0],
    [-1, 0],
  ];

  while (q.length) {
    const cur = q.shift();
    if (cur.x === 70 && cur.y === 70) return true;

    for (const [dx, dy] of dirs) {
      const nx = cur.x + dx;
      const ny = cur.y + dy;
      const k = `${nx},${ny}`;

      if (
        nx >= 0 &&
        nx <= 70 &&
        ny >= 0 &&
        ny <= 70 &&
        !blocked.has(k) &&
        !seen.has(k)
      ) {
        q.push({ x: nx, y: ny });
        seen.add(k);
      }
    }
  }
  return false;
}

const blocked = new Set();
for (const c of coords) {
  blocked.add(`${c.x},${c.y}`);
  if (!hasPath(blocked)) {
    console.log(`${c.x},${c.y}`);
    process.exit();
  }
}

