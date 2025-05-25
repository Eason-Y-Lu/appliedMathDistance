#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>
#include <vector>

int generate_random_int(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> pool(min, max);
  return pool(gen);
}

int test_alg() {
  const int row = 10;
  const int col = 10;
  std::vector<std::vector<int>> adj_materix(row, std::vector<int>(col));
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      if (i == j) {
        adj_materix[i][j] = 0;
      } else {
        adj_materix[i][j] = generate_random_int(-1, 1048576);
      }
    }
  }
  // // std::cout << "Adjacency Matrix:" << std::endl;
  for (const auto &row : adj_materix) {
    for (const auto &elem : row) {
      // // std::cout << elem << " ";
    }
    // // std::cout << std::endl;
  }
  std::vector<int> travel = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> best_travel = travel;
  int best_cost = std::numeric_limits<int>::max();
  while (std::next_permutation(travel.begin() + 1, travel.end() - 1)) {
    int cost = 0;
    for (int i = 0; i < travel.size() - 1; ++i) {
      if (adj_materix[travel[i]][travel[i + 1]] == -1) {
        cost = std::numeric_limits<int>::max();
        break;
      }
      cost += adj_materix[travel[i]][travel[i + 1]];
    }
    if (cost < best_cost) {
      best_cost = cost;
      best_travel = travel;
    }
  }
  // std::cout << "Best travel path: ";
  for (const auto &elem : best_travel) {
    // std::cout << elem << " ";
  }
  // std::cout << std::endl;
  // std::cout << "Best travel cost: " << best_cost << std::endl;
  int old_best_cost = best_cost;
  std::vector<int> old_best_travel = best_travel;
  int num_changes = (row * col) / 2;
  std::vector<std::vector<int>> adj_materix_old = adj_materix;
  std::vector<std::array<int, 2>> changes(num_changes);
  for (int i = 0; i < num_changes; ++i) {
    int x = generate_random_int(0, row - 1);
    int y = generate_random_int(0, col - 1);
    while (x == y) {
      x = generate_random_int(0, row - 1);
      y = generate_random_int(0, col - 1);
    }
    adj_materix[x][y] = generate_random_int(-1, 1048576);
    changes[i] = {x, y};
  }
  // std::cout << "Adjacency Matrix after random changes:" << std::endl;
  for (const auto &row : adj_materix) {
    for (const auto &elem : row) {
      // std::cout << elem << " ";
    }
    // std::cout << std::endl;
  }
  while (std::next_permutation(travel.begin() + 1, travel.end() - 1)) {
    int cost = 0;
    for (int i = 0; i < travel.size() - 1; ++i) {
      if (adj_materix[travel[i]][travel[i + 1]] == -1) {
        cost = std::numeric_limits<int>::max();
        break;
      }
      cost += adj_materix[travel[i]][travel[i + 1]];
    }
    if (cost < best_cost) {
      best_cost = cost;
      best_travel = travel;
    }
  }
  // std::cout << "Best travel path: ";
  for (const auto &elem : best_travel) {
    // std::cout << elem << " ";
  }
  // std::cout << std::endl;
  // std::cout << "Best travel cost: " << best_cost << std::endl;
  // check if the best path has changed

  // if old_best_trave = best_travel -> return 0
  // if old_best_trave != best_travel ->
  // if old_best_trave is not possible in new adj_materix -> return -1
  // if old_best_trave is possible in new adj_materix return the difference
  // between the old path on the new adj_materix and the new best
  if (old_best_travel == best_travel) {
    return 0;
  } else {
    int old_best_cost = 0;
    for (int i = 0; i < old_best_travel.size() - 1; ++i) {
      if (adj_materix[old_best_travel[i]][old_best_travel[i + 1]] == -1) {
        return -1;
      }
      old_best_cost += adj_materix[old_best_travel[i]][old_best_travel[i + 1]];
    }
    if (old_best_cost == best_cost) {
      return 0;
    } else {
      return old_best_cost / best_cost - 1 * 100;
    }
  }
}

int main() {
  std::vector<int> result;
  for (int i = 0; i < 10000; i++) {
    result.push_back(test_alg());
  }
  // parse
  int path_unchanged = 0;
  int impossible_path = 0;
  int different_less_then_10 = 0;
  int different_greater_then_10 =
      10000 - path_unchanged - impossible_path - different_less_then_10;
  std::cout << "Path unchanged: " << path_unchanged << std::endl;
  std::cout << "Impossible path: " << impossible_path << std::endl;
  std::cout << "Different less than 10: " << different_less_then_10
            << std::endl;
  std::cout << "Different greater than 10: " << different_greater_then_10
            << std::endl;
  std::cout << "Total: "
            << path_unchanged + impossible_path + different_less_then_10 +
                   different_greater_then_10
            << std::endl;
  return 0;
}