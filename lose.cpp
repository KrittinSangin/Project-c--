#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void showGameOverScreen(RenderWindow& window) {
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
    RenderWindow window(VideoMode(800, 600), "Game Over Screen");
    showGameOverScreen(window);
    return 0;
}