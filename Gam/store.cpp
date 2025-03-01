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

class coin {
    public:
        int mycoin;
        coin(int getcoin) {
            mycoin = getcoin;
        }
        coin(const coin& copy) {
            mycoin = copy.mycoin;
        }
        coin& operator=(const coin& other) {
            if (this != &other) {
                mycoin = other.mycoin;
            }
            return *this;
        }
    };
    

class store {
private:
    std::vector<item> stock;
public:
    bool hideAlert = false;
    item forsale[3];  // Array to store the items that are for sale
    void addItem(const item& ITEM); // Add an item to the stock
    void randomForSale();// Function to randomize which 3 items are for sale 
    void reroll();// Function to re-roll the items for sale
    void removeItem(int index);// Function to mark an item as sold out
    void displayItems(sf::RenderWindow& window, sf::Font& font);// Display the names of the items
    void displayCosts(sf::RenderWindow& window, sf::Font& font);// แสดงราคาสินค้าและสถานะ
    void handlePurchase(int ,coin&); // ฟังก์ชันที่จัดการการซื้อสินค้า
};

void store::addItem(const item& ITEM){
    stock.push_back(ITEM);
}

void store::randomForSale(){
    std::vector<item> stockClone = stock; // Clone the stock to shuffle
    int random;
    for (int i = 0; i < 3; i++) {
        random = rand() % stockClone.size();  // Get a random item
        forsale[i] = stockClone[random];  // Assign it to forsale
        stockClone.erase(stockClone.begin() + random);  // Remove item from stockClone
    }
}

void store::reroll() {
    randomForSale();  // Just call randomForSale again
}

void store::removeItem(int index) {
    forsale[index].soldOut = true;  // Mark the item as sold out
    forsale[index].name = "SOLD OUT";  // Change name to "SOLD OUT"
}

