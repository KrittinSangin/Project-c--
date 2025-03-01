#ifndef sumskill_h
#define sumskill_h

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// class สำหรับเก็บข้อมูลสกิล
class Skillact
{
public:
    std::string name;
};

class Skillpass
{
public:
    std::string name;
};

sf::CircleShape createSkillCircle(float x, float y, float radius)
{
    sf::CircleShape circle(radius);                // ใช้วงกลมแทนกล่อง
    circle.setPosition(x - radius, y - radius);    // ตั้งตำแหน่งศูนย์กลางที่ (x, y)
    circle.setFillColor(sf::Color(204, 255, 255)); // สีฟ้า
    return circle;
}

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

sf::RectangleShape createTitleBox(float windowWidth, float windowHeight)
{
    float boxWidth = 400;
    float boxHeight = 50;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition((windowWidth - boxWidth) / 2, 20);
    return box;
}

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

sf::RectangleShape createActiveBox(float windowWidth, float windowHeight)
{
    float boxWidth = 200;
    float boxHeight = 30;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition(50, windowHeight / 1.75 - boxHeight / 1.75);
    return box;
}

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

sf::RectangleShape createPassiveBox(float windowWidth, float windowHeight)
{
    float boxWidth = 200;
    float boxHeight = 30;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setPosition(50, windowHeight / 5.0 - boxHeight / 1.25);
    return box;
}

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

// สร้างไอคอนล็อคสกิล ตอนนี้ทำเป็นรูปกากบาท อาจจะเอา UI ใส่ทีหลัง
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

    // Debug: พิมพ์ว่ากำลังสร้างกากบาท
    // std::cout << "Drawing cross at position (" << x << ", " << y << ") with offset " << offset << std::endl;

    return cross;
}

// เพิ่มตัวแปร isUnlocked ในแต่ละสกิล
bool isUnlocked = false; // ปรับให้เป็น true เมื่อปลดล็อคแล้ว

// สกิลที่มีแล้วจะเป็นสีฟ้า สกิลที่ยังไม่มีจะเป็นสีเทา อันนี้ปรับสีได้
sf::CircleShape createSkillCircleWithLock(float x, float y, float radius, bool isUnlocked)
{
    sf::CircleShape circle(radius);             // ใช้วงกลมแทนกล่อง
    circle.setPosition(x - radius, y - radius); // ตั้งตำแหน่งศูนย์กลางที่ (x, y)

    if (isUnlocked)
    {
        circle.setFillColor(sf::Color(204, 255, 255)); // สีฟ้า สำหรับสกิลที่ปลดล็อค
    }
    else
    {
        circle.setFillColor(sf::Color(150, 150, 150)); // สีเทาสำหรับสกิลที่ล็อค
    }

    return circle;
}

sf::RectangleShape createTooltipBox(float x, float y, const std::string &message, sf::Font &font, sf::RenderWindow &window, float maxWidth)
{
    // แบ่งข้อความเป็นหลายบรรทัด
    std::vector<std::string> lines;
    std::string currentLine;
    float lineWidth = 0.0f;

    // หาขนาดของข้อความแต่ละบรรทัด
    for (size_t i = 0; i < message.size(); ++i)
    {
        currentLine += message[i];
        sf::Text tempText(currentLine, font, 15);
        lineWidth = tempText.getLocalBounds().width;

        // ถ้าความยาวบรรทัดเกินขนาดที่กำหนด
        if (lineWidth + 10 > maxWidth)
        {
            // เพิ่มบรรทัดใหม่
            lines.push_back(currentLine.substr(0, currentLine.size() - 1));
            currentLine = currentLine.substr(currentLine.size() - 1);
        }
    }
    // เพิ่มบรรทัดสุดท้าย
    if (!currentLine.empty())
        lines.push_back(currentLine);

    // สร้างข้อความใหม่จากหลายบรรทัด
    std::vector<sf::Text> textLines;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        sf::Text text(lines[i], font, 15);
        text.setFillColor(sf::Color(255, 255, 255, 250)); // กำหนดสีของข้อความที่นี่
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

    // ถ้าตำแหน่ง x เกินขอบด้านขวา
    if (newX + width > window.getSize().x)
    {
        newX = window.getSize().x - width - 10; // เลื่อนไปที่ขอบขวา
    }
    // ถ้าตำแหน่ง x เกินขอบด้านซ้าย
    if (newX < 10)
    {
        newX = 10; // เลื่อนไปที่ขอบซ้าย
    }

    // ถ้าตำแหน่ง y เกินขอบด้านบน
    if (newY < 10)
    {
        newY = 10; // เลื่อนขึ้นไปที่ขอบบน
    }
    // ถ้าตำแหน่ง y เกินขอบด้านล่าง
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

#endif // sumskill_h