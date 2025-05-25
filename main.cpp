#include <oneapi/tbb.h>
#include <vector>
#include <array>
#include <limits>
#include <iostream>

int generate_random_int(int min, int max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> pool(min, max);
  return pool(gen);
}
struct best_path
{
  std::vector<int> path;
  int cost;
};
best_path find_best_path(const std::array<std::array<int, 10>, 10> adjMaterix, std::vector<int> path)
{
  std::vector<int> bestPath;
  int bestCost = std::numeric_limits<int>::max();
  while (std::next_permutation(path.begin() + 1, path.end() - 1))
  {
    int cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
      if (adjMaterix[path[i]][path[i + 1]] == -1)
      {
        cost = std::numeric_limits<int>::max();
      }
      else
      {
        cost += adjMaterix[path[i]][path[i + 1]];
      }
    }
    if (cost < bestCost)
    {
      bestCost = cost;
      bestPath = path;
    }
  }
  return {bestPath, bestCost};
}
const std::array<std::array<int, 10>, 10> generate_adjacency_matrix()
{
  std::array<std::array<int, 10>, 10> adjMatrix;
  for (int i = 0; i < 10; ++i)
  {
    for (int j = 0; j < 10; ++j)
    {
      if (i == j)
      {
        adjMatrix[i][j] = 0;
      }
      else
      {
        adjMatrix[i][j] = generate_random_int(-1, 255);
      }
    }
  }
  return adjMatrix;
}
const std::array<std::array<int, 10>, 10> shift_adjacency_matrix(const std::array<std::array<int, 10>, 10> &adjMatrix)
{
  std::array<std::array<int, 10>, 10> shiftedMatrix = adjMatrix;
  int numChanges = 22;
  for (int i = 0; i < numChanges; ++i)
  {
    int row = generate_random_int(0, 9);
    int col = generate_random_int(0, 9);
    while (row == col)
    {
      row = generate_random_int(0, 9);
      col = generate_random_int(0, 9);
    }
    shiftedMatrix[row][col] = generate_random_int(-1, 255);
    shiftedMatrix[col][row] = shiftedMatrix[row][col];
  }
  return shiftedMatrix;
}
int main()
{
  std::vector<int> path = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::array<std::array<int, 10>, 10> adjMatrix = generate_adjacency_matrix();
  std::cout << "Adjacency Matrix:\n";
  for (size_t i = 0; i < adjMatrix.size(); ++i)
  {
    for (size_t j = 0; j < adjMatrix[i].size(); ++j)
    {
      std::cout << adjMatrix[i][j] << " ";
    }
    std::cout << "\n";
  }
  best_path bestPath = find_best_path(adjMatrix, path);
  std::cout << "Best path: ";
  for (size_t i = 0; i < bestPath.path.size(); ++i)
  {
    std::cout << bestPath.path[i] << " ";
  }
  std::cout << "\nCost: " << bestPath.cost << std::endl;
  std::array<std::array<int, 10>, 10> shiftedMatrix = shift_adjacency_matrix(adjMatrix);
  std::cout << "Shifted Adjacency Matrix:\n";
  for (size_t i = 0; i < shiftedMatrix.size(); ++i)
  {
    for (size_t j = 0; j < shiftedMatrix[i].size(); ++j)
    {
      std::cout << shiftedMatrix[i][j] << " ";
    }
    std::cout << "\n";
  }
  bestPath = find_best_path(shiftedMatrix, path);
  std::cout << "Best path after shifting: ";
  for (size_t i = 0; i < bestPath.path.size(); ++i)
  {
    std::cout << bestPath.path[i] << " ";
  }
  std::cout << "\nCost after shifting: " << bestPath.cost << std::endl;
  return 0;
}
