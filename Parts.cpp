#include "Parts.hpp"

Parts::Parts() {
    parts = {
        {{1, 1},
         {1, 1}},

        {{0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 0, 1, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 1, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 1, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 1, 0}}};
}

std::vector<std::vector<bool>> Parts::ConsultPart(int index) {
    return parts.at(index);
}

void Parts::RotatePart(int index) {
    const auto original = parts[index];
    const int size = static_cast<int>(parts[index].size());

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            parts[index][i][j] = original[size - 1 - j][i];
        }
    }
}

void Parts::DerotatePart(int index) {
    const auto original = parts[index];
    const int size = static_cast<int>(parts[index].size());

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            parts[index][i][j] = original[j][size - 1 - i];
        }
    }
}
