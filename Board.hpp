#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <ctime>
#include "Parts.hpp"


using namespace sf;
using namespace std;

/* Board class */

class Board : public Drawable {
    private:
        Parts parts;

        int indNewPart, indColorNewPart;
        Color newPartColor;
        int indX, indY;
        int board[20][10];
        RectangleShape boardShapes[20][10];
    public:
    Board();
    bool InstallParts();
    void UpdateBoardColors();
    virtual void draw(RenderTarget&, RenderStates) const;
};