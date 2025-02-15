#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>

int main()
{
    // ���ҧ˹�ҵ�ҧ
    sf::RenderWindow window(sf::VideoMode(800, 600), "Slot Machine Game Start");

    // ��Ŵ�Ҿ�����ѧ
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background1.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    // ���ҧ Sprite ����Ѻ�����ѧ
    sf::Sprite background(backgroundTexture);

    // ��駤�Ҩش��ҧ�ԧ�ͧ Sprite ����������ҧ�Ҿ
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);

    // ��駵��˹觢ͧ�Ҿ�����ѧ��������ҧ˹�ҵ�ҧ
    background.setPosition(window.getSize().x / 1.9, window.getSize().y / 2.3);

    // ��Ŵ�����������
    sf::Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("startbutton.png")) {
        std::cerr << "Failed to load start button image!" << std::endl;
        return -1;
    }
    sf::Sprite startButton(startButtonTexture);

    // ��駵��˹觢ͧ�������������������ҧ˹�Ҩ�
    float buttonCenterX = window.getSize().x / 2;
    float buttonCenterY = window.getSize().y / 2;
    startButton.setPosition(buttonCenterX - startButton.getLocalBounds().width + 250,
        buttonCenterY - startButton.getLocalBounds().height + 300);

    // ��駨ش��ҧ�ԧ (origin) �ͧ��������繡�ҧ�ͧ����
    startButton.setOrigin(startButton.getLocalBounds().width / 2, startButton.getLocalBounds().height / 2);

    // ���ҧ���ԡ�����Ѻ�Ѻ����
    sf::Clock clock;

    sf::Vector2f originalScale(1.f, 1.f);
    sf::Vector2f hoverScale(1.2f, 1.2f); // ���� 20% ����� Hover

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


    // �ٻ��ѡ�ͧ��
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // ��Ǩ�ͺ��ä�ԡ�������������
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                startButton.setScale(hoverScale);  // �� startButton ᷹ startButtonTexture
            }
            else {
                startButton.setScale(originalScale);  // �� startButton ᷹ startButtonTexture
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::FloatRect buttonBounds = startButton.getGlobalBounds();
                if (buttonBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                    // ����ͤ�ԡ�����������
                    std::cerr << "Left click Start" << std::endl;
                    // ����ö���������Ѻ���������������
                }
            }
        }

        // �ӹǳ��� scale �ҡ�������� sine wave �������������˴
        float time = clock.getElapsedTime().asSeconds();
        float scaleFactor = 1.1f + 0.05f * std::sin(time * 3);  // �� sine wave �����������˴

        // ��Ѻ��Ҵ�����ѧ���ʹ�˹�ҵ�ҧ ������������˴
        background.setScale(
            static_cast<float>(window.getSize().x) / background.getLocalBounds().width * scaleFactor,
            static_cast<float>(window.getSize().y) / background.getLocalBounds().height * scaleFactor
        );

        cloud.setScale(
            static_cast<float>(window.getSize().x) / cloud.getLocalBounds().width,
            static_cast<float>(window.getSize().y) / cloud.getLocalBounds().height
        );



        // �Ҵ�ء���ҧ
        window.clear();
        window.draw(background);  // �Ҵ�����ѧ
        window.draw(cloud);
        window.draw(startButton);  // �Ҵ�����������
        window.display();
    }

    return 0;
}