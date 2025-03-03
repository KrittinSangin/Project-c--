#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
using namespace std;

class Skill
{
public:
    string name;
    int skillcount;

    enum SkillType
    {
        HEALACTIVE,
        ATTACK_BOOST,
        STEAL_SKILL,
        SHIELD,
        CHIP_BOOST, // passive
        BUFF_X2,
        DEFENSE_BOOST,   // passive
        CRITICAL_ATTACK, // passive
        HEALPASSIVE      // passive
    } type;

    Skill(string n, SkillType t, int count = 0) : name(n), type(t), skillcount(count) {}
    void createskill(string s)
    {
    }
    string getName() const { return name; } // เพิ่ม getter สำหรับชื่อ
    void applySkill(class Player &p);       // ฟังก์ชันที่จะทำงานกับ Player class
    // ฟังก์ชันแสดงจำนวนสกิล
    void displaySkillCount(sf::RenderWindow &window, sf::Font &font, float x, float y)
    {
        if (skillcount > 0)
        {
            sf::Text countText;
            countText.setFont(font);
            countText.setString(to_string(skillcount));
            countText.setCharacterSize(20);
            countText.setFillColor(sf::Color::Red);
            countText.setStyle(sf::Text::Bold); // ทำให้ข้อความหนาขึ้น
            // จัดตำแหน่งข้อความให้อยู่ตรงกลางวงกลม
            sf::FloatRect textBounds = countText.getLocalBounds();
            countText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            countText.setPosition(x, y);
            window.draw(countText);
        }
    }
};

// สร้างกรอบวงกลม
sf::CircleShape createSkillCircle(float x, float y, float radius)
{
    sf::CircleShape circle(radius);                // ใช้วงกลมแทนกล่อง
    circle.setPosition(x - radius, y - radius);    // ตั้งตำแหน่งศูนย์กลางที่ (x, y)
    circle.setFillColor(sf::Color(204, 255, 255)); // สีฟ้า
    return circle;
}

// สร้างชื่อสกิล
sf::Text createSkillText(const std::string &name, sf::Font &font, float x, float y)
{
    sf::Text text;
    text.setFont(font);
    text.setString(name);
    text.setCharacterSize(13);
    text.setFillColor(sf::Color::White);
    text.setPosition(x - text.getLocalBounds().width / 2, y + 60); // ปรับให้ข้อความอยู่ใต้วงกลม
    return text;
}

// สร้างกรอบ sumskill
sf::RectangleShape createTitleBox(float windowWidth, float windowHeight)
{
    float boxWidth = 400;
    float boxHeight = 50;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition((windowWidth - boxWidth) / 2, 20);
    return box;
}

// สร้างข้อความ sumskill
sf::Text createTitleText(const std::string &title, sf::Font &font, float windowWidth)
{
    sf::Text text;
    text.setFont(font);
    text.setString(title);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(windowWidth / 2 - text.getLocalBounds().width / 2, 30);
    return text;
}

// สร้างกล่อง active
sf::RectangleShape createActiveBox(float windowWidth, float windowHeight)
{
    float boxWidth = 200;
    float boxHeight = 30;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition(50, windowHeight / 1.75 - boxHeight / 1.75);
    return box;
}

// creat message "Active"
sf::Text createActiveText(const std::string &word, sf::Font &font, float windowWidth, float activeBoxY)
{
    sf::Text text;
    text.setFont(font);
    text.setString(word);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition(50 + (200 / 2 - text.getLocalBounds().width / 2), activeBoxY + 5);
    return text;
}
// creat box "Passive"
sf::RectangleShape createPassiveBox(float windowWidth, float windowHeight)
{
    float boxWidth = 200;
    float boxHeight = 30;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition(50, windowHeight / 5.0 - boxHeight / 1.25);
    return box;
}

// creat message "Passive"
sf::Text createPassiveText(const std::string &word, sf::Font &font, float windowWidth, float PassiveBoxY)
{
    sf::Text text;
    text.setFont(font);
    text.setString(word);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition(50 + (200 / 2 - text.getLocalBounds().width / 2), PassiveBoxY + 5);
    return text;
}

