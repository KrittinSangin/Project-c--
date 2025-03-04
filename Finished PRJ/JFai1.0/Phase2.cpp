#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
using namespace sf;

enum Choices { SCISSORS = 1, PAPER, ROCK };

const std::vector<std::string> skills = {
    "HEALPASSIVE",
    "CHIP_BOOST",
    "DEFENSE_BOOST",
    "CRITICAL_ATTACK",
    "ATTACK_BOOST",
    "SHIELD",
    "BUFF_X2",
    "HEALACTIVE"
};


void readAndCompareSkills(const std::string& filename, int compare[8]) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    
    std::string skill;
    while (std::getline(inFile, skill)) {
        for (size_t i = 0; i < skills.size(); ++i) {
            if (skill == skills[i]) {
                compare[i] = 1;
                break;
            }
        }
    }
    inFile.close();
}

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
            segment.setFillColor(sf::Color(255, 0, 0)); // Red
            segment.setPosition(x + i * segmentWidth, y);
            segments.push_back(segment);
        }

        if (!font.loadFromFile("verdana.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
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
                segments[i].setFillColor(sf::Color(255, 0, 0)); // Red
            }
            else {
                segments[i].setFillColor(sf::Color(50, 50, 50)); // Gray
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

class SkillButton {
public:
    SkillButton(const sf::Texture& texture, float x, float y, float radius, int skillIndex, int skillLimit)
        : skillIndex(skillIndex), skillLimit(skillLimit) {
        // สร้างปุ่มวงกลม
        button.setRadius(radius);
        button.setPosition(x, y);
        button.setFillColor(sf::Color(100, 100, 200));
        button.setOutlineColor(sf::Color::White);
        button.setOutlineThickness(2);

        // ตั้งค่าข้อมูลรูปสกิล
        sprite.setTexture(texture);

        // ปรับขนาดรูปภาพให้พอดีกับปุ่มวงกลม
        float scale = (radius * 2) / std::max(texture.getSize().x, texture.getSize().y);
        sprite.setScale(scale, scale);

        // จัดตำแหน่งรูปภาพให้อยู่กลางปุ่มวงกลม
        sprite.setPosition(x + radius - (texture.getSize().x * scale) / 2,
            y + radius - (texture.getSize().y * scale) / 2);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(button);
        window.draw(sprite);
    }

    bool isClicked(int mouseX, int mouseY) const {
        return button.getGlobalBounds().contains(mouseX, mouseY);
    }

    int getSkillIndex() const {
        return skillIndex;
    }

    int getSkillLimit() const {
        return skillLimit;
    }

    void useSkill() {
        if (skillLimit > 0) {
            skillLimit--;
        }
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

private:
    sf::CircleShape button;
    sf::Sprite sprite;
    int skillIndex; // ดัชนีสกิล
    int skillLimit; // จำนวนครั้งที่สามารถใช้สกิลได้
};

std::vector<sf::Texture> loadSkillIcons(const std::vector<std::string>& iconPaths) {
    std::vector<sf::Texture> skillTextures;
    for (const auto& path : iconPaths) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            std::cerr << "Failed to load " << path << std::endl;
            continue; // ข้ามไปหากโหลดไม่สำเร็จ
        }
        skillTextures.push_back(texture);
    }
    return skillTextures;
}

void showGameOverScreenwin(RenderWindow& window) {
    // โหลดฟอนต์
    Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        cerr << "Error: ไม่สามารถโหลดฟอนต์ 'verdana.ttf' ได้! กรุณาตรวจสอบไฟล์." << endl;
        system("pause");
        return;
    }

    // โหลดพื้นหลัง
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background win.jpg")) {
        cerr << "Error: ไม่สามารถโหลดรูป 'background.jpg' ได้! กรุณาตรวจสอบไฟล์." << endl;
        system("pause");
        return;
    }
    Sprite backgroundSprite(backgroundTexture);

    // ข้อความ You Win!!!
    Text gameOverText("You Win!!!", font, 100);
    gameOverText.setFillColor(Color::Yellow);
    gameOverText.setPosition(150, 100);

    // ปุ่ม Next Episode
    RectangleShape playAgainBox(Vector2f(200, 50));
    playAgainBox.setFillColor(Color::Black);
    playAgainBox.setPosition(305, 270);

    Text playAgainText("Play Again", font, 30);
    playAgainText.setFillColor(Color::White);
    playAgainText.setPosition(305, 275);

    // ปุ่ม Exit
    RectangleShape exitBox(Vector2f(200, 50));
    exitBox.setFillColor(Color::Black);
    exitBox.setPosition(305, 350);

    Text exitText("Exit", font, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(375, 355);

    while (window.isOpen()) {
        Event event;
        Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        // ตรวจจับเมาส์โฮเวอร์
        playAgainBox.setFillColor(playAgainBox.getGlobalBounds().contains(mousePos) ? Color::Yellow : Color::Black);
        exitBox.setFillColor(exitBox.getGlobalBounds().contains(mousePos) ? Color::Yellow : Color::Black);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (playAgainBox.getGlobalBounds().contains(mousePos)) {
                    cout << "Play Again..." << endl;
                    return;
                }
                else if (exitBox.getGlobalBounds().contains(mousePos)) {
                    cout << "Exiting game..." << endl;
                    window.close();
                }
            }
        }

        // วาด UI
        window.clear();
        window.draw(backgroundSprite); // วาดพื้นหลัง
        window.draw(gameOverText);
        window.draw(playAgainBox);
        window.draw(playAgainText);
        window.draw(exitBox);
        window.draw(exitText);
        window.display();
    }
}

