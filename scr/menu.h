#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define MAX_MENU_ITEMS 3

class Menu {
private:
    int selectedItemIndex;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture menuTextures[MAX_MENU_ITEMS];
    sf::Sprite menuSprites[MAX_MENU_ITEMS];
    sf::Text selectedItemText;

public:
    Menu(float width, float height) {
        // Load font (Verdana)
        if (!font.loadFromFile("verdana.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Load background image
        if (!backgroundTexture.loadFromFile("test.png")) {
            std::cerr << "Failed to load background image!" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(width / backgroundSprite.getLocalBounds().width,
            height / backgroundSprite.getLocalBounds().height);

        // Load menu item images
        std::string menuFiles[MAX_MENU_ITEMS] = { "Continue-removebg-preview.png", "Restart-removebg-preview.png", "Exit-Photoroom.png" };

        // Set positions for menu items (x, y)
        sf::Vector2f positions[MAX_MENU_ITEMS] = {
            sf::Vector2f(-30, 200),  // Position for Play (250, 200)
            sf::Vector2f(250, 200),  // Position for Options (250, 300)
            sf::Vector2f(500, 200)   // Position for Exit (300, 400)
        };

        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            if (!menuTextures[i].loadFromFile(menuFiles[i])) {
                std::cerr << "Failed to load " << menuFiles[i] << std::endl;
            }
            menuSprites[i].setTexture(menuTextures[i]);

            // Scale menu items to fit within the desired size
            menuSprites[i].setScale(400 / menuSprites[i].getLocalBounds().width,
                250 / menuSprites[i].getLocalBounds().height);

            // Set the position of each menu item based on the positions array (x, y)
            menuSprites[i].setPosition(positions[i]);
        }

        // Initialize the selected item index (default is 0)
        selectedItemIndex = 0;
        menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));

        // Set up the text to display the selected item
        selectedItemText.setFont(font);
        selectedItemText.setFillColor(sf::Color::Yellow);
        selectedItemText.setCharacterSize(24);
        selectedItemText.setPosition(width / 2 - 100, height - 100);
        selectedItemText.setString("Selected: " + menuFiles[selectedItemIndex]);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            window.draw(menuSprites[i]);
        }
        window.draw(selectedItemText);
    }

    void moveUp() {
        if (selectedItemIndex > 0) {
            menuSprites[selectedItemIndex].setColor(sf::Color::White);
            selectedItemIndex--;
            menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));
            updateSelectedItemText();
        }
    }

    void moveDown() {
        if (selectedItemIndex < MAX_MENU_ITEMS - 1) {
            menuSprites[selectedItemIndex].setColor(sf::Color::White);
            selectedItemIndex++;
            menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));
            updateSelectedItemText();
        }
    }

    void updateSelectedItemText() {
        std::string menuFiles[MAX_MENU_ITEMS] = { "Continue", "Restart", "Exit" };
        selectedItemText.setString("Selected: " + menuFiles[selectedItemIndex]);
    }

    void handleMouseClick(sf::Vector2f mousePos) {
        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            if (menuSprites[i].getGlobalBounds().contains(mousePos)) {
                menuSprites[selectedItemIndex].setColor(sf::Color::White);
                selectedItemIndex = i;
                menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));
                updateSelectedItemText();
            }
        }
    }

    int getSelectedItemIndex() {
        return selectedItemIndex;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu");
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = menu.getSelectedItemIndex();
                    if (selected == 2) { // Exit option
                        window.close();
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    menu.handleMouseClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }
    return 0;
}