#include <SFML/Graphics.hpp>
#include <iostream> // For debugging
#include <iostream>
#include "Boss1_Slum.h"
#include <vector>

using namespace std;


void waitForMouseClick(sf::RenderWindow& window) {
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            return; // Exit when a mouse button is pressed
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
    static sf::Font font; // Load font once
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
    }
    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(sf::Color::White);
    Status.setPosition(300, 250);

    if ((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1] == CurrentArray[2])) {
        Status.setString("Won!");
    } else {
        Status.setString("Lost!");
    }

    return Status;
}


void SpinSlot(int chips, char *RandomizedChar, vector<char> BaseChar, int vecsize, sf::RenderWindow& window, sf::Font& font) {
    sf::Text CurrentElement;
    CurrentElement.setFont(font); // Set font
    CurrentElement.setCharacterSize(30);
    CurrentElement.setFillColor(sf::Color::Yellow);

    sf::Text Status;
    Status.setFont(font);
    Status.setCharacterSize(30);
    Status.setFillColor(sf::Color::White);
    Status.setPosition(300, 250);

    while (chips > 0) {
        OutputRandomizedChar(RandomizedChar, BaseChar, vecsize);

        window.clear(sf::Color::Black); // Clear before drawing new elements

        // Display each randomized character
        for (int i = 0; i < 3; i++) {
            CurrentElement.setString(std::string(1, RandomizedChar[i])); // Convert char to string
            CurrentElement.setPosition(100 + (50 * i), 250); // Set position before drawing
            window.draw(CurrentElement);
        }

        // Update status text
        Status = checkCondition(RandomizedChar, window);
        window.draw(Status); // Draw the status text

        window.display(); // Display everything

        waitForMouseClick(window); // Wait for user input

        chips--; // Reduce chips
    }
}


int eventHandler(){

    return 1;
}

int main() {
    vector<char> BaseChar = {'A', 'B'};
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

    // Call the function once and display results
    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font);

    // Main event loop
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



