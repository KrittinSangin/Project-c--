#ifndef paoyingchub_h
#define paoyingchub_h

#include <SFML/Graphics.hpp>
#include <window>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

enum Choices
{
    SCISSORS = 1,
    PAPER,
    ROCK
};

string getChoiceName(int choice)
{
    switch (choice)
    {
    case SCISSORS:
        return "Scissors";
    case PAPER:
        return "Paper";
    case ROCK:
        return "Rock";
    default:
        return "Invalid";
    }
}

int spinChoice()
{
    return rand() % 3 + 1; // ค่าระหว่าง 1-3 (กรรไกร, กระดาษ, ค้อน)
}

// อัปเดตข้อความเมื่อกดสปิน
void updateText(float x, float y, sf::Text &text1, sf::Text &text2, sf::Text &text3, int spin1, int spin2, int spin3, sf::Font &font)
{
    text1.setString(getChoiceName(spin1));
    text2.setString(getChoiceName(spin2));
    text3.setString(getChoiceName(spin3));

    text1.setFont(font);
    text2.setFont(font);
    text3.setFont(font);

    text1.setCharacterSize(20);
    text2.setCharacterSize(20);
    text3.setCharacterSize(20);

    text1.setFillColor(sf::Color::Black);
    text2.setFillColor(sf::Color::Black);
    text3.setFillColor(sf::Color::Black);

    // คำนวณตำแหน่งให้อยู่กลางกรอบ
    sf::FloatRect text1Bounds = text1.getLocalBounds();
    sf::FloatRect text2Bounds = text2.getLocalBounds();
    sf::FloatRect text3Bounds = text3.getLocalBounds();

    text1.setPosition(x + 25 - text1Bounds.width / 2, y + 20 - text1Bounds.height / 2);  // กรอบที่ 1
    text2.setPosition(x + 125 - text2Bounds.width / 2, y + 20 - text2Bounds.height / 2); // กรอบที่ 2
    text3.setPosition(x + 225 - text3Bounds.width / 2, y + 20 - text3Bounds.height / 2); // กรอบที่ 3
}

bool isButtonClicked(sf::RectangleShape &button, sf::Event &event)
{
    return button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
}

// สร้างปุ่มสปิน
void createButton(sf::RectangleShape &button, sf::Text &buttonText, sf::Font &font)
{
    button.setSize(sf::Vector2f(200, 50));
    button.setPosition(300, 400);
    button.setFillColor(sf::Color::Green);

    buttonText.setString("Spin");
    buttonText.setFont(font);
    buttonText.setCharacterSize(30);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(375, 410);
}

// สร้างกรอบสปิน 3 ช่อง
void createBox(sf::RectangleShape &box, float x, float y)
{
    box.setSize(sf::Vector2f(100, 50));         // ขนาดกรอบ
    box.setPosition(x, y);                      // ตำแหน่งของกรอบ
    box.setFillColor(sf::Color(225, 225, 250)); // สีของกรอบ
    box.setOutlineColor(sf::Color::White);      // ขอบกรอบ
    box.setOutlineThickness(2);                 // ความหนาของขอบกรอบ
}

// เปรียบเทียบ 2 spin
string compareSpins(int spin1, int spin2)
{
    if (spin1 == spin2)
    {
        return "It's a tie!";
    }
    else if ((spin1 == SCISSORS && spin2 == PAPER) ||
             (spin1 == PAPER && spin2 == ROCK) ||
             (spin1 == ROCK && spin2 == SCISSORS))
    {
        return "Spin 1 wins!";
    }
    else
    {
        return "Spin 2 wins!";
    }
}
#endif // paoyingchub_h