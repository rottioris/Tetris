#include <SFML/Graphics.hpp>

#include <iostream>

#include "Board.hpp"

namespace {
constexpr int NormalFall = 30;
constexpr int FastFall = 5;
}

int main() {
    Board board;
    sf::RenderWindow window(sf::VideoMode(400, 400), "Tetoris");
    window.setFramerateLimit(60);
    board.InstallParts();

    bool upPressed = false;
    bool rightPressed = false;
    bool leftPressed = false;
    int score = 0;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const bool upDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        if (upDown && !upPressed) {
            board.RotatePart();
        }
        upPressed = upDown;

        board.UpdateLimitTimer(
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? FastFall : NormalFall);

        const bool rightDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        if (rightDown && !rightPressed) {
            board.Rigth();
        }
        rightPressed = rightDown;

        const bool leftDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        if (leftDown && !leftPressed) {
            board.Left();
        }
        leftPressed = leftDown;

        if (board.UpdateBoard() && !board.InstallParts()) {
            std::cout << "Game Over!" << std::endl;
            window.close();
        }

        board.UpdateBoardColors();
        int newScore = board.CheckLines() * 5;
        if (newScore > 0) {
            score += newScore;
            std::cout << score << std::endl;
        }
        window.clear(sf::Color(20, 20, 20));
        window.draw(board);
        window.display();
    }

    return 0;
}
