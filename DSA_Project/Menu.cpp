#include "Menu.h"
Menu::Menu(int count, Font& f, Vector2f size) : font(f), windowSize(size), itemCount(count), selectedIndex(0) {
    items = new MenuItem[itemCount];
    float w = 200.f;
    float h = 50.f;
    float y = windowSize.y / 2 - (itemCount * (h + 10)) / 2;
    for (int i = 0; i < itemCount; i++) {
        items[i].rect.setSize(Vector2f(w, h));
        items[i].rect.setPosition(windowSize.x / 2 - w / 2, y + i * (h + 10));
        items[i].rect.setFillColor(Color(70, 70, 70, 200));
        items[i].rect.setOutlineThickness(2);
        items[i].rect.setOutlineColor(Color::White);
        items[i].text.setFont(font);
        items[i].text.setCharacterSize(20);
        items[i].text.setFillColor(Color::Yellow);
        items[i].isSelected = false;
        items[i].text.setPosition(items[i].rect.getPosition().x + 15, items[i].rect.getPosition().y + 10);
    }
}
Menu::~Menu() {
    delete[] items;
}
void Menu::setLabels(const char* labels[]) {
    for (int i = 0; i < itemCount; i++) {
        items[i].text.setString(labels[i]);
    }
}
void Menu::draw(RenderWindow& window) {
    for (int i = 0; i < itemCount; i++) {
        window.draw(items[i].rect);
        window.draw(items[i].text);
    }
}
void Menu::handleMouseMove(Vector2f mousePos) {
    for (int i = 0; i < itemCount; i++) {
        items[i].isSelected = false;
        if (items[i].rect.getGlobalBounds().contains(mousePos)) {
            selectedIndex = i;
            items[i].isSelected = true;
            items[i].rect.setFillColor(Color(20, 100, 100, 200));
        }
        else {
            items[i].rect.setFillColor(Color(70, 70, 70, 200));
        }
    }
}
int Menu::handleMouseClick(Vector2f mousePos) {
    for (int i = 0; i < itemCount; i++) {
        if (items[i].rect.getGlobalBounds().contains(mousePos)) {
            return i;
        }
    }
    return -1;
}
void Menu::handleKeyboardInput(Keyboard::Key key) {
    if (key == Keyboard::Down) {
        selectedIndex = (selectedIndex + 1) % itemCount;
    }
    else if (key == Keyboard::Up) {
        selectedIndex = (selectedIndex - 1 + itemCount) % itemCount;
    }

    for (int i = 0; i < itemCount; i++) {
        if (i == selectedIndex) {
            items[i].isSelected = true;
            items[i].rect.setFillColor(Color(20, 100, 100, 200));
        }
        else {
            items[i].isSelected = false;
            items[i].rect.setFillColor(Color(70, 70, 70, 200));
        }
    }
}
int Menu::getSelectedIndex() {
    return selectedIndex;
}
