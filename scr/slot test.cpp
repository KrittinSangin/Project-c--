#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace sf;

// ฟังก์ชันสุ่มค่าให้สล็อต
int spinReel() {
    return rand() % 5 + 1; // สุ่มตัวเลขระหว่าง 1 ถึง 5
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(800, 600), "Slot Machine");

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    Text slot1Text("1", font, 50);
    slot1Text.setPosition(200, 250);
    slot1Text.setFillColor(Color::White);

    Text slot2Text("1", font, 50);
    slot2Text.setPosition(350, 250);
    slot2Text.setFillColor(Color::White);

    Text slot3Text("1", font, 50);
    slot3Text.setPosition(500, 250);
    slot3Text.setFillColor(Color::White);

    RectangleShape spinButton(Vector2f(200, 60));
    spinButton.setPosition(300, 400);
    spinButton.setFillColor(Color::Green);

    Text spinText("Spin", font, 40);
    spinText.setPosition(360, 410);
    spinText.setFillColor(Color::Black);

    bool isSpinning = false;
    Clock clock;
    float spinDuration = 2.0f;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (spinButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isSpinning = true;
                    clock.restart();
                }
            }
        }

        if (isSpinning) {
            float elapsed = clock.getElapsedTime().asSeconds();
            if (elapsed < spinDuration) {
                slot1Text.setString(std::to_string(spinReel()));
                slot2Text.setString(std::to_string(spinReel()));
                slot3Text.setString(std::to_string(spinReel()));
            }
            else {
                slot1Text.setString(std::to_string(spinReel()));
                slot2Text.setString(std::to_string(spinReel()));
                slot3Text.setString(std::to_string(spinReel()));
                isSpinning = false;
            }
        }

        window.clear(Color::Black);
        window.draw(slot1Text);
        window.draw(slot2Text);
        window.draw(slot3Text);
        window.draw(spinButton);
        window.draw(spinText);
        window.display();
    }

    return 0;
}
