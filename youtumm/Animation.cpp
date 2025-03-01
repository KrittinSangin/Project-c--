#include "Animation.h"

Animation::Animation(const std::string& filePath, int frameWidth, int frameHeight, int frameCount, float switchTime, sf::Vector2f scale)
    : frameCount(frameCount), switchTime(switchTime), totalTime(0.f), currentFrame(0) {
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    uvRect.width = frameWidth;
    uvRect.height = frameHeight;
    sprite.setTextureRect(uvRect);
    sprite.setScale(scale);
}

void Animation::update(float deltaTime) {
    totalTime += deltaTime;
    if (totalTime >= switchTime) {
        totalTime -= switchTime;
        currentFrame = (currentFrame + 1) % frameCount;
    }
    uvRect.left = currentFrame * uvRect.width;
    sprite.setTextureRect(uvRect);
}

void Animation::reset() {
    currentFrame = 0;
    totalTime = 0.f;
    uvRect.left = 0;
    sprite.setTextureRect(uvRect);
}

void Animation::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Animation::setScale(float x, float y) { // ✅ เพิ่มการตั้งค่าสเกล
    sprite.setScale(x, y);
}

sf::Sprite& Animation::getSprite() {
    return sprite;
}