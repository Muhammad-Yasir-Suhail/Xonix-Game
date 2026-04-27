#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameState.h"
using namespace std;
using namespace sf;
void showLoginScreen(RenderWindow& win, Font& font, GameState& state, Players& players) {
    string in[2] = { "", "" };
    string labels[2] = { "Username:", "Password:" };
    int active = -1;
    bool success = false;
    Text title;
    title.setFont(font);
    title.setString("Login");
    title.setCharacterSize(40);
    title.setFillColor(Color(255, 215, 0));
    title.setOutlineColor(Color(139, 69, 19));
    title.setOutlineThickness(4);
    title.setStyle(Text::Bold);
    FloatRect tBounds = title.getLocalBounds();
    title.setPosition(win.getSize().x / 2 - tBounds.width / 2, 30);
    Text lbl[2], txt[2], msg;
    RectangleShape box[2];
    for (int i = 0; i < 2; i++) {
        lbl[i].setFont(font);
        lbl[i].setString(labels[i]);
        lbl[i].setCharacterSize(20);
        lbl[i].setFillColor(Color::White);
        lbl[i].setPosition(130, 130 + i * 70);

        box[i].setSize(Vector2f(200, 30));
        box[i].setPosition(270, 129 + i * 70);
        box[i].setFillColor(Color::White);
        box[i].setOutlineThickness(2);
        box[i].setOutlineColor(Color::Black);

        txt[i].setFont(font);
        txt[i].setCharacterSize(20);
        txt[i].setFillColor(Color::Black);
        txt[i].setPosition(273, 130 + i * 70);
    }

    RectangleShape loginBtn(Vector2f(130, 50));
    loginBtn.setPosition(150, 270);
    loginBtn.setFillColor(Color(100, 200, 100));

    RectangleShape backBtn(Vector2f(130, 50));
    backBtn.setPosition(350, 270);
    backBtn.setFillColor(Color(200, 100, 100));

    Text loginTxt;
    loginTxt.setFont(font);
    loginTxt.setString("Login");
    loginTxt.setCharacterSize(24);
    loginTxt.setFillColor(Color::Black);
    loginTxt.setPosition(176, 276);

    Text backTxt;
    backTxt.setFont(font);
    backTxt.setString("Back");
    backTxt.setCharacterSize(24);
    backTxt.setFillColor(Color::Black);
    backTxt.setPosition(374, 276);

    msg.setFont(font);
    msg.setCharacterSize(20);

    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) {
                win.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                Vector2f mPos = win.mapPixelToCoords(Mouse::getPosition(win));
                active = -1;
                for (int i = 0; i < 2; i++) {
                    if (box[i].getGlobalBounds().contains(mPos)) {
                        active = i;
                    }
                }
                if (backBtn.getGlobalBounds().contains(mPos)) {
                    if (success) {
                        state = GameState::MAIN_MENU;
                    }
                    else {
                        state = GameState::PLAYING;
                    }
                    return;
                }
                if (loginBtn.getGlobalBounds().contains(mPos)) {
                    Text dummy;
                    if (players.login(in[0], in[1], dummy)) {
                        msg.setString("Login successful! Click 'Back' to continue.");
                        msg.setFillColor(Color::Green);
                        success = true;
                    }
                    else {
                        msg.setString("Login failed! Invalid credentials.");
                        msg.setFillColor(Color::Red);
                        success = false;
                    }
                    FloatRect mb = msg.getLocalBounds();
                    msg.setPosition(win.getSize().x / 2 - mb.width / 2, 10);
                }
            }

            if (e.type == Event::TextEntered && active != -1) {
                if (e.text.unicode == 8) {
                    if (!in[active].empty()) {
                        in[active].pop_back();
                    }
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 127) {
                    if (in[active].length() < 20) {
                        in[active] += static_cast<char>(e.text.unicode);
                    }
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            box[i].setOutlineColor((i == active) ? Color::Blue : Color::Black);
            txt[i].setString(in[i]);
        }

        win.clear(Color(45, 40, 30));
        win.draw(title);
        for (int i = 0; i < 2; i++) {
            win.draw(lbl[i]);
            win.draw(box[i]);
            win.draw(txt[i]);
        }
        win.draw(loginBtn);
        win.draw(loginTxt);
        win.draw(backBtn);
        win.draw(backTxt);
        win.draw(msg);
        win.display();
    }
}
void showRegisterScreen(RenderWindow& win, Font& font, GameState& state, Players& players) {
    string in[4] = { "", "", "", "" };
    string labels[4] = { "Username:", "Password:", "Nickname:", "Email:" };
    int active = -1;
    bool success = false;
    Text title;
    title.setFont(font);
    title.setString("Register");
    title.setCharacterSize(40);
    title.setFillColor(Color(255, 215, 0));
    title.setOutlineColor(Color(139, 69, 19));
    title.setOutlineThickness(4);
    title.setStyle(Text::Bold);
    FloatRect tBounds = title.getLocalBounds();
    title.setPosition(win.getSize().x / 2 - tBounds.width / 2, 30);
    Text lbl[4], txt[4], msg;
    RectangleShape box[4];
    for (int i = 0; i < 4; i++) {
        lbl[i].setFont(font);
        lbl[i].setString(labels[i]);
        lbl[i].setCharacterSize(20);
        lbl[i].setFillColor(Color::White);
        lbl[i].setPosition(130, 130 + i * 70);

        box[i].setSize(Vector2f(200, 30));
        box[i].setPosition(270, 129 + i * 70);
        box[i].setFillColor(Color::White);
        box[i].setOutlineThickness(2);
        box[i].setOutlineColor(Color::Black);

        txt[i].setFont(font);
        txt[i].setCharacterSize(20);
        txt[i].setFillColor(Color::Black);
        txt[i].setPosition(273, 130 + i * 70);
    }
    RectangleShape regBtn(Vector2f(190, 50));
    regBtn.setPosition(150, 410);
    regBtn.setFillColor(Color(100, 200, 100));
    RectangleShape backBtn(Vector2f(130, 50));
    backBtn.setPosition(350, 410);
    backBtn.setFillColor(Color(200, 100, 100));
    Text regTxt;
    regTxt.setFont(font);
    regTxt.setString("Register");
    regTxt.setCharacterSize(24);
    regTxt.setFillColor(Color::Black);
    regTxt.setPosition(175, 418);
    Text backTxt;
    backTxt.setFont(font);
    backTxt.setString("Back");
    backTxt.setCharacterSize(24);
    backTxt.setFillColor(Color::Black);
    backTxt.setPosition(375, 418);
    msg.setFont(font);
    msg.setCharacterSize(20);
    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) {
                win.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                Vector2f mPos = win.mapPixelToCoords(Mouse::getPosition(win));
                active = -1;
                for (int i = 0; i < 4; i++) {
                    if (box[i].getGlobalBounds().contains(mPos)) {
                        active = i;
                    }
                }
                if (backBtn.getGlobalBounds().contains(mPos)) {
                    if (success) {
                        state = GameState::MAIN_MENU;
                    }
                    else {
                        state = GameState::PLAYING;
                    }
                    return;
                }
                if (regBtn.getGlobalBounds().contains(mPos)) {
                    Text dummy;
                    if (players.RegisterNewPlayer(in[0], in[1], dummy, in[2], in[3])) {
                        msg.setString("Registration successful! Click 'Back' to continue.");
                        msg.setFillColor(Color::Green);
                        success = true;
                    }
                    else {
                        msg.setString("Registration failed! Username exists.");
                        msg.setFillColor(Color::Red);
                        success = false;
                    }
                    FloatRect mb = msg.getLocalBounds();
                    msg.setPosition(win.getSize().x / 2 - mb.width / 2, 10);
                }
            }
            if (e.type == Event::TextEntered && active != -1) {
                if (e.text.unicode == 8) {
                    if (!in[active].empty()) {
                        in[active].pop_back();
                    }
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 127) {
                    if (in[active].length() < 20) {
                        in[active] += static_cast<char>(e.text.unicode);
                    }
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            box[i].setOutlineColor((i == active) ? Color::Blue : Color::Black);
            txt[i].setString(in[i]);
        }
        win.clear(Color(45, 40, 30));
        win.draw(title);
        for (int i = 0; i < 4; i++) {
            win.draw(lbl[i]);
            win.draw(box[i]);
            win.draw(txt[i]);
        }
        win.draw(regBtn);
        win.draw(regTxt);
        win.draw(backBtn);
        win.draw(backTxt);
        win.draw(msg);
        win.display();
    }
}
