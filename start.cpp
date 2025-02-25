#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>

int main()
{
    // สร้างหน้าต่าง
    sf::RenderWindow window(sf::VideoMode(800, 600), "Slot Machine Game Start");

    // โหลดภาพพื้นหลัง
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background1.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    // สร้าง Sprite สำหรับพื้นหลัง
    sf::Sprite background(backgroundTexture);

    // ตั้งค่าจุดอ้างอิงของ Sprite ให้อยู่ที่กลางภาพ
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);

    // ตั้งตำแหน่งของภาพพื้นหลังให้อยู่กลางหน้าต่าง
    background.setPosition(window.getSize().x / 1.9, window.getSize().y / 2.3);

    // โหลดปุ่มเริ่มเกม
    sf::Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("startbutton.png")) {
        std::cerr << "Failed to load start button image!" << std::endl;
        return -1;
    }
    sf::Sprite startButton(startButtonTexture);

    // ตั้งตำแหน่งของปุ่มเริ่มเกมให้อยู่กลางหน้าจอ
    float buttonCenterX = window.getSize().x / 2;
    float buttonCenterY = window.getSize().y / 2;
    startButton.setPosition(buttonCenterX - startButton.getLocalBounds().width + 250,
        buttonCenterY - startButton.getLocalBounds().height + 300);

    // ตั้งจุดอ้างอิง (origin) ของปุ่มให้เป็นกลางของปุ่ม
    startButton.setOrigin(startButton.getLocalBounds().width / 2, startButton.getLocalBounds().height / 2);

    // สร้างนาฬิกาสำหรับจับเวลา
    sf::Clock clock;

    sf::Vector2f originalScale(1.f, 1.f);
    sf::Vector2f hoverScale(1.2f, 1.2f); // ขยาย 20% เมื่อ Hover

    sf::Texture cloude;
    if (!cloude.loadFromFile("cloude2.png")) {
        std::cerr << "Failed to load image" << std::endl;
        return -1;
    }

    sf::Sprite cloud(cloude);
    float buttonCenterXc = window.getSize().x / 2.55;
    float buttonCenterYc = window.getSize().y / 2.8;
    cloud.setPosition(buttonCenterXc - cloud.getLocalBounds().width + 250,
        buttonCenterYc - cloud.getLocalBounds().height + 300);


    // ลูปหลักของเกม
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // ตรวจสอบการคลิกที่ปุ่มเริ่มเกม
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                startButton.setScale(hoverScale);  // ใช้ startButton แทน startButtonTexture
            }
            else {
                startButton.setScale(originalScale);  // ใช้ startButton แทน startButtonTexture
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::FloatRect buttonBounds = startButton.getGlobalBounds();
                if (buttonBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                    // เมื่อคลิกปุ่มเริ่มเกม
                    std::cerr << "Left click Start" << std::endl;
                    // สามารถใส่โค้ดสำหรับการเริ่มเกมที่นี่
                }
            }
        }

        // คำนวณค่า scale จากเวลาโดยใช้ sine wave เพื่อให้ขยายและหด
        float time = clock.getElapsedTime().asSeconds();
        float scaleFactor = 1.1f + 0.05f * std::sin(time * 3);  // ใช้ sine wave ทำให้ขยายและหด

        // ปรับขนาดพื้นหลังให้พอดีหน้าต่าง พร้อมขยายและหด
        background.setScale(
            static_cast<float>(window.getSize().x) / background.getLocalBounds().width * scaleFactor,
            static_cast<float>(window.getSize().y) / background.getLocalBounds().height * scaleFactor
        );

        cloud.setScale(
            static_cast<float>(window.getSize().x) / cloud.getLocalBounds().width,
            static_cast<float>(window.getSize().y) / cloud.getLocalBounds().height
        );



        // วาดทุกอย่าง
        window.clear();
        window.draw(background);  // วาดพื้นหลัง
        window.draw(cloud);
        window.draw(startButton);  // วาดปุ่มเริ่มเกม
        window.display();
    }

    return 0;
}