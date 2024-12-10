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

function findReachableNines(startY, startX) {
  const queue = []
  const visited = Array.from({ length: numRows }, () => Array(numCols).fill(false))
  const reachableNines = new Set()

  queue.push({ y: startY, x: startX, height: 0 })
  visited[startY][startX] = true

  while (queue.length > 0) {
    const { y, x, height } = queue.shift()

    if (height === 9) {
      reachableNines.add(`${y},${x}`)
      continue
    }

    for (const dir of directions) {
      const newY = y + dir.dy
      const newX = x + dir.dx
      
      if (isValid(newY, newX) && !visited[newY][newX]) {
        const nextHeight = grid[newY][newX]
        if (nextHeight === height + 1) {
          queue.push({ y: newY, x: newX, height: nextHeight })
          visited[newY][newX] = true
        }
      }
    }
  }

  return reachableNines.size
}

let totalScore = 0
for (const trailhead of trailheads) {
  totalScore += findReachableNines(trailhead.y, trailhead.x)
}

console.log(totalScore)
