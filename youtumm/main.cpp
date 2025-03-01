#include <SFML/Graphics.hpp>
#include "Animation.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animation Example");
    window.setFramerateLimit(60);

    // Load background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Ruaysus2.png")) {
        std::cerr << "Failed to load background.png" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Idle Animations (เล็กลงครึ่งหนึ่ง)
    Animation idle1("idleaa.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation idle2("idlez.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });

    // Attack Animations
    Animation attack2("attackz.png", 1600, 1200, 9, 0.1f, { 0.5f, 0.5f });

    // Set ตำแหน่ง
    idle1.setPosition(0, 200);
    idle2.setPosition(400, 200);
    attack2.setPosition(0, 0);

    bool isAttacking1 = false, isAttacking2 = false;

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::E && !isAttacking1 && !isAttacking2) {
                    isAttacking2 = true;
                    attack2.reset();
                }
            }
        }

        // Update animations
        if (isAttacking2) {
            attack2.update(deltaTime);
            if (attack2.getSprite().getTextureRect().left == (9 - 1) * 1600) {
                isAttacking2 = false;
            }
        }
        else {
            // ✅ อัปเดต idle ตลอดถ้าไม่มีการโจมตี
            idle1.update(deltaTime);
            idle2.update(deltaTime);
        }

        // Render
        window.clear();
        window.draw(backgroundSprite); // Draw background first

        if (!isAttacking1 && !isAttacking2) {
            // ✅ ถ้าไม่มีตัวไหนกำลังโจมตี → แสดง idle
            window.draw(idle1.getSprite());
            window.draw(idle2.getSprite());
        }
        else if (isAttacking2) {
            // ✅ ถ้า sprite2 กำลังโจมตี → แสดง attack2 เท่านั้น
            window.draw(attack2.getSprite());
        }

        window.display();
    }

    return 0;
}