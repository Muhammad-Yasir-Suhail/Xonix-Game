#ifndef SPRITES_H
#define SPRITES_H

#include <SFML/Graphics.hpp>
#include <string>

struct Enemy;

class GameSprites {
public:
    
    sf::Texture tTile, tGameover, tEnemy;
    sf::Texture tMainBackground;  
    sf::Texture tLoginBackground; 
    sf::Texture tLeaderboardBackground;
    sf::Font mFont;

    
    sf::Sprite sTile, sGameover, sEnemy;
    sf::Sprite sCurrentBackground; 
    sf::Sprite sLeaderboardBackground;
    
    bool loadAllTextures();
    void initializeSprites(int); 
    void setLoginBackground();
    void setMainBackground();
};


void drawMainMenu(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& background);
void drawGame(sf::RenderWindow& window, sf::Sprite& tileSprite, sf::Sprite& enemySprite,
    int grid[25][40], int x, int y, const Enemy enemies[], int enemyCount, float rotationAngle = 0);
void drawScore(sf::RenderWindow& window, const sf::Font& font, int score);
void drawBonusText(sf::RenderWindow& window, const sf::Font& font, const std::string& text);
void drawGameOver(sf::RenderWindow& window, const sf::Sprite& gameOverSprite, const sf::Font& font, int score);
void drawPauseMenu(sf::RenderWindow& window, const sf::Font& font);
void drawPowerUpIndicator(sf::RenderWindow& window, const sf::Font& font, int powerUpCount);

#endif