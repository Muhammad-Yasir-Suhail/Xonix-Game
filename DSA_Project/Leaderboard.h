#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;
struct Leader {
    string username;
    string name;
    int score;
};
class Leaderboard {
    Leader heap[10];
    int sz;
    void heapifyDown(int i);
    void heapifyUp(int i);
    void saveToFile();
    void loadFromFile();
public:
    Leaderboard();
    void tryInsert(string user, string name, int scr);
    void display(RenderWindow& win, Font& font);
};
