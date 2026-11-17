#include "Board.hpp"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <utility>

namespace {
constexpr int Empty = 0;
constexpr int Active = -1;
}

sf::Color Board::colorForCell(int value) {
    switch (value) {
        case 0:
            return sf::Color(50, 50, 50);
        case 1:
            return sf::Color(255, 166, 0);
        case 2:
            return sf::Color(245, 152, 245);
        case 3:
            return sf::Color(51, 204, 153);
        case 4:
            return sf::Color(255, 110, 110);
        case 5:
            return sf::Color(255, 204, 77);
        case 6:
            return sf::Color(166, 166, 205);
        case 7:
            return sf::Color(138, 194, 247);
        default:
            return sf::Color();
    }
}

Board::Board() {
    srand(static_cast<unsigned>(time(nullptr)));
    std::memset(board, Empty, sizeof(board));

    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            boardShapes[i][j].setSize(sf::Vector2f(CellSize, CellSize));
            boardShapes[i][j].setPosition(j * CellSize, i * CellSize);
            boardShapes[i][j].setFillColor(colorForCell(Empty));
            boardShapes[i][j].setOutlineThickness(2);
            boardShapes[i][j].setOutlineColor(sf::Color(100, 100, 100));
        }
    }
}

bool Board::InstallParts() {
    for (int attempt = 0; attempt < 7; ++attempt) {
        const int candidate = (rand() + attempt) % 7;
        const auto part = parts.ConsultPart(candidate);
        const int size = static_cast<int>(part.size());
        const int x = Cols / 2 - size / 2;

        bool fits = true;
        for (int i = 0; i < size && fits; ++i) {
            for (int j = 0; j < size; ++j) {
                if (part[i][j] && board[i][j + x] > Empty) {
                    fits = false;
                    break;
                }
            }
        }
        if (!fits) {
            continue;
        }

        indNewPart = candidate;
        indY = 0;
        indX = x;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (part[i][j]) {
                    board[i][j + indX] = Active;
                }
            }
        }

        indColorNewPart = 1 + rand() % 7;
        newPartColor = colorForCell(indColorNewPart);
        return true;
    }
    return false;
}

void Board::UpdateBoardColors() {
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            const int cell = board[i][j];
            boardShapes[i][j].setFillColor(cell == Active ? newPartColor : colorForCell(cell));
        }
    }
}

bool Board::UpdateBoard() {
    int active = 0;
    int movable = 0;
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                ++active;
                if (i + 1 < Rows && board[i + 1][j] <= Empty) {
                    ++movable;
                }
            }
        }
    }

    if (active == 0) {
        return false;
    }

    // Apoyada en el suelo o en otra pieza: se fija de inmediato.
    if (active != movable) {
        for (int i = 0; i < Rows; ++i) {
            for (int j = 0; j < Cols; ++j) {
                if (board[i][j] == Active) {
                    board[i][j] = indColorNewPart;
                }
            }
        }
        timer = 0;
        return true;
    }

    if (++timer < limitTimer) {
        return false;
    }
    timer = 0;

    for (int i = Rows - 2; i >= 0; --i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                board[i][j] = Empty;
                board[i + 1][j] = Active;
            }
        }
    }
    return false;
}

void Board::UpdateLimitTimer(int limit) {
    limitTimer = limit;
}

void Board::RotatePart() {
    std::vector<std::pair<int, int>> original;
    int minR = Rows;
    int minC = Cols;
    bool landed = false;

    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                original.emplace_back(i, j);
                minR = std::min(minR, i);
                minC = std::min(minC, j);
                if (i + 1 >= Rows || board[i + 1][j] > Empty) {
                    landed = true;
                }
            }
        }
    }
    if (original.empty() || landed) {
        return;
    }

    parts.RotatePart(indNewPart);
    const auto part = parts.ConsultPart(indNewPart);
    const int size = static_cast<int>(part.size());

    int partMinR = size;
    int partMinC = size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (part[i][j]) {
                partMinR = std::min(partMinR, i);
                partMinC = std::min(partMinC, j);
            }
        }
    }

    // Desplazamientos permitidos para rotar cerca de paredes o del suelo.
    static const int rowKicks[] = {0, -1, -2, -3};
    static const int colKicks[] = {0, -1, 1, -2, 2, -3, 3};

    for (int dr : rowKicks) {
        for (int dc : colKicks) {
            std::vector<std::pair<int, int>> rotated;
            bool valid = true;

            for (int i = 0; i < size && valid; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (!part[i][j]) {
                        continue;
                    }
                    const int r = minR + (i - partMinR) + dr;
                    const int c = minC + (j - partMinC) + dc;
                    if (r < 0 || r >= Rows || c < 0 || c >= Cols || board[r][c] > Empty) {
                        valid = false;
                        break;
                    }
                    rotated.emplace_back(r, c);
                }
            }
            if (!valid) {
                continue;
            }

            for (const auto& cell : original) {
                board[cell.first][cell.second] = Empty;
            }
            for (const auto& cell : rotated) {
                board[cell.first][cell.second] = Active;
            }
            indX += dc;
            indY += dr;
            return;
        }
    }

    parts.DerotatePart(indNewPart);
}

void Board::Rigth() {
    int active = 0;
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                ++active;
                if (j + 1 >= Cols || board[i][j + 1] > Empty) {
                    return;
                }
            }
        }
    }
    if (active == 0) {
        return;
    }

    ++indX;
    for (int i = 0; i < Rows; ++i) {
        for (int j = Cols - 1; j >= 0; --j) {
            if (board[i][j] == Active) {
                board[i][j] = Empty;
                board[i][j + 1] = Active;
            }
        }
    }
}

void Board::Left() {
    int active = 0;
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                ++active;
                if (j - 1 < 0 || board[i][j - 1] > Empty) {
                    return;
                }
            }
        }
    }
    if (active == 0) {
        return;
    }

    --indX;
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            if (board[i][j] == Active) {
                board[i][j] = Empty;
                board[i][j - 1] = Active;
            }
        }
    }
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < Rows; ++i) {
        for (int j = 0; j < Cols; ++j) {
            target.draw(boardShapes[i][j], states);
        }
    }
}
