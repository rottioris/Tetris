#include "Board.hpp"

Board::Board() {
    srand((unsigned)time(NULL));
    std::memset(board, 0, sizeof(board));
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) { // for aninado
            boardShapes[i][j].setSize(Vector2f(20, 20));
            boardShapes[i][j].setPosition(j * 20, i * 20);
            boardShapes[i][j].setFillColor(Color(50, 50, 50));
            boardShapes[i][j].setOutlineThickness(2);
            boardShapes[i][j].setOutlineColor(Color(100, 100, 100));
        }
    }
}

bool Board::InstallParts() {
    indNewPart = rand() % 7;

    vector<vector<bool>> Part = parts.ConsultPart(indNewPart);

    int sz = (int)Part.size();
    indY=0;
    indX=5-sz/2;

    // Verificar colisiones
    for(int i = 0; i < sz; i++) {
        for(int j = 0; j < sz; j++) {
            if(Part[i][j]) {
                int bi = i;
                int bj = j + indX;
                if(bi < 0 || bi >= 20 || bj < 0 || bj >= 10) return false;
                if(board[bi][bj] > 0) return false;
            }
        }
    }

    // Colocar pieza
    for(int i = 0; i < sz; i++) {
        for(int j = 0; j < sz; j++) {
            if(Part[i][j]) {
                board[i][j + indX] = -1;
            }
        }
    }

    int color = 1 + rand() % 7;
    indColorNewPart = color;
    switch (color) {
        case 1:
            newPartColor = Color(255, 166, 0);
            break;
        case 2:
            newPartColor = Color(245, 152, 245);
            break;
        case 3:
            newPartColor = Color(51, 204, 153);
            break;
        case 4:
            newPartColor = Color(255, 110, 110);
            break;
        case 5:
            newPartColor = Color(255, 204, 77);
            break;
        case 6:
            newPartColor = Color(166, 166, 205);
            break;
        case 7:
            newPartColor = Color(138, 194, 247);
            break;
    }

    return true;
}

void Board::UpdateBoardColors() {
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
            switch(board[i][j]) {
                case 0:
                    boardShapes[i][j].setFillColor(Color(50, 50, 50));
                    break;
                case 1:
                    boardShapes[i][j].setFillColor(Color(255, 166, 0));
                    break;
                case 2:
                    boardShapes[i][j].setFillColor(Color(245, 152, 245));
                    break;
                case 3:
                    boardShapes[i][j].setFillColor(Color(51, 204, 153));
                    break;
                case 4:
                    boardShapes[i][j].setFillColor(Color(255, 110, 110));
                    break;
                case 5:
                    boardShapes[i][j].setFillColor(Color(255, 204, 77));
                    break;
                case 6:
                    boardShapes[i][j].setFillColor(Color(166, 166, 205));
                    break;
                case 7:
                    boardShapes[i][j].setFillColor(Color(138, 194, 247));
                    break;
                    default:
                    boardShapes[i][j].setFillColor(newPartColor);
                    break;
            }
        }
    }
}

bool Board::UpdateBoard(){
    if (++timer < limitTimer)
        return false;
    timer = 0;

    int active = 0;
    int movable = 0;
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
            if(board[i][j] == -1) {
                active++;
                if(i + 1 < 20 && board[i + 1][j] <= 0) movable++;
            }
        }
    }

    if(active == 0) return false;

    if(active == movable) {
        for(int i = 18; i >= 0; i--) {
            for(int j = 0; j < 10; j++) {
                if(board[i][j] == -1) {
                    board[i][j] = 0;
                    board[i + 1][j] = -1;
                }
            }
        }
        return false;
    }

    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
            if(board[i][j] == -1) board[i][j] = indColorNewPart;
        }
    }
    return true;
}

void Board::draw(RenderTarget &rt, RenderStates rs) const {
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
           rt.draw(boardShapes[i][j], rs);
        }
    }
}
