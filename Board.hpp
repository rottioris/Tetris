#pragma once

#include <SFML/Graphics.hpp>

#include "Parts.hpp"

class Board : public sf::Drawable {
private:
    static constexpr int Rows = 20;
    static constexpr int Cols = 10;
    static constexpr int CellSize = 20;

    Parts parts;

    int indNewPart = 0;
    int indColorNewPart = 0;
    sf::Color newPartColor;
    int indX = 0;
    int indY = 0;
    int board[Rows][Cols];
    int timer = 0;
    int limitTimer = 30;
    sf::RectangleShape boardShapes[Rows][Cols];

    static sf::Color colorForCell(int value);

public:
    Board();

    bool InstallParts();
    bool UpdateBoard();
    void UpdateBoardColors();
    void UpdateLimitTimer(int limit);
    void Rigth();
    void Left();
    void RotatePart();
    int CheckLines();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
