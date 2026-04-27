#include "sprites.h"
#include "Menu.h"
#include <iostream>


struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }
    void move() {
        x += dx;
        y += dy;
    }
};


bool GameSprites::loadAllTextures() {

 
    if (!tTile.loadFromFile("images/tiles.png") ||
        !tGameover.loadFromFile("images/gameover.png") || 
        !tLeaderboardBackground.loadFromFile("images/leaderboardBG.png")||
        !tEnemy.loadFromFile("images/enemy.png") ||
        !tMainBackground.loadFromFile("images/mainBackground.png") ||
        !mFont.loadFromFile("Fonts/Orbitron.ttf")) {
        std::cerr << "Failed to load textures or font\n";
        return false;
    }
    return true;
}

void GameSprites::initializeSprites(int a) {
    
    sTile.setTexture(tTile);
    sGameover.setTexture(tGameover);
    sEnemy.setTexture(tEnemy);

    sGameover.setPosition(100, 70);
    sEnemy.setOrigin(20, 20);
    sLeaderboardBackground.setTexture(tLeaderboardBackground);
    if (a == 1) {
        setLoginBackground();
    }
    else {
        setMainBackground();
    }
}

void GameSprites::setLoginBackground() {
    sCurrentBackground.setTexture(tLoginBackground);
}

void GameSprites::setMainBackground() {
    sCurrentBackground.setTexture(tMainBackground);
}


void drawMainMenu(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& background) {
    if (background.getTexture()->getSize().x > 0)
        window.draw(background);
    else
        window.clear(sf::Color(30, 30, 50));

    sf::Text title("XONIX GAME", font, 50);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setOutlineColor(sf::Color(139, 69, 19));
    title.setOutlineThickness(4);
    title.setStyle(sf::Text::Bold);
    title.setPosition(200, 30);
    window.draw(title);
}

void drawGame(sf::RenderWindow& window, sf::Sprite& tileSprite, sf::Sprite& enemySprite,
    int grid[25][40], int x, int y, const Enemy enemies[], int enemyCount, float rotationAngle) {
    const int ts = 18; 

    
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 40; j++) {
            if (grid[i][j] == 0) continue;
            tileSprite.setTextureRect((grid[i][j] == 1) ? sf::IntRect(0, 0, ts, ts) : sf::IntRect(54, 0, ts, ts));
            tileSprite.setPosition(j * ts, i * ts);
            window.draw(tileSprite);
        }
    }

    tileSprite.setTextureRect(sf::IntRect(36, 0, ts, ts));
    tileSprite.setPosition(x * ts, y * ts);
    window.draw(tileSprite);

    
    for (int i = 0; i < enemyCount; i++) {
        enemySprite.setPosition(enemies[i].x, enemies[i].y);
        enemySprite.rotate(rotationAngle);
        window.draw(enemySprite);
    }
}

void drawScore(sf::RenderWindow& window, const sf::Font& font, int score) {
    sf::Text scoreText("Score: " + std::to_string(score), font, 20);
    scoreText.setFillColor(sf::Color(255, 215, 0));
    scoreText.setOutlineColor(sf::Color(139, 69, 19));
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(0, 0);
    window.draw(scoreText);
}

void drawBonusText(sf::RenderWindow& window, const sf::Font& font, const std::string& text) {
    sf::Text bonus(text, font, 28);
    bonus.setFillColor(sf::Color::Green);
    bonus.setOutlineColor(sf::Color::Black);
    bonus.setOutlineThickness(2);
    bonus.setPosition(200, 10);
    window.draw(bonus);
}

void drawGameOver(sf::RenderWindow& window, const sf::Sprite& gameOverSprite, const sf::Font& font, int score) {
    window.draw(gameOverSprite);

   
    sf::RectangleShape scoreBox(sf::Vector2f(300, 80));
    scoreBox.setFillColor(sf::Color(70, 70, 70, 200));
    scoreBox.setOutlineThickness(2);
    scoreBox.setOutlineColor(sf::Color::Blue);
    scoreBox.setPosition(210, 250);
    scoreBox.setSize(sf::Vector2f(300, 50));
    window.draw(scoreBox);

   
    sf::Text endText("Final Score: " + std::to_string(score), font, 30);
    endText.setFillColor(sf::Color::Yellow);
    endText.setPosition(220, 257);
    window.draw(endText);

   
    sf::Text restartText("Press ESC for Menu or R to Restart", font, 20);
    restartText.setFillColor(sf::Color::Red);
    restartText.setOutlineColor(sf::Color::White);
    restartText.setOutlineThickness(2);
    restartText.setPosition(130, 320);
    window.draw(restartText);
}

void drawPauseMenu(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text pauseText("PAUSE Menu", font, 40);
    pauseText.setFillColor(sf::Color::Green);
    pauseText.setOutlineColor(sf::Color::Blue);
    pauseText.setStyle(sf::Text::Bold);
    pauseText.setOutlineThickness(3);
    pauseText.setPosition(240, 30);
    window.draw(pauseText);
}

void drawPowerUpIndicator(sf::RenderWindow& window, const sf::Font& font, int powerUpCount) {
    sf::Text powerUpText("Power-Ups: " + std::to_string(powerUpCount), font, 20);
    powerUpText.setFillColor(sf::Color(255, 215, 0));
    powerUpText.setOutlineColor(sf::Color(139, 69, 19));
    powerUpText.setOutlineThickness(2);
    powerUpText.setPosition(400, 0);
    window.draw(powerUpText);
}
