#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>

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
            segment.setFillColor(sf::Color(255, 0, 0)); // สีแดง
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
                segments[i].setFillColor(sf::Color(255, 0, 0)); // สีแดง
            }
            else {
                segments[i].setFillColor(sf::Color(50, 50, 50)); // สีเทา
            }
        }
        updateHPText();
    }

    float getHP() const {
        return currentHP;
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
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Spin Game with HP Bar and Animations");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Load background
    sf::Texture backgroundTexture1, backgroundTexture2, backgroundTexture3;
    if (!backgroundTexture1.loadFromFile("Ruaysus2.png")) {
        std::cerr << "Failed to load Ruaysus2.png" << std::endl;
        return -1;
    }
    if (!backgroundTexture2.loadFromFile("Metang2.png")) {
        std::cerr << "Failed to load background2.png" << std::endl;
        return -1;
    }
    if (!backgroundTexture3.loadFromFile("Slum2.png")) {
        std::cerr << "Failed to load background3.png" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture1);

    // Animation setup
    Animation idle1("idleaa.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation idle2("idlez.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation attack2("attackz.png", 1600, 1200, 9, 0.1f, { 0.5f, 0.5f });

    Animation idle2_2("idley.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation attack2_2("attacky.png", 1600, 1200, 9, 0.1f, { 0.5f, 0.5f });

    Animation idle2_3("idlex.png", 800, 600, 12, 0.1f, { 0.5f, 0.5f });
    Animation attack2_3("attackx.png", 1600, 1200, 9, 0.1f, { 0.5f, 0.5f });

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
    resultText.setPosition(350, 300);

    // HP Bar setup
    HPBar playerHPBar(50, 500, 300, 20, 100, 10); // HP Bar สำหรับผู้เล่น
    HPBar bossHPBar(450, 500, 300, 20, 100, 10);  // HP Bar สำหรับบอส

    bool spinning = false;
    int currentPair = 0;
    sf::Clock spinClock, stopClock, animationClock;

    // Phase transition variables
    bool phaseTransition = false;
    sf::Clock phaseClock;
    sf::RectangleShape blackScreen(sf::Vector2f(800, 600));
    blackScreen.setFillColor(sf::Color::Black);
    sf::Text phaseText("", font, 40);
    phaseText.setFillColor(sf::Color::White);
    phaseText.setPosition(250, 250);

    int bossPhase = 1;

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

            // ค่อย ๆ หยุดทีละคู่ (ลดเวลาในการหมุน)
            if (elapsed > 1.5 && currentPair == 0) { // จาก 1.5 เป็น 0.5
                stopped[0] = stopped[3] = true; // หยุดคู่ 1-4
                string result = compareSpins(spins[0], spins[3]);
                resultText.setString("Slot 1 " + result);
                if (result == "Player Loses!") {
                    playerHPBar.setHP(playerHPBar.getHP() - 10); // ผู้เล่นเสีย HP
                    isAttacking2 = true; // เล่นอนิเมชันโจมตี
                    attack2.reset();
                }
                else if (result == "Player Wins!") {
                    bossHPBar.setHP(bossHPBar.getHP() - 10); // บอสเสีย HP
                }
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 3.0 && currentPair == 1) { // จาก 3.0 เป็น 1.0
                stopped[1] = stopped[4] = true; // หยุดคู่ 2-5
                string result = compareSpins(spins[1], spins[4]);
                resultText.setString("Slot 2 " + result);
                if (result == "Player Loses!") {
                    playerHPBar.setHP(playerHPBar.getHP() - 10); // ผู้เล่นเสีย HP
                    isAttacking2 = true; // เล่นอนิเมชันโจมตี
                    attack2.reset();
                }
                else if (result == "Player Wins!") {
                    bossHPBar.setHP(bossHPBar.getHP() - 10); // บอสเสีย HP
                }
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 4.5 && currentPair == 2) { // จาก 4.5 เป็น 1.5
                stopped[2] = stopped[5] = true; // หยุดคู่ 3-6
                string result = compareSpins(spins[2], spins[5]);
                resultText.setString("Slot 3 " + result);
                if (result == "Player Loses!") {
                    playerHPBar.setHP(playerHPBar.getHP() - 10); // ผู้เล่นเสีย HP
                    isAttacking2 = true; // เล่นอนิเมชันโจมตี
                    attack2.reset();
                }
                else if (result == "Player Wins!") {
                    bossHPBar.setHP(bossHPBar.getHP() - 10); // บอสเสีย HP
                }
                currentPair++;
                spinning = false;
            }
        }

        // Check boss phase transition
        // ในส่วนที่เปลี่ยนเฟสบอส
        if (!phaseTransition) {
            if (bossHPBar.getHP() <= 70 && bossPhase == 1) {
                bossPhase = 2;
                phaseTransition = true;
                phaseText.setString("Boss Phase 2");
                phaseClock.restart();
                backgroundSprite.setTexture(backgroundTexture2);
                idle2 = idle2_2;
                attack2 = attack2_2;
                // กำหนดตำแหน่งใหม่ให้บอสตัวที่ 2
                idle2.setPosition(400, 180); // ตำแหน่งเดียวกับบอสตัวแรก
                attack2.setPosition(0, 0);
            }
            else if (bossHPBar.getHP() <= 40 && bossPhase == 2) {
                bossPhase = 3;
                phaseTransition = true;
                phaseText.setString("Boss Final Phase");
                phaseClock.restart();
                backgroundSprite.setTexture(backgroundTexture3);
                idle2 = idle2_3;
                attack2 = attack2_3;
                // กำหนดตำแหน่งใหม่ให้บอสตัวที่ 3
                idle2.setPosition(400, 180); // ตำแหน่งเดียวกับบอสตัวแรก
                attack2.setPosition(0, 0);
            }
        }

        // Update phase transition
        if (phaseTransition) {
            if (phaseClock.getElapsedTime().asSeconds() < 3.0f) {
                window.draw(blackScreen);
                window.draw(phaseText);
            }
            else {
                phaseTransition = false;
            }
        }

        // Update animations
        if (isAttacking2) {
            attack2.update(deltaTime);
            if (attack2.getSprite().getTextureRect().left == (9 - 1) * 1600) {
                isAttacking2 = false; // หยุดอนิเมชันเมื่อเล่นจบ
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

        // วาด HP Bar
        playerHPBar.draw(window);
        bossHPBar.draw(window);

        window.display();
    }

    return 0;
}