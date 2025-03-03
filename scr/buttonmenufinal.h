#include <SFML/Graphics.hpp>
#include <iostream>

#define MAX_MENU_ITEMS 2

class Menu {
private:
    int selectedItemIndex;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture menuTextures[MAX_MENU_ITEMS];
    sf::Sprite menuSprites[MAX_MENU_ITEMS];
    bool* isMenuClicked;

public:
    Menu(float width, float height) {
        if (!font.loadFromFile("verdana.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        if (!backgroundTexture.loadFromFile("test.png")) {
            std::cerr << "Failed to load background image!" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(width / backgroundSprite.getLocalBounds().width,
            height / backgroundSprite.getLocalBounds().height);

        std::string menuFiles[MAX_MENU_ITEMS] = { "Continue-removebg-preview.png", "Exit-Photoroom.png" };

        sf::Vector2f positions[MAX_MENU_ITEMS] = {
            sf::Vector2f(100, 200),
            sf::Vector2f(400, 200)
        };

        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            if (!menuTextures[i].loadFromFile(menuFiles[i])) {
                std::cerr << "Failed to load " << menuFiles[i] << std::endl;
            }
            menuSprites[i].setTexture(menuTextures[i]);
            menuSprites[i].setScale(400 / menuSprites[i].getLocalBounds().width,
                250 / menuSprites[i].getLocalBounds().height);
            menuSprites[i].setPosition(positions[i]);
        }

        selectedItemIndex = 0;
        menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));
    }

    void setMenuClicked(bool* menuState) {
        isMenuClicked = menuState;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            window.draw(menuSprites[i]);
        }
    }

    void handleMouseClick(sf::Vector2f mousePos, sf::RenderWindow& window) {
        for (int i = 0; i < MAX_MENU_ITEMS; i++) {
            if (menuSprites[i].getGlobalBounds().contains(mousePos)) {
                menuSprites[selectedItemIndex].setColor(sf::Color::White);
                selectedItemIndex = i;
                menuSprites[selectedItemIndex].setColor(sf::Color(255, 100, 100));

                if (selectedItemIndex == 0) {
                    *isMenuClicked = false;
                }
                else if (selectedItemIndex == 1) {
                    window.close();
                }
            }
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu");
    Menu menu(window.getSize().x, window.getSize().y);

    bool isMenuClicked = false;
    menu.setMenuClicked(&isMenuClicked);

    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(50, 30);

    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    sf::Text buttonText(" Menu", font, 20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(60, 40);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                    if (button.getGlobalBounds().contains(mousePos)) {
                        isMenuClicked = true;
                    }

                    if (isMenuClicked) {
                        menu.handleMouseClick(mousePos, window);
                    }
                }
            }
        }

        window.clear();
        if (isMenuClicked) {
            menu.draw(window);
        }
        else {
            window.draw(button);
            window.draw(buttonText);
        }
        window.display();
    }
    return 0;
}
