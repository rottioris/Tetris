#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.hpp"

using namespace std;
using namespace sf;
/* Loop */
int main(){
    Board board;
    RenderWindow window(VideoMode(400, 400), "Tetoris");
    window.setFramerateLimit(60);
    board.InstallParts();
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }
        if(board.UpdateBoard() && !board.InstallParts())
            window.close();
        board.UpdateBoardColors();
        window.clear(Color(20,20,20));
        window.draw(board);
        window.display();
    }
    return 0;
}