void showGameOverScreenlose(RenderWindow& window) {
    // โหลดฟอนต์
    Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        cerr << "Error: ไม่สามารถโหลดฟอนต์ 'verdana.ttf' ได้! กรุณาตรวจสอบไฟล์." << endl;
        system("pause");
        return;
    }

    // โหลดพื้นหลัง
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) { // เปลี่ยนชื่อไฟล์ตามที่คุณมี
        cerr << "Error: ไม่สามารถโหลดรูป 'background.jpg' ได้! กรุณาตรวจสอบไฟล์." << endl;
        system("pause");
        return;
    }
    Sprite backgroundSprite(backgroundTexture);

    // ข้อความ You Lose
    Text gameOverText("You Lose", font, 100);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(205, 100);

    // ปุ่ม Play Again
    RectangleShape playAgainBox(Vector2f(200, 50));
    playAgainBox.setFillColor(Color::Black);
    playAgainBox.setPosition(305, 270);

    Text playAgainText("Play Again", font, 30);
    playAgainText.setFillColor(Color::White);
    playAgainText.setPosition(330, 275);

    // ปุ่ม Exit
    RectangleShape exitBox(Vector2f(200, 50));
    exitBox.setFillColor(Color::Black);
    exitBox.setPosition(305, 350);

    Text exitText("Exit", font, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(375, 355);

    while (window.isOpen()) {
        Event event;
        Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        // ตรวจจับเมาส์โฮเวอร์
        playAgainBox.setFillColor(playAgainBox.getGlobalBounds().contains(mousePos) ? Color::Red : Color::Black);
        exitBox.setFillColor(exitBox.getGlobalBounds().contains(mousePos) ? Color::Red : Color::Black);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (playAgainBox.getGlobalBounds().contains(mousePos)) {
                    cout << "Play Again..." << endl;
                    return;
                }
                else if (exitBox.getGlobalBounds().contains(mousePos)) {
                    cout << "Exiting game..." << endl;
                    window.close();
                }
            }
        }

        // วาด UI
        window.clear();
        window.draw(backgroundSprite); // วาดพื้นหลัง
        window.draw(gameOverText);
        window.draw(playAgainBox);
        window.draw(playAgainText);
        window.draw(exitBox);
        window.draw(exitText);
        window.display();
    }
}

