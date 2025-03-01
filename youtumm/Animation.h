#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Animation {
public:
    Animation(const std::string& filePath, int frameWidth, int frameHeight, int frameCount, float switchTime, sf::Vector2f scale);

    void update(float deltaTime);
    void reset();
    void setPosition(float x, float y);
    void setScale(float x, float y); // ✅ เพิ่มฟังก์ชันตั้งค่าขนาด
    sf::Sprite& getSprite();

private:
    sf::Texture texture, backgroundTexture;
    sf::Sprite sprite;
    sf::IntRect uvRect;
    int frameCount;
    float switchTime;
    float totalTime;
    int currentFrame;
};

#endif