/*// สร้างไอคอนล็อคสกิล ตอนนี้ทำเป็นรูปกากบาท อาจจะเอา UI ใส่ทีหลัง
// แต่ว่าฟังก์ชันนี้มันเรียกใช้ในใน main ได้ตอนแรก แต่ตำแหน่งไม่ตรง เลยแก้โค้ดใน main ไป ๆ มา ๆ
// ล่าสุด(26/02/2025) เรียกใช้ใน main ยังไม่ได้**
sf::VertexArray createLockCross(float x, float y, float radius)
{
    sf::VertexArray cross(sf::Lines, 4); // กากบาทมี 2 เส้น

    // ขนาดของเส้นตรงจะเป็นประมาณ 1/3 ของ radius
    float offset = radius / 3.0f;

    // กำหนดพิกัดของจุดเริ่มต้นและจุดสิ้นสุดของเส้นทั้งสอง
    cross[0].position = sf::Vector2f(x - offset, y - offset); // จุดเริ่มต้นของเส้นแรก
    cross[1].position = sf::Vector2f(x + offset, y + offset); // จุดสิ้นสุดของเส้นแรก

    cross[2].position = sf::Vector2f(x + offset, y - offset); // จุดเริ่มต้นของเส้นที่สอง
    cross[3].position = sf::Vector2f(x - offset, y + offset); // จุดสิ้นสุดของเส้นที่สอง

    // ตั้งสีให้เป็นสีแดง
    for (int i = 0; i < 4; i++)
    {
        cross[i].color = sf::Color::Red;
    }

    return cross;
}*/

// เพิ่มตัวแปร isUnlocked ในแต่ละสกิล
bool isUnlocked = false; // ปรับให้เป็น true เมื่อปลดล็อคแล้ว

// สกิลที่มีแล้วจะเป็นสีฟ้า สกิลที่ยังไม่มีจะเป็นสีเทา อันนี้ปรับสีได้
sf::CircleShape createSkillCircleWithLock(float x, float y, float radius, int skilcount)
{
    sf::CircleShape circle(radius);             // ใช้วงกลมแทนกล่อง
    circle.setPosition(x - radius, y - radius); // ตั้งตำแหน่งศูนย์กลางที่ (x, y)

    if (skilcount > 0)
    {
        circle.setFillColor(sf::Color(255, 192, 203)); // สีชมพู สำหรับสกิลที่ปลดล็อค
    }
    else
    {
        circle.setFillColor(sf::Color(150, 150, 150)); // สีเทาสำหรับสกิลที่ล็อค
    }

    return circle;
}

// creat tipbox
sf::RectangleShape createTooltipBox(float x, float y, int skillCount, sf::Font &font, sf::RenderWindow &window, float maxWidth)
{
    // แปลงจำนวนสกิลเป็นข้อความ
    std::string message = "Skill Count: " + std::to_string(skillCount);

    // แบ่งข้อความเป็นหลายบรรทัด (ถ้าจำเป็น)
    std::vector<std::string> lines;
    std::string currentLine;
    float lineWidth = 0.0f;

    for (size_t i = 0; i < message.size(); ++i)
    {
        currentLine += message[i];
        sf::Text tempText(currentLine, font, 15);
        lineWidth = tempText.getLocalBounds().width;

        if (lineWidth + 10 > maxWidth)
        {
            lines.push_back(currentLine.substr(0, currentLine.size() - 1));
            currentLine = currentLine.substr(currentLine.size() - 1);
        }
    }
    if (!currentLine.empty())
        lines.push_back(currentLine);

    // สร้างข้อความใหม่จากหลายบรรทัด
    std::vector<sf::Text> textLines;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        sf::Text text(lines[i], font, 15);
        text.setFillColor(sf::Color(255, 255, 255, 250)); // กำหนดสีของข้อความ
        text.setPosition(x - 12.5, y + (i * 20) - 40);
        textLines.push_back(text);
    }

    // คำนวณขนาดของกล่อง
    float height = lines.size() * 20; // 25px ต่อบรรทัด
    float width = maxWidth;

    // สร้างกล่องข้อความ
    sf::RectangleShape tooltip(sf::Vector2f(width, height));
    tooltip.setFillColor(sf::Color(153, 153, 255, 100));

    // ตรวจสอบตำแหน่งกล่องข้อความว่าตกขอบหรือไม่
    float newX = x - 17.5;
    float newY = y - 40;

    if (newX + width > window.getSize().x)
    {
        newX = window.getSize().x - width - 10; // เลื่อนไปที่ขอบขวา
    }
    if (newX < 10)
    {
        newX = 10; // เลื่อนไปที่ขอบซ้าย
    }
    if (newY < 10)
    {
        newY = 10; // เลื่อนขึ้นไปที่ขอบบน
    }
    if (newY + height > window.getSize().y)
    {
        newY = window.getSize().y - height - 10; // เลื่อนลงไปที่ขอบล่าง
    }

    tooltip.setPosition(newX, newY); // ตั้งตำแหน่งใหม่ให้กับกล่อง

    // วาดกล่องและข้อความ
    for (size_t i = 0; i < textLines.size(); ++i)
    {
        window.draw(textLines[i]); // วาดข้อความในกล่อง
    }

    return tooltip;
}

