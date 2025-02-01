#include <SFML/Graphics.hpp>
#include <iostream> // For debugging

int main() {
    // Create a window with a title
    sf::RenderWindow window(sf::VideoMode(1800, 600), "SFML Test");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }

    // Create a text object
    sf::Text text("Bobby", font, 50);
    text.setFillColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Bold);

    // Center the text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}
