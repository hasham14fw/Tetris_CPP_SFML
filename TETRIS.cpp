#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <ctime>
#include <cstdlib>
#include <algorithm>
const int WIDTH = 10;
const int HEIGHT = 20;
const int BLOCK_SIZE = 30;
const float BASE_DROP_INTERVAL = 0.5f;

const std::array<std::array<std::array<int, 4>, 4>, 28> TETROMINOS = { {
        // I-Tetromino
         {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
         {{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}},
         {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},  
        {{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}},  

         // O-Tetromino 
     {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}, 
     {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
     {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
      {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},

      // T-Tetromino
     {{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}},
     {{{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}},   
     {{{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}},   
     {{{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0} }},  

     // S-Tetromino
     {{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}}}, 
     {{{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}},   
     {{ {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}}},   
     {{ {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}},  

     // Z-Tetromino
     {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}}, 
      {{{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}},  
     {{ {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}},   
     {{ {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}},  

     // L-Tetromino
     {{{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}}, 
     {{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}}},   
     {{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}},   
     {{{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}}},  

     // J-Tetromino
     {{{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}}}, 
     {{{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}}},   
    {{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}}},   
      {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}}   
     } };



const std::array<sf::Color, 7> TETROMINO_COLORS = {
    sf::Color::Cyan, sf::Color::Magenta, sf::Color::Green,
    sf::Color::Red, sf::Color::Yellow, sf::Color::Blue, sf::Color::White
};

class Tetris {
public:
    Tetris()
        : board(HEIGHT, std::vector<int>(WIDTH, 0)),
        currentTetromino(0),
        currentX(0),
        currentY(0),
        score(0),
        level(1),
        currentRotation(0),
        dropTime(0),
        dropInterval(BASE_DROP_INTERVAL),
        moveTime(0),
        moveDelay(0.15f),
        gameOver(false) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        font.loadFromFile("arial.ttf");
        spawnTetromino();
    }

    void spawnTetromino() {
        currentTetromino = rand() % TETROMINOS.size();
        currentX = WIDTH / 2 - 2;
        currentY = 0;
        if (collision()) gameOver = true;
    }

    void reset() {
        board.assign(HEIGHT, std::vector<int>(WIDTH, 0));
        score = 0;
        level = 1;
        dropTime = 0;
        gameOver = false;
        spawnTetromino();
    }

    bool collision() {
        const auto& tetromino = TETROMINOS[currentTetromino];
        for (int y = 0; y < tetromino.size(); ++y) {
            for (int x = 0; x < tetromino[y].size(); ++x) {
                if (tetromino[y][x] != 0) {
                    int boardX = currentX + x;
                    int boardY = currentY + y;
                    if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT || (boardY >= 0 && board[boardY][boardX] != 0)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void merge() {
        const auto& tetromino = TETROMINOS[currentTetromino];
        for (int y = 0; y < tetromino.size(); ++y) {
            for (int x = 0; x < tetromino[y].size(); ++x) {
                if (tetromino[y][x] != 0) {
                    board[currentY + y][currentX + x] = currentTetromino % 4 + 1;
                }
            }
        }
    }

    void clearLines() {
        int linesCleared = 0;
        for (int y = HEIGHT - 1; y >= 0; --y) {
            if (std::all_of(board[y].begin(), board[y].end(), [](int v) { return v != 0; })) {
                board.erase(board.begin() + y);
                board.insert(board.begin(), std::vector<int>(WIDTH, 0));
                linesCleared++;
            }
        }
        if (linesCleared > 0) {
            score += linesCleared * 10;
            level = score / 50 + 1;
        }
    }

    void drop() {
        currentY++;
        if (collision()) {
            currentY--;
            merge();
            clearLines();
            spawnTetromino();
        }
    }
    void rotate() {
        int previousTetromino = currentTetromino;
        if ((currentTetromino + 1) % 4 == 0) {
            currentTetromino = currentTetromino - 3;
        }
        else {
            currentTetromino = currentTetromino + 1;
        }
        if (collision()) {
            currentTetromino = previousTetromino;

        }
    }
    void handleInput(float deltaTime) {
        if (gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                reset();
            }
            return;
        }

        moveTime += deltaTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && moveTime >= moveDelay) {
            currentX--;
            if (collision()) currentX++;
            moveTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && moveTime >= moveDelay) {
            currentX++;
            if (collision()) currentX--;
            moveTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            dropInterval = BASE_DROP_INTERVAL / 10;
        }
        else {
            dropInterval = BASE_DROP_INTERVAL;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (!rotateKeyPressed) { 
                rotate();
                rotateKeyPressed = true; 
            }
        }
        else {
            rotateKeyPressed = false;
        }
    }

    void update(float deltaTime) {
        if (gameOver) return;

        dropTime += deltaTime;
        if (dropTime >= dropInterval - (level - 1) * 0.05f)
        {
            drop();
            dropTime = 0;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.clear();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (board[y][x] != 0) {
                    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
                    block.setFillColor(TETROMINO_COLORS[board[y][x] - 1]);
                    block.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
                    window.draw(block);
                }
            }
        }

        const auto& tetromino = TETROMINOS[currentTetromino];
        for (int y = 0; y < tetromino.size(); ++y) {
            for (int x = 0; x < tetromino[y].size(); ++x) {
                if (tetromino[y][x] != 0) {
                    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));

                   // block.setFillColor(TETROMINO_COLORS[currentTetromino%7]);
                    block.setPosition((currentX + x) * BLOCK_SIZE, (currentY + y) * BLOCK_SIZE);
                    window.draw(block);
                }
            }
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score) + "\nLevel: " + std::to_string(level));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);

        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER\nPress R\nto Restart");
            gameOverText.setCharacterSize(36);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setStyle(sf::Text::Bold);
            gameOverText.setPosition(WIDTH * BLOCK_SIZE / 4, HEIGHT * BLOCK_SIZE / 3);
            window.draw(gameOverText);
        }

        window.display();
    }

private:
    std::vector<std::vector<int>> board;
    int currentTetromino, currentX, currentY, score, level, currentRotation;
    float dropTime, dropInterval, moveTime, moveDelay;
    bool gameOver;
    bool rotateKeyPressed = false;
    sf::Font font;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE), "Tetris");
    Tetris game;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        game.handleInput(deltaTime);
        game.update(deltaTime);
        game.draw(window);
    }

    return 0;
}

