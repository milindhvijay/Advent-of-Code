const fs = require("fs");

const coords = fs
  .readFileSync("input.txt", "utf8")
  .trim()
  .split("\n")
  .map((l) => {
    const [x, y] = l.split(",").map(Number);
    return { x, y };
  });

const corrupted = new Set();
for (let i = 0; i < Math.min(coords.length, 1024); i++) {
  corrupted.add(`${coords[i].x},${coords[i].y}`);
}

const q = [{ x: 0, y: 0, s: 0 }];
const seen = new Set(["0,0"]);
const dirs = [
  [0, 1],
  [0, -1],
  [1, 0],
  [-1, 0],
];

while (q.length) {
  const cur = q.shift();

  if (cur.x === 70 && cur.y === 70) {
    console.log(cur.s);
    process.exit();
  }

  for (const [dx, dy] of dirs) {
    const nx = cur.x + dx;
    const ny = cur.y + dy;
    const k = `${nx},${ny}`;

    if (
      nx >= 0 &&
      nx <= 70 &&
      ny >= 0 &&
      ny <= 70 &&
      !corrupted.has(k) &&
      !seen.has(k)
    ) {
      q.push({ x: nx, y: ny, s: cur.s + 1 });
      seen.add(k);
    }
  }
}

