const fs = require('fs')
const path = require('path')

const input = fs.readFileSync(path.join(__dirname, 'input.txt'), 'utf-8').trim()
const grid = input.split('\n').map(line => line.trim().split('').map(Number))
const numRows = grid.length
const numCols = grid[0].length

const directions = [
  { dy: -1, dx: 0 },
  { dy: 1, dx: 0 },
  { dy: 0, dx: -1 },
  { dy: 0, dx: 1 }
]

function isValid(y, x) {
  return y >= 0 && y < numRows && x >= 0 && x < numCols
}

const trailheads = []
for (let y = 0; y < numRows; y++) {
  for (let x = 0; x < numCols; x++) {
    if (grid[y][x] === 0) {
      trailheads.push({ y, x })
    }
  }
}

const memo = Array.from({ length: numRows }, () => Array(numCols).fill(-1))

function countTrails(y, x) {
  if (grid[y][x] === 9) {
    return 1
  }

  if (memo[y][x] !== -1) {
    return memo[y][x]
  }

  let count = 0
  const currentHeight = grid[y][x]

  for (const dir of directions) {
    const newY = y + dir.dy
    const newX = x + dir.dx
    
    if (isValid(newY, newX)) {
      const nextHeight = grid[newY][newX]
      if (nextHeight === currentHeight + 1) {
        count += countTrails(newY, newX)
      }
    }
  }

  memo[y][x] = count
  return count
}

let totalRating = 0
for (const trailhead of trailheads) {
  totalRating += countTrails(trailhead.y, trailhead.x)
}

console.log(totalRating)
