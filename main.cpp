#include <oneapi/tbb.h>
#include <vector>
#include <array>
#include <limits>
#include <iostream>
#include <fstream>

int generate_random_int(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pool(min, max);
    return pool(gen);
}

struct best_path {
    std::vector<int> path;
    int cost;
};

best_path find_best_path(const std::array<std::array<int, 10>, 10> adjMaterix, std::vector<int> path) {
    std::vector<int> bestPath;
    int bestCost = std::numeric_limits<int>::max();
    while (std::next_permutation(path.begin() + 1, path.end() - 1)) {
        int cost = 0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            if (adjMaterix[path[i]][path[i + 1]] == -1) {
                cost = std::numeric_limits<int>::max();
                break;
            } else {
                cost += adjMaterix[path[i]][path[i + 1]];
            }
        }
        if (cost < bestCost) {
            bestCost = cost;
            bestPath = path;
        }
    }
    return {bestPath, bestCost};
}

std::array<std::array<int, 10>, 10> generate_adjacency_matrix() {
    std::array<std::array<int, 10>, 10> adjMatrix{};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < i; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;
            } else {
                int distance = generate_random_int(-1, 255);
                while (distance == 0) {
                    distance = generate_random_int(-1, 255);
                }
                adjMatrix[i][j] = distance;
                adjMatrix[j][i] = distance;
            }
        }
    }
    adjMatrix[0][0] = 0;
    return adjMatrix;
}

std::array<std::array<int, 10>, 10> shift_adjacency_matrix(const std::array<std::array<int, 10>, 10> &adjMatrix) {
    std::array<std::array<int, 10>, 10> shiftedMatrix = adjMatrix;
    int numChanges = 22;
    for (int i = 0; i < numChanges; ++i) {
        int row = generate_random_int(0, 9);
        int col = generate_random_int(0, 9);
        while (row == col) {
            row = generate_random_int(0, 9);
            col = generate_random_int(0, 9);
        }
        shiftedMatrix[row][col] = generate_random_int(-1, 255);
        shiftedMatrix[col][row] = shiftedMatrix[row][col];
    }
    return shiftedMatrix;
}

int calculate_path_cost(const std::array<std::array<int, 10>, 10> &adjMatrix, const std::vector<int> &path) {
    int cost = 0;
    for (size_t i = 0; i < path.size() - 1; i++) {
        if (adjMatrix[path[i]][path[i + 1]] == -1) {
            return std::numeric_limits<int>::max();
        }
        cost += adjMatrix[path[i]][path[i + 1]];
    }
    return cost;
}

// best_path find_alt_path(const std::array<std::array<int, 10>, 10> adjMatrix, std::vector<int> path){
// Take in a previously found best path and find the next best path
// Detect if the old path is still complete with no -1 values
// If it is, return the old path
// If not check where did the path fail
// get all permutations of the path with 2 adjacent nodes swapped around the failed node

//}

int main() {
    oneapi::tbb::concurrent_vector<int> results;
    oneapi::tbb::parallel_for(0, 10000, [&results](int i) {
        const auto adjMatrix = generate_adjacency_matrix();
        const std::vector<int> path = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        best_path bestPath = find_best_path(adjMatrix, path);
        const auto shiftedMatrix = shift_adjacency_matrix(adjMatrix);
        best_path altPath = find_best_path(shiftedMatrix, path);
        int old_path_on_new_dist_cost = calculate_path_cost(shiftedMatrix, bestPath.path);
        if (old_path_on_new_dist_cost == std::numeric_limits<int>::max()) {
            results.push_back(std::numeric_limits<int>::max());
        } else if (old_path_on_new_dist_cost == altPath.cost) {
            results.push_back(0);
        } else {
            double precentage_difference = (old_path_on_new_dist_cost - altPath.cost) / static_cast<double>(altPath.
                                               cost);
            if (precentage_difference < 0) {
                std::printf("Old matrix cost: %d, New matrix cost: %d, Percentage difference: %f\n",
                            old_path_on_new_dist_cost, altPath.cost, precentage_difference);
                std::printf("Old path: ");
                for (const auto &node: bestPath.path) {
                    std::printf("%d ", node);
                }
                std::printf("\n");
                std::printf("Shifted matrix: \n");
                for (const auto &row: shiftedMatrix) {
                    for (const auto &value: row) {
                        std::printf("%d, ", value);
                    }
                    std::printf("\n");
                }
            }
            results.push_back(static_cast<int>(precentage_difference * 100));
        }
    });
    int old_path_still_best = 0;
    int old_path_in_10 = 0;
    int old_path_in_20 = 0;
    int old_path_larger_than_20 = 0;
    int old_path_not_valid = 0;
    for (const auto &result: results) {
        if (result == 0) {
            old_path_still_best++;
        } else if (result == std::numeric_limits<int>::max()) {
            old_path_not_valid++;
        } else if (result < 10) {
            old_path_in_10++;
        } else if (result < 20) {
            old_path_in_20++;
        } else {
            old_path_larger_than_20++;
        }
    }
    tbb::concurrent_unordered_map<int, int> frequency;
    const int result_length = results.size();
    tbb::parallel_for(0, result_length, [&results, &frequency](int i) {
        frequency[results[i]]++;
    });
    std::ofstream resultsFile;
    resultsFile.open("results.csv");
    resultsFile << "Result,Frequency\n";
    for (const auto &pair: frequency) {
        resultsFile << pair.first << "," << pair.second << "\n";
    }
    resultsFile.close();
    std::cout << "Old path still best: " << old_path_still_best << "\n";
    std::cout << "Old path not valid: " << old_path_not_valid << "\n";
    std::cout << "Old path in 10: " << old_path_in_10 << "\n";
    std::cout << "Old path in 20: " << old_path_in_20 << "\n";
    std::cout << "Old path larger than 20: " << old_path_larger_than_20 << "\n";
    std::cout << "Total: " << results.size() << "\n";
    std::cout << "Percentage of old path still best: " << (static_cast<double>(old_path_still_best) / results.size()) *
            100 << "%\n";
    std::cout << "Percentage of old path not valid: " << (static_cast<double>(old_path_not_valid) / results.size()) *
            100 << "%\n";
    std::cout << "Percentage of old path in 10: " << (static_cast<double>(old_path_in_10) / results.size()) * 100 <<
            "%\n";
    std::cout << "Percentage of old path in 20: " << (static_cast<double>(old_path_in_20) / results.size()) * 100 <<
            "%\n";
    std::cout << "Percentage of old path larger than 20: " << (
        static_cast<double>(old_path_larger_than_20) / results.size()) * 100 << "%\n";
    return 0;
}
