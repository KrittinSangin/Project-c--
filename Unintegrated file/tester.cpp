#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include<cmath>
using namespace std;
using namespace sf;
//gam's
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
            void addmoney(int add){
                mycoin += add;
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
                sf::Text itemText(forsale[i].name, font, 20);
                itemText.setFillColor(sf::Color::White);
                itemText.setPosition(160 + i * 205, 270);
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

//





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
bool virgin = true;
Text checkCondition(char* CurrentArray, Font& font,int & chips,const int maxchips,Text &ChipsText,coin &money,Text &MoneyText) {
    
    Text Status;
    Status.setFont(font);
    Status.setCharacterSize(35);
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
             Status.setString("Won " + to_string(10) + " $");
             //ChipsText.setString("Chips: " + to_string(chips));
            money.addmoney(10);
            break;
        
        case 'S':
           //handle your skill slot right here
           
            break;
        
        default:
        Status.setString("Won!");
            break;
        }
    } else if(!virgin){
        Status.setString("Lost!");
    } else{
        Status.setString("Press to Spin!");
        virgin = false;
    }
    return Status;
    
}

void SpinSlot(int chips, char* RandomizedChar, vector<char> BaseChar, int vecsize, sf::RenderWindow& window, sf::Font& font, int maxchips,bool & FinishedPhase,coin &money) {
    bool isSpinning1 = false, isSpinning2 = false, isSpinning3 = false;
    Clock clock;
    float spinDuration = 0.85;
    
    Text Slot1, Slot2, Slot3, Status, ChipsText,MoneyText;
    Slot1.setFont(font);
    Slot1.setCharacterSize(35);
    Slot1.setFillColor(Color::Yellow);
    Slot1.setPosition(450, 250);

    Slot2 = Slot1;
    Slot2.setPosition(400, 250);

    Slot3 = Slot1;
    Slot3.setPosition(350, 250);

    Status.setFont(font);
    Status.setCharacterSize(35);
    Status.setFillColor(Color::White);
    Status.setPosition(300, 250);
    
    
    ChipsText.setFont(font);
    ChipsText.setCharacterSize(35);
    ChipsText.setFillColor(Color::White);
    ChipsText.setPosition(400, 300);
    
    MoneyText.setFont(font);
    MoneyText.setCharacterSize(35);
    MoneyText.setFillColor(Color::White);
    MoneyText.setPosition(400, 300);
    


    RectangleShape spinButton = DrawSpinButton();

    while (chips >= 3) {
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
        Status = checkCondition(RandomizedChar, font,chips,maxchips,ChipsText,money,MoneyText);

        window.clear();
        window.draw(Slot1);
        window.draw(Slot2);
        window.draw(Slot3);
        window.draw(ChipsText);
        window.draw(MoneyText);
        window.draw(Status);
        window.draw(spinButton);
        window.display();

        waitForMouseClick(window, isSpinning1, isSpinning2, isSpinning3, clock, spinButton);
        chips--;
    }
    
}


int main() {
    enum GameState { SLOT_MACHINE, STORE, NEXT_STAGE };
GameState gameState = SLOT_MACHINE;

    Font font;
    if (!font.loadFromFile("verdana.ttf")) {
        cerr << "Error: Could not load verdana.ttf\n";
        return -1;
    }




     store game;
    //gam's
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
    item ITEMA = {"HEAL", 10, false};
    item ITEMB = {"ATTACK_BOOST", 20, false};
    item ITEMC = {"STEAL_SKILL", 30, false};
    item ITEMD = {"SHIELD", 40, false};
    item ITEME = {"CHIP_BOOST", 50, false};

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
    bool FinishedPhase = false;
    vector<char> BaseChar = {'M','C','S'};
    //vector<char> BaseChar = {'M'};
    int maxchips = 10;
    int chips = maxchips;
    int vecsize = BaseChar.size();

    coin money(150);    // starting money value
    coin copymoney = money;



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

    Vector2f originalScale(1.f, 1.f);
    Vector2f hoverScale(1.2f, 1.2f);
    bool Runonce = true;
    bool MainmenuLoop = true;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        float scaleFactor = 1.1f + 0.05f * sin(time * 1.5);
        background.setScale(window.getSize().x / background.getLocalBounds().width * scaleFactor,
                            window.getSize().y / background.getLocalBounds().height * scaleFactor);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    
        window.clear();
    
        if (gameState == SLOT_MACHINE) {
            // ---------- SLOT MACHINE PHASE ----------
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                startButton.setScale(hoverScale);
            } else {
                startButton.setScale(originalScale);
            }
    
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && MainmenuLoop) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    cerr << "Left click Start" << endl;
                    background = Sprite(); // Remove background
                    startButton = Sprite(); // Remove start button
                    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font, maxchips, FinishedPhase, money);
                    MainmenuLoop = false;
                    gameState = STORE;
                    FinishedPhase = true; // Move to store phase
                }
            }
    
            if (MainmenuLoop) { // Show main menu if slot hasn't started yet
                window.draw(background);
                window.draw(startButton);
            }
        } else if(gameState == STORE){
            // ---------- STORE PHASE (CONTINUOUS RENDERING) ----------
            std::stringstream ss;
            ss << "Money: " << money.mycoin;
            coinText.setString(ss.str());


            // Draw store UI elements
            window.draw(randombutton);
            window.draw(randombuttonText);
            window.draw(storeText);
            window.draw(showmoney);
            window.draw(costrandombuttonText);
            window.draw(coinText);
    
            game.displayItems(window, font);
            game.displayCosts(window, font);
    
            if (showAlert) {
                window.draw(alertText);
            }
    
            window.draw(exitButton);
            window.draw(exitText);
    
            // Handle store interactions
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Handle reroll button
                    if (RandomButtonClicked(randombutton, event)) {
                        if (money.mycoin >= 100) {
                            money.mycoin -= 100;
                            game.reroll();  // Re-roll store items
                            alertText.setString("");
                            showAlert = false;
                        } else {
                            alertText.setString("Not enough money!");
                            showAlert = true;
                        }
                    }
    
                    // Handle item purchases
                    for (int i = 0; i < 3; i++) {
                        if (isCostClicked(i, event, game)) {
                            if (game.forsale[i].name != "SOLD OUT") {
                                if (money.mycoin >= game.forsale[i].cost) {
                                    money.mycoin -= game.forsale[i].cost;
                                    game.removeItem(i);
                                    alertText.setString("");
                                    showAlert = false;
                                } else {
                                    alertText.setString("Not enough money!");
                                    showAlert = true;
                                }
                            }
                        }
                    }
    
                    // Handle exit button
                    if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        cerr << "Exiting store" << endl;
                        gameState = NEXT_STAGE; // Move to the next phase
                    }
                }
            }
        } else if(gameState = NEXT_STAGE){
            cerr << "Success";
        }
    
        window.display(); // Keep rendering the store while open
    }
    
    
        return 0;
    }