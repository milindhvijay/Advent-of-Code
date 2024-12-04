const fs = require("fs");

function count(grid) {
  const rows = grid.length;
  let total = 0;

  function check(x, y) {
    try {
      const d1 = grid[x - 1][y - 1] + grid[x][y] + grid[x + 1][y + 1];
      const d2 = grid[x - 1][y + 1] + grid[x][y] + grid[x + 1][y - 1];
      return (d1 === "MAS" || d1 === "SAM") && (d2 === "MAS" || d2 === "SAM");
    } catch {
      return false;
    }
  }

  for (let x = 1; x < rows - 1; x++) {
    for (let y = 1; y < grid[x].length - 1; y++) {
      total += check(x, y) ? 1 : 0;
    }
  }
  return total;
}

const grid = fs.readFileSync("input.txt", "utf-8").trim().split("\n");
console.log(count(grid));