bool isCursorOverCircle(sf::CircleShape &circle, sf::Vector2i mousePos)
{
    // ตรวจสอบว่าตำแหน่งของเคอร์เซอร์อยู่ในวงกลมหรือไม่
    sf::FloatRect bounds = circle.getGlobalBounds();
    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
}

void updateLayout(float &spacingX, float &spacingY, float &radius, int cols, int rows, float windowWidth, float windowHeight, float startX, float startY)
{
    spacingX = (windowWidth - startX * 2) / cols;
    spacingY = (windowHeight - startY * 2) / (rows + 1);
    radius = std::min(spacingX, spacingY) / 2.0f;
}

// ฟังก์ชันสร้างปุ่มกากบาท (X)
sf::VertexArray createCloseIcon(float x, float y, float size)
{
    sf::VertexArray cross(sf::Lines, 4);

    cross[0].position = sf::Vector2f(x, y);
    cross[1].position = sf::Vector2f(x + size, y + size);

    cross[2].position = sf::Vector2f(x + size, y);
    cross[3].position = sf::Vector2f(x, y + size);

    for (int i = 0; i < 4; i++)
    {
        cross[i].color = sf::Color::Red; // สีแดง
    }

    return cross;
}

// ตรวจจับการคลิกที่ปุ่มกากบาท
bool isCloseIconClicked(sf::Vector2f mousePos, float x, float y, float size)
{
    return (mousePos.x >= x && mousePos.x <= x + size &&
            mousePos.y >= y && mousePos.y <= y + size);
}

// Cache สำหรับเก็บ texture
std::unordered_map<std::string, sf::Texture> textureCache;

// ฟังก์ชันโหลดรูปโดยใช้แค่ชื่อไฟล์
sf::Sprite loadImage(const std::string &skillName)
{
    std::string fileName = skillName + ".jpg"; // สร้างชื่อไฟล์จากชื่อสกิล
    if (textureCache.find(fileName) == textureCache.end())
    {
        // ถ้ายังไม่มี ให้โหลด
        sf::Texture texture;
        if (!texture.loadFromFile(fileName))
        {
            std::cerr << "Failed to load image: " << fileName << std::endl;
            return sf::Sprite(); // คืนค่า sprite เปล่าๆ ถ้าโหลดไม่ได้
        }
        textureCache[fileName] = std::move(texture); // เก็บ texture ใน cache
    }
    return sf::Sprite(textureCache[fileName]); // คืนค่า sprite โดยใช้ texture จาก cache
}

// ฟังก์ชันสร้าง sprite
sf::Sprite createSkillSprite(sf::Sprite &sprite, const sf::CircleShape &circle)
{
    // ตั้งค่าตำแหน่งของ sprite ให้พอดีกับตำแหน่งของ circle
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2); // จัดตำแหน่งกลาง
    sprite.setPosition(circle.getPosition().x + circle.getRadius(),
                       circle.getPosition().y + circle.getRadius());

    // คืนค่า sprite ที่ได้
    return sprite;
}

