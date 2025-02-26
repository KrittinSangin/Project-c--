#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
using namespace sf;


RectangleShape DrawSpinButton(sf::RenderWindow& window){
    RectangleShape spinButton(Vector2f(200, 60));
    spinButton.setPosition(300, 400);
    spinButton.setFillColor(Color::Green);
    window.draw(spinButton);
    return spinButton;
}




void waitForMouseClick(sf::RenderWindow& window) {
    sf::Event event;
    
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (DrawSpinButton(window).getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                std::cerr << "Button Pressed" << std::endl;
                return; // Exit when a mouse button is pressed
            }
           
        }
    }
}
void OutputRandomizedChar(char *Array1,vector<char> v,int size){
    for(int i = 0;i < 3;i++){
        int randint = rand()%size;
       Array1[i] = v[randint];
    }
    
}

sf::Text checkCondition(char *CurrentArray, sf::RenderWindow& window) {
    sf::Text Status;
    static sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
    }
    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(sf::Color::White);
    Status.setPosition(380, 200);

    if ((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1] == CurrentArray[2])) {
        Status.setString("Won!");
    } else {
        Status.setString("Lost!");
    }

    return Status;
}


void SpinSlot(int chips, char *RandomizedChar, vector<char> BaseChar, int vecsize, sf::RenderWindow& window, sf::Font& font) {
    window.clear();
    sf::Text CurrentElement;
    CurrentElement.setFont(font);
    CurrentElement.setCharacterSize(30);
    CurrentElement.setFillColor(sf::Color::Yellow);

    sf::Text Status;
    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(sf::Color::White);
    Status.setPosition(300, 250);

    sf::Text Chips;
    Chips.setFont(font);
    Chips.setCharacterSize(30);
    Chips.setFillColor(sf::Color::White);
    Chips.setPosition(400, 300);

    while (chips >= 0) {
        
        
        OutputRandomizedChar(RandomizedChar, BaseChar, vecsize);

        window.clear(sf::Color::Black); 
        for (int i = 0; i < 3; i++) {
            CurrentElement.setString(std::string(1, RandomizedChar[i]));
            CurrentElement.setPosition(350 + (50 * i), 250);
            window.draw(CurrentElement);
        }
        
        Chips.setString(to_string(chips));
        window.draw(Chips);
        Status = checkCondition(RandomizedChar, window);
        window.draw(Status); 
        DrawSpinButton(window);
        window.display(); 
        waitForMouseClick(window);
        chips--;
    }
}


int eventHandler(){

    return 1;
}


int main()
{
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }


    
    vector<char> BaseChar = {'A', 'B','C'};
    int chips = 10;
    int vecsize = BaseChar.size();
    srand(time(0));
    char RandomizedChar[3] = {};
    std::vector<sf::Sprite> sprites;


    // ���ҧ˹�ҵ�ҧ
    sf::RenderWindow window(sf::VideoMode(800, 600), "Slot Machine Game Start");

    // ��Ŵ�Ҿ�����ѧ
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background1.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    // ���ҧ Sprite ����Ѻ�����ѧ
    sf::Sprite background(backgroundTexture);

    // ��駤�Ҩش��ҧ�ԧ�ͧ Sprite ����������ҧ�Ҿ
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);

    // ��駵��˹觢ͧ�Ҿ�����ѧ��������ҧ˹�ҵ�ҧ
    background.setPosition(window.getSize().x / 1.9, window.getSize().y / 2.3);

    // ��Ŵ�����������
    sf::Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("startbutton.png")) {
        std::cerr << "Failed to load start button image!" << std::endl;
        return -1;
    }
    sf::Sprite startButton(startButtonTexture);

    // ��駵��˹觢ͧ�������������������ҧ˹�Ҩ�
    float buttonCenterX = window.getSize().x / 2;
    float buttonCenterY = window.getSize().y / 2;
    startButton.setPosition(buttonCenterX - startButton.getLocalBounds().width + 250,
        buttonCenterY - startButton.getLocalBounds().height + 300);

    // ��駨ش��ҧ�ԧ (origin) �ͧ��������繡�ҧ�ͧ����
    startButton.setOrigin(startButton.getLocalBounds().width / 2, startButton.getLocalBounds().height / 2);

    // ���ҧ���ԡ�����Ѻ�Ѻ����
    sf::Clock clock;

    sf::Vector2f originalScale(1.f, 1.f);
    sf::Vector2f hoverScale(1.2f, 1.2f); // ���� 20% ����� Hover

    
    // �ٻ��ѡ�ͧ��
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // ��Ǩ�ͺ��ä�ԡ�������������
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                startButton.setScale(hoverScale);  // �� startButton ᷹ startButtonTexture
            }
            else {
                startButton.setScale(originalScale);  // �� startButton ᷹ startButtonTexture
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::FloatRect buttonBounds = startButton.getGlobalBounds();
                if (buttonBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                    // ����ͤ�ԡ�����������
                    std::cerr << "Left click Start" << std::endl;
                    startButton = Sprite();
                    background = Sprite();

                    

                    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font);
                   
                    break;
                    // ����ö���������Ѻ���������������
                }

            }
        }

        // �ӹǳ��� scale �ҡ�������� sine wave �������������˴
        float time = clock.getElapsedTime().asSeconds();
        float scaleFactor = 1.1f + 0.05f * std::sin(time * 1.5);  // �� sine wave �����������˴

        // ��Ѻ��Ҵ�����ѧ���ʹ�˹�ҵ�ҧ ������������˴
        background.setScale(
            static_cast<float>(window.getSize().x) / background.getLocalBounds().width * scaleFactor,
            static_cast<float>(window.getSize().y) / background.getLocalBounds().height * scaleFactor
        );

       



        // �Ҵ�ء���ҧ
        window.clear();
        window.draw(background);  // �Ҵ�����ѧ
        window.draw(startButton);  // �Ҵ�����������
        window.display();
    }

    return 0;
}