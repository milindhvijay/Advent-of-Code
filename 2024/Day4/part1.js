const fs = require("fs");

function count(word, grid) {
  const rows = grid.length;
  const cols = grid[0].length;
  const dirs = [
    [1, 0],
    [0, 1],
    [1, 1],
    [1, -1],
    [-1, 0],
    [0, -1],
    [-1, -1],
    [-1, 1],
  ];
  let total = 0;

  function check(x, y, dx, dy) {
    for (let i = 0; i < word.length; i++) {
      const nx = x + i * dx;
      const ny = y + i * dy;
      if (
        nx < 0 ||
        nx >= rows ||
        ny < 0 ||
        ny >= cols ||
        grid[nx][ny] !== word[i]
      ) {
        return false;
      }
    }
    return true;
  }

  for (let x = 0; x < rows; x++) {
    for (let y = 0; y < cols; y++) {
      dirs.forEach(([dx, dy]) => (total += check(x, y, dx, dy) ? 1 : 0));
    }
  }
  return total;
}

const grid = fs.readFileSync("input.txt", "utf-8").trim().split("\n");
console.log(count("XMAS", grid));

