const fs = require("fs");

const data = fs.readFileSync("input.txt", "utf8").trim();
const robots = data.split("\n").map((line) => {
  const [px, py, vx, vy] = line.match(/-?\d+/g).map(Number);
  return [px, py, vx, vy];
});

const X = 101;
const Y = 103;
const DIRS = [
  [-1, 0],
  [0, 1],
  [1, 0],
  [0, -1],
];

let q1 = 0,
  q2 = 0,
  q3 = 0,
  q4 = 0;
let mx = 0,
  my = 0;

for (let t = 1; t <= 1e6; t++) {
  let G = Array.from({ length: Y }, () => Array(X).fill("."));

  if (t === 100) {
    q1 = q2 = q3 = q4 = 0;
    mx = Math.floor(X / 2);
    my = Math.floor(Y / 2);
  }

  for (let i = 0; i < robots.length; i++) {
    let [px, py, vx, vy] = robots[i];
    px = (((px + vx) % X) + X) % X;
    py = (((py + vy) % Y) + Y) % Y;
    robots[i] = [px, py, vx, vy];
    G[py][px] = "#";

    if (t === 100) {
      if (px < mx && py < my) q1++;
      if (px > mx && py < my) q2++;
      if (px < mx && py > my) q3++;
      if (px > mx && py > my) q4++;
    }
  }

  if (t === 100) {
    console.log(q1 * q2 * q3 * q4);
  }

  let components = 0;
  let seen = new Set();

  for (let x = 0; x < X; x++) {
    for (let y = 0; y < Y; y++) {
      if (G[y][x] === "#" && !seen.has(`${x},${y}`)) {
        components++;
        let queue = [[x, y]];
        while (queue.length) {
          let [cx, cy] = queue.shift();
          let key = `${cx},${cy}`;
          if (seen.has(key)) continue;
          seen.add(key);
          for (const [dx, dy] of DIRS) {
            let nx = cx + dx,
              ny = cy + dy;
            if (
              nx >= 0 &&
              nx < X &&
              ny >= 0 &&
              ny < Y &&
              G[ny][nx] === "#" &&
              !seen.has(`${nx},${ny}`)
            ) {
              queue.push([nx, ny]);
            }
          }
        }
      }
    }
  }
  if (components <= 200) {
    console.log(t);
    break;
  }
}

