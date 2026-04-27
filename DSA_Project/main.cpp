#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include "Player.h"
#include "GameState.h"
#include "Menu.h"
#include "Header.h"
#include "sprites.h"
#include "Leaderboard.h"
#include "Inventory.h"
#include "Utility.h"
using namespace std;
using namespace sf;
const int M = 25;
const int N = 40;
int ts = 18;
int grid[M][N] = { 0 };
struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }
    void move() {
        x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
    }
};
void drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}
void resetGame(int grid[M][N], int& x, int& y, int& dx, int& dy, bool& gameState, Enemy enemies[], int eCount, int& score,
    int& powerUpCount, bool& powerUpActive, float& powerUpTimer, int& lastPowerUpScore) {
    score = 0;
    powerUpCount = 0;
    powerUpActive = false;
    powerUpTimer = 0.0f;
    lastPowerUpScore = 0;
    for (int i = 1; i < M - 1; i++)
        for (int j = 1; j < N - 1; j++)
            grid[i][j] = 0;

    x = 10; y = 0;
    dx = dy = 0;
    gameState = true;

    for (int i = 0; i < eCount; i++)
        enemies[i] = Enemy();
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);
    Vector2f windowSize(window.getSize().x, window.getSize().y);

    GameSprites sp;
    if (!sp.loadAllTextures()) return -1;
    sp.initializeSprites(2);

    GameState currentState = START_SCREEN;
    Enemy enemies[10];
    int enemyCount = 4;
    int x = 0, y = 0, dx = 0, dy = 0;
    bool Game = true;
    float timer = 0, delay = 0.07;
    Clock clock;

    int score = 0;
    int pUpCount = 0;
    bool pUpActive = false;
    float pUpTimer = 0;
    const float pUpDuration = 3;
    int lastpUpScore = 0;
    const int POWER_UP_INTERVAL = 30;

    int rewardCount = 0;
    int bonusMultiplier = 1;
    float bonusTimer = 0;
    bool showBonusText = false;
    string bonusText = "";
    const float bonusDisplayDuration = 2.0f;

    Players players;
    Leaderboard leaderboard;


    Theme darkTheme = { Color(30, 30, 30), Color::White, Color(50, 50, 50) };
    Theme lightTheme = { Color::White, Color::Black, Color(200, 200, 200) };
    Theme redTheme = { Color(100, 0, 0), Color::Red, Color(150, 0, 0) };
    Theme blueTheme = { Color(0, 0, 100), Color::Blue, Color(0, 0, 150) };

    string n1 = "Dark";
    string n2 = "Light";
    string n3 = "Red";
    string n4 = "Blue";

    Inventory inv;
    InvTree it1;
    inv.addTheme(n1, darkTheme);
    inv.addTheme(n2, lightTheme);
    inv.addTheme(n3, redTheme);
    inv.addTheme(n4, blueTheme);
    string currentThemeName = "Dark";
    Theme* currentTheme = inv.getTheme(currentThemeName);


    
    const char* gameMenu[] = { "Start Game", "Leaderboard", "Register/Login", "Multiplayer", "Player Menu", "Exit" };
    Menu mainMenu(6, sp.mFont, Vector2f(N * ts, M * ts));
    mainMenu.setLabels(gameMenu);

    const char* playerOptions[] = { "Profile", "Load Game", "Leaderboard", "Inventory", "Account", "Back" };
    Menu playerSubMenu(6, sp.mFont, Vector2f(N * ts, M * ts));
    playerSubMenu.setLabels(playerOptions);

    const char* pauseLabels[] = { "Resume", "Start New Game", "Save Game", "Leaderboard", "Exit to Menu" };
    Menu pauseMenu(5, sp.mFont, Vector2f(N * ts, M * ts));
    pauseMenu.setLabels(pauseLabels);

    const char* startLabels[] = { "Login", "Register", "Play as Guest", "Exit" };
    Menu introMenu(4, sp.mFont, Vector2f(N * ts, M * ts));
    introMenu.setLabels(startLabels);

    
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            switch (currentState) {
            case START_SCREEN:
                if (event.type == Event::MouseMoved)
                    introMenu.handleMouseMove(window.mapPixelToCoords(Mouse::getPosition(window)));

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                        introMenu.handleKeyboardInput(event.key.code);
                    else if (event.key.code == Keyboard::Enter) {
                        int selected = introMenu.getSelectedIndex();
                        if (selected == 0) currentState = LOGIN_SCREEN;
                        else if (selected == 1) currentState = REGISTER_SCREEN;
                        else if (selected == 2) currentState = MAIN_MENU;
                        else if (selected == 3) window.close();
                    }
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int selected = introMenu.handleMouseClick(window.mapPixelToCoords(Mouse::getPosition(window)));
                    if (selected == 0) currentState = LOGIN_SCREEN;
                    else if (selected == 1) currentState = REGISTER_SCREEN;
                    else if (selected == 2) currentState = MAIN_MENU;
                    else if (selected == 3) window.close();
                }
                break;

            case LOGIN_SCREEN:
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                    currentState = START_SCREEN;
                else
                    showLoginScreen(window, sp.mFont, currentState, players);
                break;

            case REGISTER_SCREEN:
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                    currentState = START_SCREEN;
                else
                    showRegisterScreen(window, sp.mFont, currentState, players);
                break;

            case MAIN_MENU:
                if (event.type == Event::MouseMoved)
                    mainMenu.handleMouseMove(window.mapPixelToCoords(Mouse::getPosition(window)));

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                        mainMenu.handleKeyboardInput(event.key.code);
                    else if (event.key.code == Keyboard::Enter) {
                        int selected = mainMenu.getSelectedIndex();
                        if (selected == 0) {
                            currentState = PLAYING;
                            resetGame(grid, x, y, dx, dy, Game, enemies, enemyCount, score,
                                pUpCount, pUpActive, pUpTimer, lastpUpScore);
                        }
                        else if (selected == 1) currentState = LEADERBOARD_SCREEN;
                        else if (selected == 2) currentState = START_SCREEN;
                        else if (selected == 3) currentState = MULTIPLAYER_MENU;
                        else if (selected == 4) currentState = PLAYER_MENU;
                        else if (selected == 5) window.close();
                    }
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int selected = mainMenu.handleMouseClick(window.mapPixelToCoords(Mouse::getPosition(window)));
                    if (selected == 0) {
                        currentState = PLAYING;
                        resetGame(grid, x, y, dx, dy, Game, enemies, enemyCount, score,
                            pUpCount, pUpActive, pUpTimer, lastpUpScore);
                    }
                    else if (selected == 1) currentState = LEADERBOARD_SCREEN;
                    else if (selected == 2) currentState = START_SCREEN;
                    else if (selected == 3) currentState = MULTIPLAYER_MENU;
                    else if (selected == 4) currentState = PLAYER_MENU;
                    else if (selected == 5) window.close();
                }
                break;

            case PLAYER_MENU:
                if (event.type == Event::MouseMoved)
                    playerSubMenu.handleMouseMove(window.mapPixelToCoords(Mouse::getPosition(window)));

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                        playerSubMenu.handleKeyboardInput(event.key.code);
                    else if (event.key.code == Keyboard::Enter) {
                        int selected = playerSubMenu.getSelectedIndex();
                        if (selected == 0) currentState = PROFILE_SCREEN;
                        else if (selected == 1) currentState = LOAD_GAME_SCREEN;
                        else if (selected == 2) currentState = LEADERBOARD_SCREEN;
                        else if (selected == 3) currentState = INVENTORY_SCREEN;
                        else if (selected == 4) currentState = ACCOUNT_SCREEN;
                        else if (selected == 5) currentState = MAIN_MENU;
                    }
                    else if (event.key.code == Keyboard::Escape) {
                        currentState = MAIN_MENU;
                    }
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int selected = playerSubMenu.handleMouseClick(window.mapPixelToCoords(Mouse::getPosition(window)));
                    if (selected == 0) currentState = PROFILE_SCREEN;
                    else if (selected == 1) currentState = LOAD_GAME_SCREEN;
                    else if (selected == 2) currentState = LEADERBOARD_SCREEN;
                    else if (selected == 3) currentState = INVENTORY_SCREEN;
                    else if (selected == 4) currentState = ACCOUNT_SCREEN;
                    else if (selected == 5) currentState = MAIN_MENU;
                }
                break;

            case LEADERBOARD_SCREEN:
            case PROFILE_SCREEN:
            case INVENTORY_SCREEN:
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        currentState = PLAYER_MENU;
                    }
                    else if (event.key.code == Keyboard::Num1) {
                        currentThemeName = "Dark";
                        currentTheme = inv.getTheme(currentThemeName);
                    }
                    else if (event.key.code == Keyboard::Num2) {
                        currentThemeName = "Light";
                        currentTheme = inv.getTheme(currentThemeName);
                    }
                    else if (event.key.code == Keyboard::Num3) {
                        currentThemeName = "Red";
                        currentTheme = inv.getTheme(currentThemeName);
                    }
                    else if (event.key.code == Keyboard::Num4) {
                        currentThemeName = "Blue";
                        currentTheme = inv.getTheme(currentThemeName);
                    }
                    break;
                }
            case ACCOUNT_SCREEN:
            case LOAD_GAME_SCREEN:
            case MULTIPLAYER_MENU:
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                    currentState = MAIN_MENU;
                break;

            case PAUSED:
                if (event.type == Event::MouseMoved)
                    pauseMenu.handleMouseMove(window.mapPixelToCoords(Mouse::getPosition(window)));

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                        pauseMenu.handleKeyboardInput(event.key.code);
                    else if (event.key.code == Keyboard::Enter) {
                        int selected = pauseMenu.getSelectedIndex();
                        if (selected == 0) currentState = PLAYING;
                        else if (selected == 1) {
                            resetGame(grid, x, y, dx, dy, Game, enemies, enemyCount, score,
                                pUpCount, pUpActive, pUpTimer, lastpUpScore);
                            currentState = PLAYING;
                        }
                        else if (selected == 2) currentState = SAVE_GAME_SCREEN;
                        else if (selected == 3) currentState = LEADERBOARD_SCREEN;
                        else if (selected == 4) currentState = MAIN_MENU;
                    }
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int selected = pauseMenu.handleMouseClick(window.mapPixelToCoords(Mouse::getPosition(window)));
                    if (selected == 0) currentState = PLAYING;
                    else if (selected == 1) {
                        resetGame(grid, x, y, dx, dy, Game, enemies, enemyCount, score,
                            pUpCount, pUpActive, pUpTimer, lastpUpScore);
                        currentState = PLAYING;
                    }
                    else if (selected == 2) currentState = SAVE_GAME_SCREEN;
                    else if (selected == 3) currentState = LEADERBOARD_SCREEN;
                    else if (selected == 4) currentState = MAIN_MENU;
                }
                break;

            case PLAYING:
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                    currentState = PAUSED;
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space && pUpCount > 0) {
                    pUpActive = true;
                    pUpTimer = pUpDuration;
                    pUpCount--;
                }
                break;

            case GAME_OVER:
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) currentState = MAIN_MENU;
                    else if (event.key.code == Keyboard::R) {
                        resetGame(grid, x, y, dx, dy, Game, enemies, enemyCount, score,
                            pUpCount, pUpActive, pUpTimer, lastpUpScore);
                        currentState = PLAYING;
                    }
                }
                break;
            }
        }

        
        if (currentState == PLAYING) {
            float time = clock.restart().asSeconds();
            timer += time;

            if (pUpActive) {
                pUpTimer -= time;
                if (pUpTimer <= 0) pUpActive = false;
            }

            if (showBonusText) {
                bonusTimer -= time;
                if (bonusTimer <= 0) showBonusText = false;
            }

            if (score >= 50 && (score - lastpUpScore) >= POWER_UP_INTERVAL) {
                int newPowerUps = (score - 50) / POWER_UP_INTERVAL - (lastpUpScore - 50) / POWER_UP_INTERVAL;
                pUpCount += newPowerUps;
                lastpUpScore = score;
            }

            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }

            if (timer > delay) {
                x += dx;
                y += dy;

                x = max(0, min(x, N - 1));
                y = max(0, min(y, M - 1));

                if (grid[y][x] == 2) Game = false;
                if (grid[y][x] == 0) grid[y][x] = 2;

                timer = 0;
            }

            if (!pUpActive)
                for (int i = 0; i < enemyCount; i++) enemies[i].move();

            if (grid[y][x] == 1) {
                dx = dy = 0;
                for (int i = 0; i < enemyCount; i++) drop(enemies[i].y / ts, enemies[i].x / ts);

                int capturedThisTurn = 0;
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < N; j++) {
                        if (grid[i][j] == 2 || grid[i][j] == 0) {
                            grid[i][j] = 1;
                            capturedThisTurn++;
                        }
                        else if (grid[i][j] == -1) {
                            grid[i][j] = 0;
                        }
                    }
                }

                bonusMultiplier = 1;
                if ((rewardCount < 3 && capturedThisTurn > 10) ||
                    (rewardCount >= 3 && rewardCount < 5 && capturedThisTurn > 5)) {
                    bonusMultiplier = 2;
                    rewardCount++;
                    showBonusText = true;
                    bonusTimer = bonusDisplayDuration;
                    bonusText = "2X Bonus!";
                }
                else if (rewardCount >= 5 && capturedThisTurn > 5) {
                    bonusMultiplier = 4;
                    rewardCount++;
                    showBonusText = true;
                    bonusTimer = bonusDisplayDuration;
                    bonusText = "4X Bonus!";
                }

                score += capturedThisTurn * bonusMultiplier;
            }

            for (int i = 0; i < enemyCount; i++)
                if (grid[enemies[i].y / ts][enemies[i].x / ts] == 2) {
                    score--;
                    Game = false;
                }

            if (!Game) {
                if (loggedIn) {
                    leaderboard.tryInsert(currentUser.username, currentUser.nickname, score);
                    players.updateScore(score);
                }
                currentState = GAME_OVER;
            }
        }

        window.clear(currentTheme ? currentTheme->bgColor : Color::Black);

        switch (currentState) {
        case START_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            introMenu.draw(window);
            break;

        case LOGIN_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            showLoginScreen(window, sp.mFont, currentState, players);
            break;

        case REGISTER_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            showRegisterScreen(window, sp.mFont, currentState, players);
            break;

        case MAIN_MENU:
            drawMainMenu(window, sp.mFont, sp.sCurrentBackground);
            mainMenu.draw(window);
            break;

        case PLAYER_MENU:
            window.draw(sp.sLeaderboardBackground);
            playerSubMenu.draw(window);
            break;

        case LEADERBOARD_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            leaderboard.display(window, sp.mFont);
            break;

        case PROFILE_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            players.display(window, sp.mFont);
            break;

        case PLAYING:
        case GAME_OVER:
        case PAUSED:
            drawGame(window, sp.sTile, sp.sEnemy, grid, x, y, enemies, enemyCount, 5.0f);
            drawScore(window, sp.mFont, score);
            drawPowerUpIndicator(window, sp.mFont, pUpCount);

            if (showBonusText)
                drawBonusText(window, sp.mFont, bonusText);

            if (pUpActive) {
                Text activeText("ENEMIES FROZEN!", sp.mFont, 28);
                activeText.setFillColor(Color::Red);
                activeText.setOutlineColor(Color::White);
                activeText.setOutlineThickness(2);
                activeText.setPosition(window.getSize().x / 2 - 150, 10);
                window.draw(activeText);
            }

            if (currentState == GAME_OVER)
                drawGameOver(window, sp.sGameover, sp.mFont, score);
            else if (currentState == PAUSED) {
                drawPauseMenu(window, sp.mFont);
                pauseMenu.draw(window);
            }
            break;

        case INVENTORY_SCREEN:
        {
           
            Text title("THEME SELECTION", sp.mFont, 40);
            title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 50);
            title.setFillColor(currentTheme ? currentTheme->textColor : Color::White);
            window.draw(title);

            
            Text instructions("Press 1-4 to select theme\nESC to return", sp.mFont, 20);
            instructions.setPosition(50, window.getSize().y - 100);
            instructions.setFillColor(currentTheme ? currentTheme->textColor : Color::White);
            window.draw(instructions);

            
            float startY = 150;
            float startX = window.getSize().x / 2 - 200;

            // Dark theme
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Num1) {
                    currentTheme = it1.getTheme("Dark");
                    currentThemeName = "Dark";
                }
                else if (event.key.code == Keyboard::Num2) {
                    currentTheme = it1.getTheme("Light");
                    currentThemeName = "Light";
                }
                else if (event.key.code == Keyboard::Num3) {
                    currentTheme = it1.getTheme("Red");
                    currentThemeName = "Red";
                }
                else if (event.key.code == Keyboard::Num4) {
                    currentTheme = it1.getTheme("Blue");
                    currentThemeName = "Blue";
                }
            }


            break;
        }
        case ACCOUNT_SCREEN:
        case LOAD_GAME_SCREEN:
        case MULTIPLAYER_MENU:
        case SAVE_GAME_SCREEN:
            window.draw(sp.sLeaderboardBackground);
            
            Text screenText("Screen Placeholder", sp.mFont, 30);
            screenText.setPosition(100, 100);
            window.draw(screenText);
            break;
        }
       
        window.display();
    }
  
    return 0;
}