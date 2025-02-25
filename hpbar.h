#include <SFML/Graphics.hpp>
#include <sstream>

class HPBar {
private:
    sf::RectangleShape background;
    std::vector<sf::RectangleShape> segments;
    sf::Font font;
    sf::Text hpText;
    sf::Text hpPercentageText;
    float maxHP;
    float currentHP;
    int numSegments;
    float segmentWidth;

public:
    HPBar(float x, float y, float width, float height, float maxHP, int numSegments) {
        this->maxHP = maxHP;
        this->currentHP = maxHP;
        this->numSegments = numSegments;
        this->segmentWidth = width / numSegments;

        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(50, 50, 50));
        background.setPosition(x, y);

        for (int i = 0; i < numSegments; ++i) {
            sf::RectangleShape segment(sf::Vector2f(segmentWidth - 2, height));
            segment.setFillColor(sf::Color(255, 0, 0)); // ��ᴧ
            segment.setPosition(x + i * segmentWidth, y);
            segments.push_back(segment);
        }

        if (!font.loadFromFile("verdana.ttf")) {
            // Handle error
        }
        hpText.setFont(font);
        hpText.setString("HP");
        hpText.setCharacterSize(18);
        hpText.setFillColor(sf::Color::White);
        hpText.setPosition(x - 40, y);

        hpPercentageText.setFont(font);
        hpPercentageText.setCharacterSize(18);
        hpPercentageText.setFillColor(sf::Color::White);
        hpPercentageText.setPosition(x + width + 10, y);
        updateHPText();
    }

    void setHP(float hp) {
        currentHP = std::max(0.0f, std::min(hp, maxHP));
        int activeSegments = static_cast<int>((currentHP / maxHP) * numSegments);

        for (int i = 0; i < numSegments; ++i) {
            if (i < activeSegments) {
                segments[i].setFillColor(sf::Color(255, 0, 0)); // ��ᴧ
            }
            else {
                segments[i].setFillColor(sf::Color(50, 50, 50)); // �������
            }
        }
        updateHPText();
    }

    void updateHPText() {
        std::ostringstream ss;
        ss << static_cast<int>((currentHP / maxHP) * 100) << "%";
        hpPercentageText.setString(ss.str());
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        for (auto& segment : segments) {
            window.draw(segment);
        }
        window.draw(hpText);
        window.draw(hpPercentageText);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "HP Bar Example");
    HPBar hpBar(100, 50, 300, 20, 100, 100); // 10 segments

    float hp = 100;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            hp -= 0.1; // Ŵ HP ������١��ŧ�١��
            hpBar.setHP(hp);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            hp += 0.1; // ���� HP ������١�â�鹶١��
            hpBar.setHP(hp);
        }

        window.clear();
        hpBar.draw(window);
        window.display();
    }

    return 0;
}
#pragma once
