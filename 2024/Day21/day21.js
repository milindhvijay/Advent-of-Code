const fs = require("fs");

const keypads = [
  {
    7: [0, 0],
    8: [0, 1],
    9: [0, 2],
    4: [1, 0],
    5: [1, 1],
    6: [1, 2],
    1: [2, 0],
    2: [2, 1],
    3: [2, 2],
    "-": [3, 0],
    0: [3, 1],
    A: [3, 2],
  },
  {
    "-": [0, 0],
    "^": [0, 1],
    A: [0, 2],
    "<": [1, 0],
    v: [1, 1],
    ">": [1, 2],
  },
];

let cache;

function calculateDistance(step, seq, totalSteps, start = "A") {
  if (step === totalSteps) return seq.length;
  if (cache[step][seq]) return cache[step][seq];

  let totalDistance = 0;
  for (let i = 0; i < seq.length; i++) {
    const keypad = keypads[+(step > 0)];
    const [startX, startY] = keypad[start];
    const [endX, endY] = keypad[seq[i]];
    const [gapX, gapY] = keypad["-"];

    const moveVertical = (startX > endX ? "^" : "v").repeat(
      Math.abs(startX - endX),
    );
    const moveHorizontal = (startY > endY ? "<" : ">").repeat(
      Math.abs(startY - endY),
    );

    let minDistance = Infinity;

    if (startX !== gapX || endY !== gapY) {
      minDistance = Math.min(
        minDistance,
        calculateDistance(
          step + 1,
          moveHorizontal + moveVertical + "A",
          totalSteps,
        ),
      );
    }

    if (endX !== gapX || startY !== gapY) {
      minDistance = Math.min(
        minDistance,
        calculateDistance(
          step + 1,
          moveVertical + moveHorizontal + "A",
          totalSteps,
        ),
      );
    }

    totalDistance += minDistance;
    start = seq[i];
  }

  return (cache[step][seq] = totalDistance);
}

function solve(input, robots) {
  cache = Array.from({ length: robots + 1 }, () => ({}));
  return input.reduce((total, code) => {
    return total + calculateDistance(0, code, robots + 1) * parseInt(code);
  }, 0);
}

const input = fs.readFileSync("input.txt", "utf8").trim().split("\n");
console.log(solve(input, 2));
console.log(solve(input, 25));

