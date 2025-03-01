#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<cmath>
using namespace std;
using namespace sf;





RectangleShape DrawSpinButton() {
    RectangleShape spinButton(Vector2f(200, 60));
    spinButton.setPosition(320, 400);
    spinButton.setFillColor(Color::Green);
    return spinButton;
}

void waitForMouseClick(sf::RenderWindow& window, bool& isSpinning1, bool& isSpinning2, bool& isSpinning3, Clock& clock, RectangleShape& spinButton) {
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (spinButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                cerr << "Button Pressed" << endl;
                isSpinning1 = isSpinning2 = isSpinning3 = true;
                clock.restart();
                return;
            }
        }
    }
}

void OutputRandomizedChar(char* Array1, vector<char> v, int size) {
    for (int i = 0; i < 3; i++) {
        int randint = rand() % size;
        Array1[i] = v[randint];
    }
}

Text checkCondition(char* CurrentArray, Font& font,int & chips,const int maxchips,Text &ChipsText) {
    Text Status;
    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(Color::White);
    Status.setPosition(380, 200);

    if ((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1] == CurrentArray[2])) {
        
        switch (CurrentArray[0])
        {
        case 'C':
            Status.setString("Won " + to_string(maxchips*2/10) + " Chips");
           chips = chips + (maxchips*3/10);
           ChipsText.setString("Chips: " + to_string(chips));
            break;
        case 'M':
           
            break;
        
        case 'S':
           //handle your skill slot right here
            break;
        
        default:
        Status.setString("Won!");
            break;
        }
    } else {
        Status.setString("Lost!");
    }
    return Status;
}

void SpinSlot(int chips, char* RandomizedChar, vector<char> BaseChar, int vecsize, sf::RenderWindow& window, sf::Font& font, int maxchips,bool & FinishedPhase) {
    bool isSpinning1 = false, isSpinning2 = false, isSpinning3 = false;
    Clock clock;
    float spinDuration = 0.85;
    
    Text Slot1, Slot2, Slot3, Status, ChipsText;
    Slot1.setFont(font);
    Slot1.setCharacterSize(30);
    Slot1.setFillColor(Color::Yellow);
    Slot1.setPosition(450, 250);

    Slot2 = Slot1;
    Slot2.setPosition(400, 250);

    Slot3 = Slot1;
    Slot3.setPosition(350, 250);

    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(Color::White);
    Status.setPosition(300, 250);

    ChipsText.setFont(font);
    ChipsText.setCharacterSize(30);
    ChipsText.setFillColor(Color::White);
    ChipsText.setPosition(400, 300);

    RectangleShape spinButton = DrawSpinButton();

    while (chips >= 0) {
        OutputRandomizedChar(RandomizedChar, BaseChar, vecsize);
        clock.restart();

        while (isSpinning1 || isSpinning2 || isSpinning3) {
            float elapsed = clock.getElapsedTime().asSeconds();
            window.clear();

            if (isSpinning1) {
                Slot1.setString(string(1, BaseChar[rand() % vecsize]));
            }
            if (isSpinning2) {
                Slot2.setString(string(1, BaseChar[rand() % vecsize]));
            }
            if (isSpinning3) {
                Slot3.setString(string(1, BaseChar[rand() % vecsize]));
            }

            if (elapsed > spinDuration && isSpinning1) {
                Slot1.setString(string(1, RandomizedChar[0]));
                isSpinning1 = false;
            }
            if (elapsed > spinDuration - 0.5 && isSpinning2) {
                Slot2.setString(string(1, RandomizedChar[1]));
                isSpinning2 = false;
            }
            if (elapsed > spinDuration - 0.75 && isSpinning3) {
                Slot3.setString(string(1, RandomizedChar[2]));
                isSpinning3 = false;
            }
            
            window.draw(Slot1);
            window.draw(Slot2);
            window.draw(Slot3);
            window.display();
        }

        clock.restart();
        ChipsText.setString("Chips: " + to_string(chips));
        Status = checkCondition(RandomizedChar, font,chips,maxchips,ChipsText);

        window.clear();
        window.draw(Slot1);
        window.draw(Slot2);
        window.draw(Slot3);
        window.draw(ChipsText);
        window.draw(Status);
        window.draw(spinButton);
        window.display();

        waitForMouseClick(window, isSpinning1, isSpinning2, isSpinning3, clock, spinButton);
        chips--;
    }
    FinishedPhase = true;
}


int main() {
    bool FinishedPhase = false;



    Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }

    vector<char> BaseChar = {'A','C'};
    int maxchips = 10;
    int chips = maxchips;
    int vecsize = BaseChar.size();
    srand(time(0));
    char RandomizedChar[3] = {};

    sf::RenderWindow window(sf::VideoMode(800, 600), "Slot Machine");

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background1.png")) {
        cerr << "Failed to load background image!" << endl;
        return -1;
    }
    Sprite background(backgroundTexture);
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
    background.setPosition(window.getSize().x / 1.9, window.getSize().y / 2.3);

    Texture startButtonTexture;
    if (!startButtonTexture.loadFromFile("startbutton.png")) {
        cerr << "Failed to load start button image!" << endl;
        return -1;
    }
    Sprite startButton(startButtonTexture);
    float buttonCenterX = window.getSize().x / 2;
    float buttonCenterY = window.getSize().y / 2;
    startButton.setPosition(buttonCenterX - startButton.getLocalBounds().width + 250, buttonCenterY - startButton.getLocalBounds().height + 300);
    startButton.setOrigin(startButton.getLocalBounds().width / 2, startButton.getLocalBounds().height / 2);

    Clock clock;
    Vector2f originalScale(1.f, 1.f);
    Vector2f hoverScale(1.2f, 1.2f);



    while (window.isOpen()) {


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                startButton.setScale(hoverScale);
            } else {
                startButton.setScale(originalScale);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    cerr << "Left click Start" << endl;
                    background = Sprite();
                    startButton = Sprite();
                    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font,maxchips,FinishedPhase);
                }
            }
        }

        float time = clock.getElapsedTime().asSeconds();
        float scaleFactor = 1.1f + 0.05f * sin(time * 1.5);
        background.setScale(window.getSize().x / background.getLocalBounds().width * scaleFactor,
                            window.getSize().y / background.getLocalBounds().height * scaleFactor);




        
                            
        window.clear();
        window.draw(background);
        window.draw(startButton);
        window.display();

        if(FinishedPhase){
            FinishedPhase = false;
            cerr << "Finished Phase 1";

        }
    }

    return 0;
}
