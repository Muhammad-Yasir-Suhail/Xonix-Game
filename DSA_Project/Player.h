#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Utility.h"
using namespace std;
using namespace sf;

struct Player {
    string username;
    string password;
    string nickname;
    string email;
    int totalScore;
    string matchHistory;
    string friendsList;
    Player* next;

    Player(string usr = "", string pw = "", string nn = "", string em = "", int sc = 0, string mh = "", string fl = "") {
        username = usr;
        password = pw;
        nickname = nn;
        email = em;
        totalScore = sc;
        matchHistory = mh;
        friendsList = fl;
        next = nullptr;
    }
};

Player currentUser;
bool loggedIn = false;

class Players {
    Player* root;

public:
    Players(Player* root = nullptr) {
        this->root = root;
    }

    bool CheckUsername(string un) {
        fstream file("Players.csv", ios::in);
        string line;

        while (getline(file, line)) {
            string user = "";
            int i = 0;

            while (line[i] != ',' && i < line.length()) {
                user += line[i];
                ++i;
            }

            if (user == un) {
                return false;
            }
        }

        return true;
    }

    bool checkIfSecure(string pass) {
        if (pass.length() < 8) {
            return false;
        }

        bool hasUpper = false;

        for (int i = 0; i < pass.length(); i++) {
            if (isupper(pass[i])) {
                hasUpper = true;
                break;
            }
        }

        return hasUpper;
    }

    bool RegisterNewPlayer(string un, string pass, Text& text, string nn = "", string em = "") {
        if (!CheckUsername(un)) {
            text.setString("Username exists. Choose another username.");
            text.setFillColor(Color::Red);
            return false;
        }

        if (!checkIfSecure(pass)) {
            text.setString("Password should be at least 8 characters and include uppercase.");
            text.setFillColor(Color::Red);
            return false;
        }

        Player* n = new Player(un, pass, nn, em, 0, "", "");
        n->next = root;
        root = n;

        fstream file("Players.csv", ios::app);

        if (file.is_open()) {
            file << un << "," << pass << "," << nn << "," << em << "," << 0 << "," << "" << "," << "" << endl;
            text.setString("Registration successful.");
            text.setFillColor(Color::Green);
            return true;
        }
        text.setString("Failed to open file.");
        text.setFillColor(Color::Red);
        return false;
    }
    string getNicknameFromFile(string user) {
        fstream file("Players.csv", ios::in);
        string line;
        while (getline(file, line)) {
            string un = "", nn = "";
            int i = 0;
            while (line[i] != ',' && i < Length(line)) {
                un += line[i];
                i++;
            }
            i++;
            while (i < Length(line) && line[i] != ',') {
                i++;
            }
            i++;        
            while (i < Length(line) && line[i] != ',') {
                nn += line[i];
                i++;
            }
            if (un == user) {
                return nn;
            }
        }
        return "Unknown";
    }
    int getScoreFromFile(string user) {
        fstream file("Scores.csv", ios::in);
        string line;
        while (getline(file, line)) {
            string un = "";
            int i = 0;
            while (i < Length(line) && line[i] != ',') {
                un += line[i];
                i++;
            }
            if (un == user) {
                i++;
                string scoreStr = "";
                while (i < Length(line)) {
                    scoreStr += line[i];
                    i++;
                }
                return StringToInt(scoreStr);
            }
        }
        return 0;
    }
    bool login(string user, string pw, Text& text) {
        fstream file("Players.csv", ios::in);
        string l;
        while (getline(file, l)) {
            string un = "", pass = "", nn = "", em = "", ts = "", mh = "", fl = "";
            int field = 0;
            string fields[7];
            for (int j = 0; j < l.length(); j++) {
                if (l[j] == ',') {
                    field++;
                }
                else {
                    fields[field] += l[j];
                }
            }
            un = fields[0];
            pass = fields[1];
            nn = fields[2];
            em = fields[3];
            ts = fields[4];
            mh = fields[5];
            fl = fields[6];

            if (un == user && pass == pw) {
                loggedIn = true;
                currentUser.username = un;
                currentUser.password = pass;
                currentUser.nickname = nn;
                currentUser.email = em;
                currentUser.totalScore = StringToInt(ts);
                currentUser.matchHistory = mh;
                currentUser.friendsList = fl;

                text.setString("Login successful!");
                text.setFillColor(Color::Green);
                return true;
            }
        }
        loggedIn = false;
        text.setString("Invalid username or password.");
        text.setFillColor(Color::Red);
        return false;
    }
    void updateScore(int score) {
        if (loggedIn == false) {
            return;
        }
        currentUser.totalScore += score;
        fstream file("Players.csv", ios::in);
        ofstream temp("temp.csv");
        string line;

        while (getline(file, line)) {
            string fields[7];
            int field = 0;

            for (char ch : line) {
                if (ch == ',') {
                    field++;
                }
                else {
                    fields[field] += ch;
                }
            }

            if (fields[0] == currentUser.username) {
                temp << fields[0] << "," << fields[1] << "," << fields[2] << "," << fields[3]
                    << "," << currentUser.totalScore << "," << fields[5] << "," << fields[6] << endl;
            }
            else {
                temp << line << endl;
            }
        }
        file.close();
        temp.close();
        remove("Players.csv");
        rename("temp.csv", "Players.csv");
    }

    void display(RenderWindow& window, Font& font) {
        window.clear(Color(30, 30, 30));
        Text title("PLAYER PROFILE", font, 42);
        title.setFillColor(Color::Yellow);
        title.setStyle(Text::Bold);
        title.setOutlineColor(Color::Red);
        title.setOutlineThickness(3);
        title.setPosition(160, 25);
        window.draw(title);
        RectangleShape profileBox;
        profileBox.setSize(Vector2f(580, 360));
        profileBox.setFillColor(Color(20, 20, 20));
        profileBox.setOutlineColor(Color::Blue);
        profileBox.setOutlineThickness(3);
        profileBox.setPosition(60, 100);
        window.draw(profileBox);
        if (loggedIn == false) {
            Text msg("Log in to view profile", font, 26);
            msg.setFillColor(Color::Red);
            msg.setStyle(Text::Bold);
            msg.setPosition(180, 250);
            window.draw(msg);
            return;
        }
        string labels[6] = { "Username", "Nickname", "Email", "Total Score", "Match Points", "Friends"};
        string values[6] = {
            currentUser.username,
            currentUser.nickname,
            currentUser.email,
            StringConverter(currentUser.totalScore),
            currentUser.matchHistory.empty() ?"None" : currentUser.matchHistory,
            currentUser.friendsList.empty() ? "Friend list is empty" : currentUser.friendsList
        };
        float y = 120;
        for (int i = 0; i < 6; i++) {
            Text lbl(labels[i] + " ", font, 17);
            lbl.setFillColor(Color::White);
            lbl.setStyle(Text::Bold);
            lbl.setPosition(80, y);
            window.draw(lbl);
            RectangleShape line;
            line.setSize(Vector2f(350, 30));
            line.setFillColor(Color(40, 40, 40));
            line.setOutlineColor(Color::Yellow);
            line.setOutlineThickness(1);
            line.setPosition(240, y);
            window.draw(line);

            Text val(values[i], font, 22);
            val.setFillColor(Color::Yellow);
            val.setPosition(250, y + 2);
            window.draw(val);
            y += 50;
        }
    }
};