void store::displayItems(sf::RenderWindow& window, sf::Font& font) {
    for (int i = 0; i < 3; i++) {
        if (forsale[i].name != "SOLD OUT") {  // Display name if not sold out
            sf::Text itemText(forsale[i].name, font, 100);
            itemText.setFillColor(sf::Color::White);
            itemText.setPosition(160 + i * 205, 150);
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

void store::displayCosts(sf::RenderWindow& window, sf::Font& font) {
    for (int i = 0; i < 3; i++) {
            if (forsale[i].soldOut) {
                sf::Text soldOutPriceText("SOLD OUT", font, 30);
                soldOutPriceText.setFillColor(sf::Color::Red);  // สีแดงสำหรับสินค้าที่หมด
                soldOutPriceText.setPosition(140 + i * 200, 310);
                window.draw(soldOutPriceText);
            } else {
                // แสดงราคาสินค้า
                sf::Text costText(std::to_string(forsale[i].cost), font, 45);
                costText.setFillColor(sf::Color::Yellow);
                costText.setPosition(155 + i * 215, 300);
                window.draw(costText);
            }
        }
}

void store::handlePurchase(int itemIndex, coin& playerMoney) {
    if (!forsale[itemIndex].soldOut && playerMoney.mycoin >= forsale[itemIndex].cost) {
        // ถ้าสามารถซื้อได้และสินค้ายังไม่หมด
        playerMoney.mycoin -= forsale[itemIndex].cost;  // ลดเงิน
        forsale[itemIndex].soldOut = true;  // ทำให้สินค้าหมด
        forsale[itemIndex].name = "SOLD OUT";
    }    
}

//independence func
// Check if a button was clicked
bool RandomButtonClicked(const sf::RectangleShape& button, const sf::Event& event) {
    return button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
}

// Check if a price has been clicked
bool isCostClicked(int index, const sf::Event& event, store& STORE) {
    // ตรวจสอบว่า item ไม่ได้เป็น SOLD OUT ก่อน
    if (STORE.forsale[index].soldOut) {  // Check if item is sold out
        return false;  // If the item is sold out, don't allow click
    }
    float x = 155 + index * 210;
    float y = 300;
    float width = 60;
    float height = 60;
    return sf::FloatRect(x, y, width, height).contains(event.mouseButton.x, event.mouseButton.y);
}

int main() {
    srand(time(0));  // Seed the random number generator

    coin money(150);
    coin copymoney = money;

    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Store");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        std::cout << "Error loading font!" << std::endl;
        return -1;
    }

    store game;

    // Create random button
    sf::RectangleShape randombutton(sf::Vector2f(150, 150));
    randombutton.setFillColor(sf::Color::Blue);
    randombutton.setPosition(600, 430);

    // Create random Button text
    sf::Text randombuttonText("RANDOM", font, 30);
    randombuttonText.setFillColor(sf::Color::Yellow);
    randombuttonText.setPosition(607, 460);

    // Create text for store label
    sf::Text storeText("STORE", font, 45);
    storeText.setFillColor(sf::Color::Yellow);
    storeText.setPosition(320, 30);

    // Create button to show money
    sf::RectangleShape showmoney(sf::Vector2f(230, 50));
    showmoney.setFillColor(sf::Color::Green);
    showmoney.setPosition(30, 500);

    // Create text for money
    sf::Text coinText("COIN", font, 27);
    coinText.setFillColor(sf::Color::Blue);
    coinText.setPosition(64, 501);

    // Price button text
    sf::Text costrandombuttonText("100", font, 30);
    costrandombuttonText.setFillColor(sf::Color::Yellow);
    costrandombuttonText.setPosition(643, 510);

    // สร้างปุ่ม Exit
    RectangleShape exitButton(Vector2f(30, 30));
    exitButton.setFillColor(Color::Red);
    exitButton.setPosition(740, 20);

    //create exittexts
    Text exitText("X", font, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(744, 17); // ตำแหน่งของข้อความในปุ่ม

    // Create items
    item ITEMA = {"A", 10, false};
    item ITEMB = {"B", 20, false};
    item ITEMC = {"C", 30, false};
    item ITEMD = {"D", 40, false};
    item ITEME = {"E", 50, false};

    // Add items to store
    game.addItem(ITEMA);
    game.addItem(ITEMB);
    game.addItem(ITEMC);
    game.addItem(ITEMD);
    game.addItem(ITEME);

    // Randomize items for sale
    game.randomForSale();

    // Alert not enough money text
    sf::Text alertText("", font, 25);
    alertText.setFillColor(sf::Color::Red);
    alertText.setPosition(300, 400);

    // Clock for timer not enough money show 1 s
    sf::Clock clock;
    bool showAlert = false;
    float alertDuration = 1.0f;

    // ตัวแปรสถานะของหน้าจอ (0 = หน้าแรก, 1 = หน้าถัดไป)
    int currentScreen = 0;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if button was clicked

                    if (RandomButtonClicked(randombutton, event)) {
                        if (copymoney.mycoin >= 100) {
                            copymoney.mycoin -= 100;
                            game.reroll();  // Re-roll the items for sale
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
                        if (isCostClicked(i, event, game)) {
                            if (game.forsale[i].name != "SOLD OUT") {
                                if (copymoney.mycoin >= game.forsale[i].cost) {
                                    copymoney.mycoin -= game.forsale[i].cost;
                                    game.removeItem(i);  // Mark item as sold
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

                    //check if exit button was clicked
                    if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        // เปลี่ยนไปยังหน้าอื่น (เปลี่ยนสีหน้าจอและข้อความ)
                        currentScreen = 1; 
                    }
                }
            }
        }

        if(currentScreen == 0){
            // Check if alert should be shown
        if (showAlert && clock.getElapsedTime().asSeconds() > alertDuration) {
            alertText.setString("");  // Hide alert after 1 second
            showAlert = false;
        }
    
            // Update money display
            std::stringstream ss;
            ss << "Money: " << copymoney.mycoin;
            coinText.setString(ss.str());
    
            window.clear(); //หลังจากนี้จะเป็นวาดกราฟฟิค default window.clear(sf::Color::Black)
    
            // Draw buttons and texts
            window.draw(randombutton);
            window.draw(randombuttonText);
            window.draw(storeText);
            window.draw(showmoney);
            window.draw(costrandombuttonText);
            window.draw(coinText);
    
            // Draw items and prices
            game.displayItems(window, font);
            game.displayCosts(window, font);
    
            // Draw alert text if necessary
            if (showAlert) {
            window.draw(alertText);
            }
        }else{
            window.clear(Color::White);  // สีฟ้า
        }
        window.draw(exitButton);
        window.draw(exitText);

        window.display();
    }

    return 0;
}
