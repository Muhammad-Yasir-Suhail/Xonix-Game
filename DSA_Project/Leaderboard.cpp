#include "Leaderboard.h"
#include "Utility.h"
Leaderboard::Leaderboard() {
    sz = 0;
    loadFromFile();
}
void Leaderboard::heapifyDown(int i) {
    int min = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < sz && heap[l].score < heap[min].score) {
        min = l;
    }
    if (r < sz && heap[r].score < heap[min].score) {
        min = r;
    }
    if (min != i) {
        Swap(heap[i], heap[min]);
        heapifyDown(min);
    }
}
void Leaderboard::heapifyUp(int i) {
    int p = (i - 1) / 2;
    if (i > 0 && heap[i].score < heap[p].score) {
        Swap(heap[i], heap[p]);
        heapifyUp(p);
    }
}
void Leaderboard::tryInsert(string user, string name, int scr) {
    if (user == "") {
        return;
    }
    for (int i = 0; i < sz; i++) {
        if (heap[i].username == user) {
            if (scr > heap[i].score) {
                heap[i].score = scr;
                heapifyDown(i);
                heapifyUp(i);
                saveToFile();
            }
            return;
        }
    }
    if (sz < 10) {
        heap[sz++] = { user, name, scr };
        heapifyUp(sz - 1);
        saveToFile();
    }
    else if (scr > heap[0].score) {
        heap[0] = { user, name, scr };
        heapifyDown(0);
        saveToFile();
    }
}
void Leaderboard::saveToFile() {
    ofstream f("Leaderboard.csv", ios::out);
    for (int i = 0; i < sz; i++) {
        f << heap[i].username << "," << heap[i].name << "," << heap[i].score << endl;
    }
    f.close();
}
void Leaderboard::loadFromFile() {
    ifstream f("Leaderboard.csv");
    string line;
    sz = 0;
    while (getline(f, line) && sz < 10) {
        int i = 0;
        string u = "", n = "", s = "";
        while (i < Length(line) && line[i] != ',') {
            u += line[i++];
        }
        i++;
        while (i < Length(line) && line[i] != ',') {
            n += line[i++];
        }
        i++;
        while (i < Length(line)) {
            s += line[i++];
        }
        heap[sz++] = { u, n, StringToInt(s) };
    }
    f.close();
    for (int i = sz / 2 - 1; i >= 0; i--) {
        heapifyDown(i);
    }
}
void Leaderboard::display(RenderWindow& win, Font& font) {
    Text title("Leaderboard", font, 50);
    title.setFillColor(Color(255, 215, 0));
    title.setOutlineColor(Color(139, 69, 19));
    title.setOutlineThickness(4);
    title.setStyle(Text::Bold);
    title.setPosition(200, 20);
    win.draw(title);
    Leader tmp[10];
    for (int i = 0; i < sz; i++) {
        tmp[i] = heap[i];
    }
    for (int i = 0; i < sz - 1; i++) {
        int mx = i;
        for (int j = i + 1; j < sz; j++) {
            if (tmp[j].score > tmp[mx].score) {
                mx = j;
            }
        }
        if (mx != i) {
            Swap(tmp[i], tmp[mx]);
        }
    }
    if (sz == 0) {
        Text t;
        t.setFont(font);
        t.setCharacterSize(24);
        t.setFillColor(Color::White);
        t.setPosition(100, 100);
        t.setString("Leaderboard is empty.");
        win.draw(t);
        return;
    }
    for (int i = 0; i < sz; i++) {
        Text t;
        t.setFont(font);
        t.setCharacterSize(24);
        t.setOutlineThickness(2);
        t.setOutlineColor(Color::Red);
        t.setFillColor(Color::Yellow);
        t.setPosition(100, 100 + i * 30);
        t.setString(StringConverter(i + 1) + ") " + tmp[i].name + " " + StringConverter(tmp[i].score));
        win.draw(t);
    }
}