int main() {

    bool shield = false;
    bool x2 = false;


    srand(static_cast<unsigned int>(time(0)));
    int compare[8] = {0}; // Initialize all to 0
    readAndCompareSkills("SkillMem.txt", compare);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Spin Game with HP Bar and Animations");
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // โหลดรูปสกิล
    std::vector<std::string> iconPaths = { "skill1.jpg", "skill2.jpg", "skill3.jpg", "skill4.jpg" };
    std::vector<sf::Texture> skillTextures = loadSkillIcons(iconPaths);

    // สร้างปุ่มสกิล
    std::vector<SkillButton> skillButtons;
    for (size_t i = 0; i < skillTextures.size(); ++i) {
        skillButtons.emplace_back(skillTextures[i], 25 + i * 100, 530, 30, i, 1);
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
    vector<int> spins(6, 1);  // Default to Scissors
    vector<bool> stopped(6, false); // Track if spinning is stopped
    vector<float> positionsX = { 62.5, 162.5, 262.5, 437.5, 537.5, 637.5 };

    // Result of slot 3
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
    HPBar playerHPBar(50, 500, 300, 20, 100+compare[0]*20+compare[2]*10, 10); // Player HP Bar
    HPBar bossHPBar(450, 500, 300, 20, 100, 10);  // Boss HP Bar

    // เก็บลำดับการเลือกสกิล
    std::vector<int> selectedSkills; // ลำดับการเลือกสกิล

    // เก็บรูปสกิลที่เลือกไว้บนหัวสล็อต
    std::vector<sf::Sprite> slotSkillSprites(3); // สกิลที่เลือกไว้สำหรับช่อง 1, 2, 3

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

    // จำกัดการ Spin และการใช้สกิล
    int spinLimit = 15+compare[1]*6; // จำนวนครั้งที่สามารถ Spin ได้

    while (window.isOpen()) {
        float deltaTime = animationClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (spinLimit > 0) { // ตรวจสอบว่ายังสามารถ Spin ได้
                        spinning = true;
                        currentPair = 0;
                        spinClock.restart();
                        stopClock.restart();
                        resultText.setString("");
                        fill(stopped.begin(), stopped.end(), false); // Reset stopped flags
                        spinLimit--; // ลดจำนวนครั้งที่เหลือ
                    }
                    else {
                        resultText.setString("No more spins left!");
                        showGameOverScreenlose(window);
                        break; // ออกจากเกม
                        //handle lose screen
                    }
                }

                // ตรวจจับการคลิกปุ่มสกิล
                for (size_t i = 0; i < skillButtons.size(); ++i) {
                    if (skillButtons[i].isClicked(event.mouseButton.x, event.mouseButton.y)) {
                        if (selectedSkills.size() < 3 && skillButtons[i].getSkillLimit() > 0) { // เลือกได้สูงสุด 3 สกิล และยังมีสกิลเหลือ
                            selectedSkills.push_back(i); // เพิ่มสกิลที่เลือกเข้าไปในลำดับ
                            skillButtons[i].useSkill(); // ลดจำนวนครั้งที่เหลือ
                            std::cout << "Skill " << i + 1 << " selected for slot " << selectedSkills.size() << std::endl;

                            // แสดงรูปสกิลที่เลือกไว้บนหัวสล็อต
                            if (selectedSkills.size() <= 3) {
                                slotSkillSprites[selectedSkills.size() - 1] = skillButtons[i].getSprite();
                                float skillX = positionsX[selectedSkills.size() - 1] + 50 - (skillButtons[i].getSprite().getGlobalBounds().width / 2);
                                float skillY = 40 - (skillButtons[i].getSprite().getGlobalBounds().height / 2);
                                slotSkillSprites[selectedSkills.size() - 1].setPosition(skillX, skillY);
                            }
                        }
                        else {
                            std::cout << "No more skills left for skill " << i + 1 << "!" << std::endl;
                        }
                    }
                }
            }
        }

        if (spinning) {
            float elapsed = spinClock.getElapsedTime().asSeconds();

            // Spin the boxes
            for (int i = 0; i < 6; i++) {
                if (!stopped[i]) {
                    spins[i] = spinChoice();
                    updateText(positionsX[i], 75, texts[i], spins[i], font);
                }
            }

            // Stop the first pair after 1.5 seconds
            if (elapsed > 1.5 && currentPair == 0) {
                stopped[0] = stopped[3] = true; // Stop boxes 1-4
                string result = compareSpins(spins[0], spins[3]);
                resultText.setString("Slot 1 " + result);
                if (result == "Player Loses!") {
                    if (compare[5] == 0) {  // If Shield is NOT active
                        float damage = 10;
                        playerHPBar.setHP(playerHPBar.getHP() - damage);
                    } else {
                        std::cout << "Shield absorbed the attack!" << std::endl;
                    }
                    isAttacking2 = true; // Trigger boss attack animation
                    attack2.reset();

                    // ใช้สกิลของบอสช่อง 1
                    std::cout << "Boss uses skill 1 on slot 1" << std::endl;
                }
                else if (result == "Player Wins!") {
                    if (selectedSkills.size() > 0) {
                        std::cout << "Using skill " << selectedSkills[0] + 1 << " for slot 1" << std::endl;
                        switch (selectedSkills[0]) {
                        case 0:
                            // ทำงานสกิล 1 (เช่น ลด HP บอส)
                            bossHPBar.setHP(bossHPBar.getHP() - 20);
                            break;
                        case 1:
                            // ทำงานสกิล 2 (เช่น เพิ่ม HP ผู้เล่น)
                            playerHPBar.setHP(playerHPBar.getHP() + 10);
                            break;
                        case 2:
                            // ทำงานสกิล 3
                            break;
                        case 3:
                            // ทำงานสกิล 4
                            break;
                        }
                    }
                    float damage = 10 + (5 * compare[3]); // Base damage + Critical Attack bonus
if (compare[6] == 1) damage *= 2; // Apply Buff_X2 effect

bossHPBar.setHP(bossHPBar.getHP() - damage);
                }
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 3.0 && currentPair == 1) {
                stopped[1] = stopped[4] = true; // Stop boxes 2-5
                string result = compareSpins(spins[1], spins[4]);
                resultText.setString("Slot 2 " + result);
                if (result == "Player Loses!") {
                    if (compare[5] == 0) {  // If Shield is NOT active
                        float damage = 10;
                        playerHPBar.setHP(playerHPBar.getHP() - damage);
                    } else {
                        std::cout << "Shield absorbed the attack!" << std::endl;
                    }
                    isAttacking2 = true; // Trigger boss attack animation
                    attack2.reset();

                    // ใช้สกิลของบอสช่อง 2
                    std::cout << "Boss uses skill 2 on slot 2" << std::endl;
                }
                else if (result == "Player Wins!") {
                    if (selectedSkills.size() > 1) {
                        std::cout << "Using skill " << selectedSkills[1] + 1 << " for slot 2" << std::endl;
                        switch (selectedSkills[1]) {
                        case 0:
                            // ทำงานสกิล 1 (เช่น ลด HP บอส)
                            bossHPBar.setHP(bossHPBar.getHP() - 20);
                            break;
                        case 1:
                            // ทำงานสกิล 2 (เช่น เพิ่ม HP ผู้เล่น)
                            playerHPBar.setHP(playerHPBar.getHP() + 10);
                            break;
                        case 2:
                            // ทำงานสกิล 3
                            break;
                        case 3:
                            // ทำงานสกิล 4
                            break;
                        }
                    }
                    float damage = 10 + (5 * compare[3]); // Base damage + Critical Attack bonus
if (compare[6] == 1) damage *= 2; // Apply Buff_X2 effect

bossHPBar.setHP(bossHPBar.getHP() - damage);
                }
                currentPair++;
                stopClock.restart();
            }
            else if (elapsed > 4.5 && currentPair == 2) {
                stopped[2] = stopped[5] = true; // Stop boxes 3-6
                string result = compareSpins(spins[2], spins[5]);
                resultText.setString("Slot 3 " + result);
                if (result == "Player Loses!") {
                    
                    if (compare[5] == 0) {  // If Shield is NOT active
                        float damage = 10;
                        playerHPBar.setHP(playerHPBar.getHP() - damage);
                    } else {
                        std::cout << "Shield absorbed the attack!" << std::endl;
                    } // Decrease player HP
                    isAttacking2 = true; // Trigger boss attack animation
                    attack2.reset();

                    // ใช้สกิลของบอสช่อง 3
                    std::cout << "Boss uses skill 3 on slot 3" << std::endl;
                }
                else if (result == "Player Wins!") {
                    if (selectedSkills.size() > 2) {
                        std::cout << "Using skill " << selectedSkills[2] + 1 << " for slot 3" << std::endl;
                        switch (selectedSkills[2]) {
                        case 0:
                            // ทำงานสกิล 1 (เช่น ลด HP บอส)
                            bossHPBar.setHP(bossHPBar.getHP() - 20);
                            break;
                        case 1:
                            // ทำงานสกิล 2 (เช่น เพิ่ม HP ผู้เล่น)
                            playerHPBar.setHP(playerHPBar.getHP() + 10);
                            break;
                        case 2:
                            // ทำงานสกิล 3

                            break;
                        case 3:
                            // ทำงานสกิล 4
                            break;
                        }
                    }
                    float damage = 10 + (5 * compare[3]); // Base damage + Critical Attack bonus
if (compare[6] == 1) damage *= 2; // Apply Buff_X2 effect

bossHPBar.setHP(bossHPBar.getHP() - damage);
                }
                currentPair++;
                spinning = false;

                // รีเซ็ตการเลือกสกิลเมื่อจบเทิร์น
                selectedSkills.clear();
                for (auto& sprite : slotSkillSprites) {
                    sprite = sf::Sprite(); // ล้างรูปสกิลที่แสดงบนหัวสล็อต
                }
            }
        }

        // Check boss phase transition
        if (!phaseTransition) {
            if (bossHPBar.getHP() <= 0 && bossPhase == 1) {
                bossHPBar.setHP(bossHPBar.getHP() + 100);
                bossPhase = 2;
                phaseTransition = true;
                phaseText.setString("Boss Phase 2");
                phaseClock.restart();
                backgroundSprite.setTexture(backgroundTexture2);
                idle2 = idle2_2;
                attack2 = attack2_2;
                // Reset positions for phase 2
                idle2.setPosition(400, 180);
                attack2.setPosition(0, 0);
            }
            else if (bossHPBar.getHP() <= 0 && bossPhase == 2) {
                bossHPBar.setHP(bossHPBar.getHP() + 100);
                bossPhase = 3;
                phaseTransition = true;
                phaseText.setString("Boss Final Phase");
                phaseClock.restart();
                backgroundSprite.setTexture(backgroundTexture3);
                idle2 = idle2_3;
                attack2 = attack2_3;
                // Reset positions for phase 3
                idle2.setPosition(400, 180);
                attack2.setPosition(0, 0);
            }
            else if (bossHPBar.getHP() <= 0 && bossPhase == 3) {
                // เมื่อบอสตายในเฟสสุดท้าย
                showGameOverScreenwin(window);
                break; // ออกจากเกม
            }
            else if (playerHPBar.getHP() <= 0 or spinLimit == 0) {
                // เมื่อบอสตายในเฟสสุดท้าย
                showGameOverScreenlose(window);
                break; // ออกจากเกม
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
                isAttacking2 = false; // Stop attack animation
            }
        }
        else {
            idle1.update(deltaTime);
            idle2.update(deltaTime);
        }

        // Draw everything
        window.clear();
        window.draw(backgroundSprite); // Draw background

        if (!isAttacking1 && !isAttacking2) {
            window.draw(idle1.getSprite()); // Draw idle1 animation
            window.draw(idle2.getSprite()); // Draw idle2 animation
        }
        for (int i = 0; i < 6; i++) window.draw(boxes[i]); // Draw spin boxes
        for (int i = 0; i < 6; i++) window.draw(texts[i]); // Draw spin texts
        window.draw(button); // Draw spin button
        window.draw(buttonText); // Draw spin button text
        window.draw(resultText); // Draw result text

        // Draw attack animation if active
        if (isAttacking2) {
            window.draw(attack2.getSprite());
        }

        // Draw HP Bars
        playerHPBar.draw(window);
        bossHPBar.draw(window);

        // Draw skill buttons
        for (auto& button : skillButtons) {
            button.draw(window);
        }

        // Draw selected skill icons on slots
        for (size_t i = 0; i < slotSkillSprites.size(); ++i) {
            window.draw(slotSkillSprites[i]);
        }

        window.display();
    }

    return 0;
}