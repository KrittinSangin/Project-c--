#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;
using namespace sf;
class item {
public:
    std::string name;
    int cost;
    bool soldOut;
};

class store {
private:
    std::vector<item> stock;
public:
    item forsale[3];  // Array to store the items that are for sale
    int playerMoney = 1000;  // จำนวนเงินผู้เล่น

    // Add an item to the stock
    void addItem(const item& ITEM) {
        stock.push_back(ITEM);
    }

    // Function to randomize which 3 items are for sale
    void randomForSale() {
        std::vector<item> stockClone = stock; // Clone the stock to shuffle
        int random;
        for (int i = 0; i < 3; i++) {
            random = rand() % stockClone.size();  // Get a random item
            forsale[i] = stockClone[random];  // Assign it to forsale
            stockClone.erase(stockClone.begin() + random);  // Remove item from stockClone
        }
    }

    // Function to re-roll the items for sale
    void reroll() {
        randomForSale();  // Just call randomForSale again
    }

    // Function to mark an item as sold out
    void removeItem(int index) {
        forsale[index].name = "";  // Change the name to SOLD OUT
    }

    // Display the names of the items
    void displayItems(sf::RenderWindow& window, sf::Font& font) {
        for (int i = 0; i < 3; i++) {
            if (forsale[i].name != "") {  // Display name if not sold out
                sf::Text itemText(forsale[i].name, font, 100);
                itemText.setFillColor(sf::Color::White);
                itemText.setPosition(160 + i * 200, 150);
                window.draw(itemText);
            } else {
                // Display SOLD OUT if the item is sold
                sf::Text soldOutText("", font, 100);
                soldOutText.setFillColor(sf::Color::Red);
                soldOutText.setPosition(160 + i * 200, 150);
                window.draw(soldOutText);
            }
        }
    }

    // แสดงราคาสินค้าและสถานะ
    void displayCosts(sf::RenderWindow& window, sf::Font& font) {
        for (int i = 0; i < 3; i++) {
            if (forsale[i].name != "") {  // ถ้ามีชื่อสินค้า
                if (forsale[i].soldOut) {
                    sf::Text soldOutPriceText("Sold Out", font, 35);
                    soldOutPriceText.setFillColor(sf::Color::Red);  // สีแดงสำหรับสินค้าที่หมด
                    soldOutPriceText.setPosition(155 + i * 170, 300);
                    window.draw(soldOutPriceText);
                } else {
                    // แสดงราคาสินค้า
                    sf::Text costText(std::to_string(forsale[i].cost), font, 40);
                    costText.setFillColor(sf::Color::Yellow);
                    costText.setPosition(165 + i * 200, 300);
                    window.draw(costText);
                }
            }
        }
    }

        // ฟังก์ชันที่จัดการการซื้อสินค้า
        void handlePurchase(int itemIndex) {
            if (!forsale[itemIndex].soldOut && playerMoney >= forsale[itemIndex].cost) {
                // ถ้าสามารถซื้อได้
                playerMoney -= forsale[itemIndex].cost;  // ลดเงิน
                forsale[itemIndex].soldOut = true;  // ทำให้สินค้าหมด
            }
        }
    };

// Check if a button was clicked
bool isButtonClicked(const sf::RectangleShape& button, const sf::Event& event) {
    return button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
}

// Check if a price has been clicked
bool isCostClicked(int index, const sf::Event& event, store& STORE) {
    float x = 165 + index * 200;
    float y = 300;
    float width = 40;
    float height = 40;
    return sf::FloatRect(x, y, width, height).contains(event.mouseButton.x, event.mouseButton.y);
}

int main() {
    srand(time(0));  // Seed the random number generator

    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Store");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cout << "Error loading font!" << std::endl;
        return -1;
    }

    store game;

    // Create button
    sf::RectangleShape button(sf::Vector2f(150, 150));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(560, 400);

    // Money
    int money = 150;

    // Create button to show money
    sf::RectangleShape showmoney(sf::Vector2f(230, 50));
    showmoney.setFillColor(sf::Color::Green);
    showmoney.setPosition(50, 500);

    // Create text for money
    sf::Text coinText("COIN", font, 27);
    coinText.setFillColor(sf::Color::Black);
    coinText.setPosition(90, 500);

    // Create text for store label
    sf::Text storeText("STORE", font, 45);
    storeText.setFillColor(sf::Color::Yellow);
    storeText.setPosition(320, 20);

    // Button text
    sf::Text buttonText("RANDOM", font, 30);
    buttonText.setFillColor(sf::Color::Yellow);
    buttonText.setPosition(565, 435);

    // Price button text
    sf::Text bbuttonText("100", font, 30);
    bbuttonText.setFillColor(sf::Color::Yellow);
    bbuttonText.setPosition(610, 495);

    // Create items
    item ITEMA = {"A", 10, false};
    item ITEMB = {"B", 20, false};
    item ITEMC = {"C", 30, false};
    item ITEMD = {"D", 40, false};
    item ITEME = {"E", 50, false};
    store STORE;

    // Add items to store
    STORE.addItem(ITEMA);
    STORE.addItem(ITEMB);
    STORE.addItem(ITEMC);
    STORE.addItem(ITEMD);
    STORE.addItem(ITEME);

    // Randomize items for sale
    STORE.randomForSale();

    // Alert text
    sf::Text alertText("", font, 25);
    alertText.setFillColor(sf::Color::Red);
    alertText.setPosition(300, 400);

    // Clock for timer
    sf::Clock clock;
    bool showAlert = false;
    float alertDuration = 1.0f;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if "RANDOM" button was clicked
                    if (isButtonClicked(button, event)) {
                        if (money >= 100) {
                            money -= 100;
                            STORE.reroll();  // Re-roll the items for sale
                            alertText.setString("");
                            showAlert = false;
                            clock.restart();
                        } else {
                            alertText.setString("Not enough money!");
                            showAlert = true;
                            clock.restart();
                        }
                    }

                    // Check if a price was clicked
                    for (int i = 0; i < 3; i++) {
                        if (isCostClicked(i, event, STORE)) {
                            if (STORE.forsale[i].name != "SOLD OUT") {
                                if (money >= STORE.forsale[i].cost) {
                                    money -= STORE.forsale[i].cost;
                                    STORE.removeItem(i);  // Mark item as sold
                                    alertText.setString("");
                                    showAlert = false;
                                    clock.restart();
                                } else {
                                    alertText.setString("Not enough money!");
                                    showAlert = true;
                                    clock.restart();
                                }
                            }
                        }
                    }
                }
            }
        }

        // Check if alert should be shown
        if (showAlert && clock.getElapsedTime().asSeconds() > alertDuration) {
            alertText.setString("");
            showAlert = false;
        }

        // Update money display
        std::stringstream ss;
        ss << "Money: " << money;
        coinText.setString(ss.str());

        window.clear();

        // Draw button and texts
        window.draw(button);
        window.draw(buttonText);
        window.draw(bbuttonText);
        window.draw(storeText);
        window.draw(showmoney);
        window.draw(coinText);
        window.draw(alertText);

        // Draw items and prices
        STORE.displayItems(window, font);
        STORE.displayCosts(window, font);

        window.display();
    }

    return 0;
}
