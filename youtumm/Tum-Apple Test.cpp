#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

enum Choices { SCISSORS = 1, PAPER, ROCK };

string getChoiceName(int choice) {
    switch (choice) {
    case SCISSORS: return "Scissors";
    case PAPER: return "Paper";
    case ROCK: return "Rock";
    default: return "Invalid";
    }
}

int spinChoice() {
    return rand() % 3 + 1;
}

// เปรียบเทียบ 2 ช่อง
string compareSpins(int spin1, int spin2) {
    if (spin1 == spin2) return "Tie!";
    if ((spin1 == SCISSORS && spin2 == PAPER) ||
        (spin1 == PAPER && spin2 == ROCK) ||
        (spin1 == ROCK && spin2 == SCISSORS))
        return "Player Wins!";
    return "Player Loses!";
}

void createBox(sf::RectangleShape& box, float x, float y) {
    box.setSize(sf::Vector2f(100, 50));
    box.setPosition(x, y);
    box.setFillColor(sf::Color(225, 225, 250));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);
}

void updateText(float x, float y, sf::Text& text, int spin, sf::Font& font) {
    text.setString(getChoiceName(spin));
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(x + 50 - textBounds.width / 2, y + 15 - textBounds.height / 2);
}

// Animation Class
class Animation {
public:
    Animation(const std::string& filePath, int frameWidth, int frameHeight, int frameCount, float switchTime, sf::Vector2f scale);
    void update(float deltaTime);
    void reset();
    void setPosition(float x, float y);
    void setScale(float x, float y);
    sf::Sprite& getSprite();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::IntRect uvRect;
    int frameCount;
    float switchTime;
    float totalTime;
    int currentFrame;
};

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

void Animation::setScale(float x, float y) {
    sprite.setScale(x, y);
}

sf::Sprite& Animation::getSprite() {
    return sprite;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Spin Game with Animation");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Load background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Ruaysus2.png")) {
        std::cerr << "Failed to load background.png" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Animation setup
    Animation idle1("idleaa.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation idle2("idlez.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation attack2("attackz.png", 1600, 1200, 9, 0.1f, { 0.5f, 0.5f });

    idle1.setPosition(0, 200);
    idle2.setPosition(400, 200);
    attack2.setPosition(0, 0);

    bool isAttacking1 = false, isAttacking2 = false;

    // Spin game setup
    vector<sf::RectangleShape> boxes(6);
    vector<sf::Text> texts(6);
    vector<int> spins(6, 1);  // ค่าเริ่มต้นเป็น Scissors
    vector<bool> stopped(6, false); // ช่องไหนหยุดแล้ว
    vector<float> positionsX = { 62.5, 162.5, 262.5, 437.5, 537.5, 637.5 };

    // ตัวแปรเก็บผลลัพธ์ช่องที่ 3
    string slot3Result;

    for (int i = 0; i < 6; i++) {
        createBox(boxes[i], positionsX[i], 75);
        updateText(positionsX[i], 75, texts[i], spins[i], font);
    }

    sf::RectangleShape button;
    button.setSize(sf::Vector2f(200, 50));
    button.setPosition(300, 400);
    button.setFillColor(sf::Color::Green);

    sf::Text buttonText("Spin", font, 30);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(375, 410);

    sf::Text resultText("", font, 20);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition(300, 300);

    bool spinning = false;
    int currentPair = 0;
    sf::Clock spinClock, stopClock, animationClock;

    while (window.isOpen()) {
        float deltaTime = animationClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                spinning = true;
                currentPair = 0;
                spinClock.restart();
                stopClock.restart();
                resultText.setString("");
                fill(stopped.begin(), stopped.end(), false); // รีเซ็ตทุกช่องให้หมุน
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::E && !isAttacking1 && !isAttacking2 && slot3Result == "Player Loses!") {
                    isAttacking2 = true;
                    attack2.reset();
                }
            }
        }

        if (spinning) {
            float elapsed = spinClock.getElapsedTime().asSeconds();

            // หมุนทุกช่องที่ยังไม่หยุด
            for (int i = 0; i < 6; i++) {
                if (!stopped[i]) {
                    spins[i] = spinChoice();
                    updateText(positionsX[i], 75, texts[i], spins[i], font);
                }
            }

            // ค่อย ๆ หยุดทีละคู่
            if (elapsed > 1.5 && currentPair == 0) {
                stopped[0] = stopped[3] = true; // หยุดคู่ 1-4
                resultText.setString("Slot 1 " + compareSpins(spins[0], spins[3]));
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 3.0 && currentPair == 1) {
                stopped[1] = stopped[4] = true; // หยุดคู่ 2-5
                resultText.setString("Slot 2 " + compareSpins(spins[1], spins[4]));
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 4.5 && currentPair == 2) {
                stopped[2] = stopped[5] = true; // หยุดคู่ 3-6
                slot3Result = compareSpins(spins[2], spins[5]); // เก็บผลลัพธ์ช่องที่ 3
                resultText.setString("Slot 3 " + slot3Result);
                currentPair++;
                spinning = false;
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
            idle1.update(deltaTime);
            idle2.update(deltaTime);
        }

        // Draw everything
        window.clear();
        window.draw(backgroundSprite); // วาดพื้นหลัง
        if (!isAttacking1 && !isAttacking2) {
            window.draw(idle1.getSprite()); // วาดอนิเมชัน idle1
            window.draw(idle2.getSprite()); // วาดอนิเมชัน idle2
        }
        for (int i = 0; i < 6; i++) window.draw(boxes[i]); // วาดช่อง Spin
        for (int i = 0; i < 6; i++) window.draw(texts[i]); // วาดข้อความในช่อง Spin
        window.draw(button); // วาดปุ่ม Spin
        window.draw(buttonText); // วาดข้อความบนปุ่ม Spin
        window.draw(resultText); // วาดผลลัพธ์

        // วาดอนิเมชันโจมตีเป็นสิ่งสุดท้าย (อยู่บนสุด)
        if (isAttacking2) {
            window.draw(attack2.getSprite());
        }

        window.display();
    }

    return 0;
}