// ฟังก์ชันที่ใช้เปลี่ยนขนาดของ sprite ตามที่กำหนด
sf::Sprite resizeImage(sf::Sprite &sprite, float width, float height)
{
    // Get the current texture's size
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Calculate scale factors for width and height
    float scaleX = width / bounds.width;
    float scaleY = height / bounds.height;

    // Apply the scale to the sprite
    sprite.setScale(scaleX, scaleY);

    return sprite;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Skill Summary", sf::Style::Titlebar | sf::Style::Close);
    window.setSize(sf::Vector2u(800, 600));

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return -1;
    }

    std::vector<Skill> skillspass;
    skillspass.push_back(Skill("HEAL_PASSIVE", Skill::HEALPASSIVE));
    skillspass.push_back(Skill("CHIP_BOOST", Skill::CHIP_BOOST));
    skillspass.push_back(Skill("DEFENSE_BOOST", Skill::DEFENSE_BOOST));
    skillspass.push_back(Skill("CRITICAL_ATTACK", Skill::CRITICAL_ATTACK));

    std::vector<Skill> skillsact;
    skillsact.push_back(Skill("ATTACK_BOOST", Skill::ATTACK_BOOST));
    skillsact.push_back(Skill("STEAL_SKILL", Skill::STEAL_SKILL));
    skillsact.push_back(Skill("SHIELD", Skill::SHIELD));
    skillsact.push_back(Skill("BUFF_X2", Skill::BUFF_X2));
    skillsact.push_back(Skill("HEAL_ACTIVE", Skill::HEALACTIVE));

    int cols = 6;
    float startX = 80;
    float startY = 200;

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    int rows = (skillsact.size() + cols - 1) / cols;
    float spacingX = (windowWidth - startX * 0.5) / cols;
    float spacingY = (windowHeight - startY * 0.5) / (rows + 1);
    float radius = std::min(spacingX, spacingY) / 3.0f;

    std::vector<sf::CircleShape> activeSkillBoxes;
    std::vector<sf::Text> activeSkillTexts;
    std::vector<sf::CircleShape> passiveSkillBoxes;
    std::vector<sf::Text> passiveSkillTexts;
    std::vector<bool> isLockDrawn(skillsact.size() + skillspass.size(), false); // ตั้งค่าเริ่มต้นเป็น false

    // Load sprites for active and passive skills
    std::vector<sf::Sprite> activeSkillSprites;
    for (const auto &skill : skillsact)
    {
        sf::Sprite sprite = loadImage(skill.getName()); // โหลดรูปภาพ
        sprite = resizeImage(sprite, 60, 60);           // ปรับขนาดรูปภาพ
        activeSkillSprites.push_back(sprite);
    }

    std::vector<sf::Sprite> passiveSkillSprites;
    for (const auto &skill : skillspass)
    {
        sf::Sprite sprite = loadImage(skill.getName()); // โหลดรูปภาพ
        sprite = resizeImage(sprite, 60, 60);           // ปรับขนาดรูปภาพ
        passiveSkillSprites.push_back(sprite);
    }

    for (size_t i = 0; i < skillsact.size(); i++)
    {
        bool isUnlocked = (i % 2 == 0); // ปลดล็อคทุกตัวที่มี index เป็นเลขคู่ (หรือเปลี่ยนตามต้องการ)
        float x = startX + (i % cols) * spacingX;
        float y = startY + (i / cols) * spacingY + 230;

        // สร้างวงกลมพร้อมสถานะล็อค
        activeSkillBoxes.push_back(createSkillCircleWithLock(x, y, radius, skillsact[i].skillcount));
        activeSkillTexts.push_back(createSkillText(skillsact[i].name, font, x, y));

        // Create the sprite for this skill
        sf::Sprite sprite = createSkillSprite(activeSkillSprites[i], activeSkillBoxes[i]);
        sprite = resizeImage(sprite, 60, 60);
        activeSkillSprites.push_back(sprite);
    }

    for (size_t i = 0; i < skillspass.size(); i++)
    {
        bool isUnlocked = (i % 2 == 0); // ปลดล็อคทุกตัวที่มี index เป็นเลขคู่ (หรือเปลี่ยนตามต้องการ)
        float x = startX + (i % cols) * spacingX;
        float y = startY + (i / cols) * spacingY;

        // สร้างวงกลมพร้อมสถานะล็อค
        passiveSkillBoxes.push_back(createSkillCircleWithLock(x, y, radius, skillspass[i].skillcount));
        passiveSkillTexts.push_back(createSkillText(skillspass[i].name, font, x, y));

        // Create the sprite for this skill
        sf::Sprite sprite = createSkillSprite(passiveSkillSprites[i], passiveSkillBoxes[i]);
        sprite = resizeImage(sprite, 60, 60);
        passiveSkillSprites.push_back(sprite);
    }

    sf::RectangleShape titleBox = createTitleBox(windowWidth, windowHeight);
    sf::Text titleText = createTitleText("Skill Summary", font, windowWidth);

    sf::RectangleShape ActiveBox = createActiveBox(windowWidth, windowHeight);
    sf::Text ActiveText = createActiveText("Active", font, windowWidth, ActiveBox.getPosition().y);

    sf::RectangleShape PassiveBox = createPassiveBox(windowWidth, windowHeight);
    sf::Text PassiveText = createPassiveText("Passive", font, windowWidth, PassiveBox.getPosition().y);

    float iconX = 750, iconY = 10, iconSize = 20;
    sf::VertexArray closeIcon = createCloseIcon(iconX, iconY, iconSize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        window.clear(sf::Color(50, 50, 50)); // ล้างหน้าจอให้เป็นสีเทาอ่อน

        window.draw(titleBox);
        window.draw(titleText);

        window.draw(PassiveBox);
        window.draw(PassiveText);

        window.draw(ActiveBox);
        window.draw(ActiveText);

        // วาดสกิล Passive
        for (size_t i = 0; i < skillspass.size(); i++)
        {
            window.draw(passiveSkillBoxes[i]);
            window.draw(passiveSkillTexts[i]);
            window.draw(passiveSkillSprites[i]); // Draw the sprite

            /*// แสดงจำนวนสกิล (ตำแหน่งตรงกลางวงกลม)
            float circleCenterX = passiveSkillBoxes[i].getPosition().x + passiveSkillBoxes[i].getRadius();
            float circleCenterY = passiveSkillBoxes[i].getPosition().y + passiveSkillBoxes[i].getRadius();
            skillspass[i].displaySkillCount(window, font, circleCenterX, circleCenterY);
            */

            /*if (skillspass[i].skillcount == 0 && !isLockDrawn[i])
            {
                // วาดกากบาท
                sf::VertexArray lockCross = createLockCross(passiveSkillBoxes[i].getPosition().x + radius, passiveSkillBoxes[i].getPosition().y + radius, radius);
                window.draw(lockCross);
                isLockDrawn[i] = true;
            }*/

            // ตรวจสอบว่าเคอร์เซอร์อยู่บนวงกลมหรือไม่
            if (passiveSkillBoxes[i].getGlobalBounds().contains(sf::Vector2f(mousePos)))
            {
                // แสดงจำนวนสกิลแทนชื่อสกิล
                sf::RectangleShape tooltipBox = createTooltipBox(passiveSkillBoxes[i].getPosition().x, passiveSkillBoxes[i].getPosition().y, skillspass[i].skillcount, font, window, 120);
                window.draw(tooltipBox);
            }
        }

        // วาดสกิล Active
        for (size_t i = 0; i < skillsact.size(); i++)
        {
            window.draw(activeSkillBoxes[i]);
            window.draw(activeSkillTexts[i]);
            window.draw(activeSkillSprites[i]); // Draw the sprite

            /* // แสดงจำนวนสกิล (ตำแหน่งตรงกลางวงกลม)
             float circleCenterX = activeSkillBoxes[i].getPosition().x + activeSkillBoxes[i].getRadius();
             float circleCenterY = activeSkillBoxes[i].getPosition().y + activeSkillBoxes[i].getRadius();
             skillsact[i].displaySkillCount(window, font, circleCenterX, circleCenterY);
             */

            /* if (skillsact[i].skillcount == 0 && !isLockDrawn[skillsact.size() + i])
             {
                 // วาดกากบาท
                 sf::VertexArray lockCross = createLockCross(activeSkillBoxes[i].getPosition().x + radius, activeSkillBoxes[i].getPosition().y + radius, radius);
                 window.draw(lockCross);
                 isLockDrawn[skillsact.size() + i] = true;
             }
             */

            // ตรวจสอบว่าเคอร์เซอร์อยู่บนวงกลมหรือไม่
            if (activeSkillBoxes[i].getGlobalBounds().contains(sf::Vector2f(mousePos)))
            {
                sf::RectangleShape tooltipBox = createTooltipBox(activeSkillBoxes[i].getPosition().x, activeSkillBoxes[i].getPosition().y, skillsact[i].skillcount, font, window, 120);
                window.draw(tooltipBox);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));
            if (isCloseIconClicked(mousePos, iconX, iconY, iconSize))
            {
                window.close(); // คลิกปิดหน้าต่าง
            }
        }
        window.draw(closeIcon);
        window.display();
    }

    return 0;
}
