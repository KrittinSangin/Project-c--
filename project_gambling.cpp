#include <SFML/Graphics.hpp>
#include <iostream> // For debugging
#include <iostream>
#include "Boss1_Slum.h"
#include <vector>

using namespace std;





void OutputRandomizedChar(char *Array1,vector<char> v,int size){
    for(int i = 0;i < 3;i++){
        int randint = rand()%size;
       Array1[i] = v[randint];
    }
    
}
sf::Text checkCondition(char *CurrentArray){
    sf::Text Status;
    if((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1]== CurrentArray[2]) &&(CurrentArray[0]== CurrentArray[2])){ 
        Status.setString("Won");
        Status.setCharacterSize(30);
        Status.setFillColor(sf::Color::White);
        Status.setPosition(300,250);
        


    }
    return Status;
}

void SpinSlot(int chips, char *RandomizedChar ,vector<char> BaseChar ,int vecsize,sf::RenderWindow& window){
    sf::Text defaultchipdisplay;
    sf::Text chipnum;
    sf::Text CurrentElement;
    CurrentElement.setCharacterSize(30);
    CurrentElement.setFillColor(sf::Color::Yellow);
    CurrentElement.setPosition(100,250);
    do{
    OutputRandomizedChar(RandomizedChar,BaseChar,vecsize);
    for(int i = 0;i < 3;i++){
       CurrentElement.setString(RandomizedChar[i]);
        window.draw(CurrentElement);
        CurrentElement.setPosition(100+(50*i),250);
        window.display();
    }
    
    checkCondition(RandomizedChar);
   
    //cin.get();
    chips--;
    }while(chips > 0);
}

int eventHandler(){

    return 1;
}

int main() {

    
    vector<char> BaseChar= {'A','B'};
    int chips = 10;
    int vecsize =  BaseChar.size();
    srand(time(0));
    char RandomizedChar[3] = {};
    //SpinSlot(chips,RandomizedChar,BaseChar,vecsize);//input 1.chips 2.basechar 3.vecsize
  
    






    // Create a window with a title
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }

    // Create a text object
    sf::Text text("Beebob", font, 50);
    text.setFillColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Bold);

    // Center the text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);




    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {//run your functions here
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            




        }


        window.clear(sf::Color::Black);
        SpinSlot(chips,RandomizedChar,BaseChar,vecsize,window);
    }

    return 0;
}




