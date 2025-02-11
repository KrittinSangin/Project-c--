#include <SFML/Graphics.hpp>
#include <iostream> // For debugging
#include <iostream>
#include <vector>
using namespace std;


void waitForMouseClick(sf::RenderWindow& window) {
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            return; 
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
        window.display(); 
        waitForMouseClick(window);
        chips--;
    }
}


int eventHandler(){

    return 1;
}

int main() {
    //coin slot skills 
    vector<char> BaseChar = {'A', 'B','C'};
    int chips = 10;
    int vecsize = BaseChar.size();
    srand(time(0));
    char RandomizedChar[3] = {};

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }

    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font);

   
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}



