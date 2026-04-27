#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


struct Theme {
	Color bgColor;
	Color textColor;
	Color btnColor;
	Color highlightColor;
	string name;
};


struct InvNode {
	string name;
	int height;
	Theme thm;
	InvNode* left;
	InvNode* right;

	InvNode(string name = "", Theme t = {}, int h = 1)
		: name(name), thm(t), height(h), left(nullptr), right(nullptr) {
	}
};


class InvTree {
	InvNode* root;

	int height(InvNode* node) {
		return node ? node->height : 0;
	}

	int balanceFactor(InvNode* node) {
		return node ? height(node->left) - height(node->right) : 0;
	}

	int max(int a, int b) {
		return (a > b) ? a : b;
	}

	void updateHeight(InvNode* node) {
		if (node)
			node->height = 1 + max(height(node->left), height(node->right));
	}

	InvNode* rightRotate(InvNode* y) {
		InvNode* x = y->left;
		InvNode* T2 = x->right;

		x->right = y;
		y->left = T2;

		updateHeight(y);
		updateHeight(x);
		return x;
	}

	InvNode* leftRotate(InvNode* x) {
		InvNode* y = x->right;
		InvNode* T2 = y->left;

		y->left = x;
		x->right = T2;

		updateHeight(x);
		updateHeight(y);
		return y;
	}

	InvNode* insert(InvNode* node, const string& key, const Theme& data) {
		if (!node) return new InvNode(key, data);

		if (key < node->name)
			node->left = insert(node->left, key, data);
		else if (key > node->name)
			node->right = insert(node->right, key, data);
		else
			return node;

		updateHeight(node);
		int balance = balanceFactor(node);

		
		if (balance > 1 && key < node->left->name)
			return rightRotate(node);
		if (balance < -1 && key > node->right->name)
			return leftRotate(node);
		if (balance > 1 && key > node->left->name) {
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}
		if (balance < -1 && key < node->right->name) {
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node;
	}

	InvNode* minValueNode(InvNode* node) {
		InvNode* curr = node;
		while (curr && curr->left)
			curr = curr->left;
		return curr;
	}

	InvNode* remove(InvNode* root, const string& key) {
		if (!root) return nullptr;

		if (key < root->name)
			root->left = remove(root->left, key);
		else if (key > root->name)
			root->right = remove(root->right, key);
		else {
			if (!root->left || !root->right) {
				InvNode* temp = root->left ? root->left : root->right;
				delete root;
				return temp;
			}

			InvNode* temp = minValueNode(root->right);
			root->name = temp->name;
			root->thm = temp->thm;
			root->right = remove(root->right, temp->name);
		}

		updateHeight(root);
		int balance = balanceFactor(root);

		
		if (balance > 1 && balanceFactor(root->left) >= 0)
			return rightRotate(root);
		if (balance > 1 && balanceFactor(root->left) < 0) {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
		if (balance < -1 && balanceFactor(root->right) <= 0)
			return leftRotate(root);
		if (balance < -1 && balanceFactor(root->right) > 0) {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	Theme* search(InvNode* node, const string& name) {
		if (!node)
			return nullptr;
		else if (name < node->name)
			return search(node->left, name);
		else if (name > node->name)
			return search(node->right, name);
		else
			return &(node->thm);
	}

	void inorder(InvNode* node) {
		if (node) {
			inorder(node->left);
			cout << node->name << " ";
			inorder(node->right);
		}
	}

public:
	InvTree() : root(nullptr) {}

	void addTheme(const string& name, const Theme& data) {
		root = insert(root, name, data);
	}

	void removeTheme(const string& name) {
		root = remove(root, name);
	}

	Theme* getTheme(const string& name) {
		return search(root, name);
	}

	void displayThemes() {
		cout << "Available Themes: ";
		inorder(root);
		cout << endl;
	}

	
void drawThemePreview(RenderWindow& window, Font& font, float x, float y,
	const Theme& theme, bool isSelected, Color textColor)  {
		
		RectangleShape bg(Vector2f(400, 100));
		bg.setPosition(x, y);
		bg.setFillColor(theme.btnColor);
		bg.setOutlineThickness(2);
		bg.setOutlineColor(isSelected ? Color::Yellow : theme.bgColor);
		window.draw(bg);

		
		Text nameText(theme.name, font, 24);
		nameText.setPosition(x + 20, y + 20);
		nameText.setFillColor(textColor);
		window.draw(nameText);

		
		RectangleShape bgSample(Vector2f(50, 30));
		bgSample.setPosition(x + 300, y + 20);
		bgSample.setFillColor(theme.bgColor);
		window.draw(bgSample);

		RectangleShape textSample(Vector2f(50, 30));
		textSample.setPosition(x + 300, y + 60);
		textSample.setFillColor(theme.textColor);
		window.draw(textSample);

		if (isSelected) {
			Text selectedText("SELECTED", font, 20);
			selectedText.setPosition(x + 150, y + 60);
			selectedText.setFillColor(Color::Green);
			window.draw(selectedText);
		}
	}
};


class Inventory {
	InvTree tree;
public:
	void addTheme(const string& theme, const Theme& t) {
		tree.addTheme(theme, t);
	}

	void removeTheme(const string& theme) {
		tree.removeTheme(theme);
	}

	void showThemes() {
		tree.displayThemes();
	}

	Theme* getTheme(const string& name) {
		return tree.getTheme(name);
	}

	
	void drawPreview(RenderWindow& window, Font& font, float x, float y,
		const string& themeName, bool isSelected, Color textColor) {
		Theme* theme = tree.getTheme(themeName);
		if (theme) {
			tree.drawThemePreview(window, font, x, y, *theme, isSelected, textColor);
		}
	}
};


void applyTheme(RenderWindow& window, Theme* theme) {
	if (theme)
		window.clear(theme->bgColor);
	else
		window.clear(Color::Black);
}
