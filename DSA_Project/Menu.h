#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
using namespace sf;
struct MenuItem {
    RectangleShape rect;
    Text text;
    bool isSelected;
};
class Menu {
private:
    Font& font;
    Vector2f windowSize;
    int itemCount;
    int selectedIndex;
    MenuItem* items;
public:
    Menu(int count, Font& f, Vector2f size);
    ~Menu();
    void setLabels(const char* labels[]);
    void draw(RenderWindow& window);
    void handleMouseMove(Vector2f mousePos);
    int handleMouseClick(Vector2f mousePos);
    void handleKeyboardInput(Keyboard::Key key);
    int getSelectedIndex();
};

#endif
