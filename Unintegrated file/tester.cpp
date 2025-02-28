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
//skill

class Skill
{
public:
    string name;

    enum SkillType
    {
        HEALACTIVE,//active
        ATTACK_BOOST,
        STEAL_SKILL,
        SHIELD,
        CHIP_BOOST,//active
        BUFF_X2,
        DEFENSE_BOOST,//active
        CRITICAL_ATTACK,//active
        HEALPASSIVE
    } type;

    Skill(string n, SkillType t) : name(n), type(t) {}
    void createskill(string s){
        
    }



    void applySkill(class Player &p); // ฟังก์ชันที่จะทำงานกับ Player class
};

// Class สำหรับผู้เล่น
class Player
{
public:
    string name;
    float attackPower;
    float hp;
    float hpMax;
    float defense;
    bool shieldActive;
    int chip;                      // จำนวนเหรียญที่ผู้เล่นมี
    int chipMax;                   // จำนวนเหรียญสูงสุดที่สามารถสะสมได้
    bool buffed;                   // เช็คว่าอยู่ในสถานะบัฟหรือไม่
    vector<Skill> availableSkills; // สกิลที่ผู้เล่นสามารถเลือกใช้
    Skill *stolenSkill;            // สกิลที่ขโมยจากฝ่ายตรงข้าม
    float criticalChance;          // โอกาสในการโจมตีแบบ Critical (20%)
    bool isCritical;               // ฟีเจอร์เพื่อเก็บสถานะ Critical
    void handleChipReduction();    // ฟังก์ชันจัดการการลด chip

    Player(string n, float power) : name(n), attackPower(power), hp(100), hpMax(100), defense(10), shieldActive(false), chip(10), chipMax(100), buffed(false), stolenSkill(nullptr), criticalChance(0.2), isCritical(false) {}

    void useSkill(Skill skill);
    void stealSkill(Skill *opponentSkill);
    void takeDamage(float damage);
    void displayStatus();
    void activateShield();    // ฟังก์ชันในการเปิดโล่
    void addChip(int amount); // ฟังก์ชันเพิ่มเหรียญ
    void resetBuff();         // ฟังก์ชันในการรีเซ็ตสถานะบัฟ
    void checkCritical();     // ฟังก์ชันเช็คโอกาสการโจมตีแบบ Critical
    void attack();            // ฟังก์ชันโจมตี
};

// ฟังก์ชันที่ทำงานกับแต่ละสกิล
void Skill::applySkill(Player &p)
{
    switch (type)
    {
    case HEALACTIVE:
        // ฟื้นฟู HP 20%
        p.attackPower += (p.buffed ? p.hpMax * 0.4 : p.hpMax * 0.2); // ถ้าบัฟแล้วเพิ่มเป็น 2 เท่า
        if (p.hp > p.hpMax)
            p.hp = p.hpMax; // HP ไม่เกิน HPMax
        cout << p.name << " used Heal. HP is now " << p.hp << endl;
        break;

    case ATTACK_BOOST:
        // เพิ่มพลังโจมตี 2 เท่า
        p.attackPower *= (p.buffed ? 2 : 1); // ถ้าบัฟแล้วเพิ่มเป็น 2 เท่า
        cout << p.name << " used Attack Boost. Attack power is now " << p.attackPower << endl;
        // ตรวจสอบโอกาส Critical เมื่อใช้ Attack Boost
        p.checkCritical();
        break;

    case STEAL_SKILL:
        if (p.stolenSkill)
        {
            cout << p.name << " used " << p.stolenSkill->name << " which was stolen!" << endl;
            p.stolenSkill->applySkill(p);
        }
        else
        {
            cout << "No skill stolen!" << endl;
        }
        break;

    case SHIELD:
        // เปิดโล่
        if (p.buffed)
        {
            // ถ้ามีบัฟจะสามารถใช้โล่ได้ 2 ครั้ง
            cout << p.name << " used Shield twice due to Buff!" << endl;
            p.activateShield();
            p.activateShield();
        }
        else
        {
            p.activateShield();
        }
        break;

    case CHIP_BOOST:
        // เพิ่มเหรียญ 20% ของ chipMax
        p.addChip(p.buffed ? p.chipMax * 0.4 : p.chipMax * 0.2);
        cout << p.name << " used Chip Boost. Chip is now " << p.chip << endl;
        break;

    case BUFF_X2:
        // ใช้ Buff x2 ซึ่งจะเพิ่มผลลัพธ์ของสกิลในครั้งถัดไป
        p.buffed = true;
        cout << p.name << " used Buff x2! Next skill will be doubled!" << endl;
        break;

    case DEFENSE_BOOST:
        // เพิ่ม Defense 20
        p.defense += 20;
        cout << p.name << " used Defense Boost. Defense is now " << p.defense << endl;
        break;

    case CRITICAL_ATTACK:
        p.checkCritical();
        break;

    case HEALPASSIVE:
        // ฟื้นฟู HP 20%
        /* 
        p.attackPower += (p.buffed ? p.hpMax * 0.4 : p.hpMax * 0.2); // ถ้าบัฟแล้วเพิ่มเป็น 2 เท่า
        if (p.hp > p.hpMax)
            p.hp = p.hpMax; // HP ไม่เกิน HPMax
        cout << p.name << " used Heal. HP is now " << p.hp << endl; 
        */
        break;
    }
}

// ฟังก์ชันที่เช็คโอกาส Critical
void Player::checkCritical()
{
    float randomValue = (rand() % 100) / 100.0; // สุ่มค่า 0.0 - 1.0
    if (randomValue <= criticalChance)
    {
        isCritical = true;
        cout << name << " will make a Critical Attack!" << endl;
    }
    else
    {
        isCritical = false;
        cout << name << " made a normal attack." << endl;
    }
}

// ฟังก์ชันในการโจมตี
void Player::attack()
{
    float damage = attackPower; // ความเสียหายปกติ
    if (isCritical)
    {
        damage *= 2; // เพิ่มความเสียหายเป็น 2 เท่าเมื่อ Critical
        cout << name << " performed a Critical Attack! Damage is " << damage << endl;
    }
    else
    {
        cout << name << " performed a normal attack. Damage is " << damage << endl;
    }
}

// ฟังก์ชันขโมยสกิลจากฝ่ายตรงข้าม
void Player::stealSkill(Skill *opponentSkill)
{
    stolenSkill = opponentSkill; // เก็บสกิลที่ขโมยมาจากฝ่ายตรงข้าม
    cout << name << " stole " << opponentSkill->name << " from the opponent!" << endl;
}

// ฟังก์ชันในการเปิดโล่
void Player::activateShield()
{
    shieldActive = true;
    cout << name << " activated the Shield! Next attack will be blocked." << endl;
}

// ฟังก์ชันในการรับความเสียหาย
void Player::takeDamage(float damage)
{
    if (shieldActive)
    {
        // หากโล่ถูกเปิดใช้งานจะป้องกันการโจมตีทั้งหมด
        cout << name << " is protected by the Shield! No damage taken." << endl;
        shieldActive = false; // โล่จะหายไปหลังจากการโจมตีครั้งนี้
    }
    else
    {
        // ลดความเสียหายโดยการใช้ defense
        float realDamage = damage - (defense * 0.1); // คำนวณ damage ที่แท้จริง
        if (realDamage < 0)
            realDamage = 0; // ถ้าค่าความเสียหายน้อยกว่า 0 ก็ไม่ให้เป็นลบ
        hp -= realDamage;
        if (hp < 0)
            hp = 0;
        cout << name << " took " << realDamage << " damage. HP is now " << hp << endl;
    }
}

// ฟังก์ชันในการเพิ่มchip
void Player::addChip(int amount)
{
    chip += amount;
    if (chip > chipMax)
        chip = chipMax; // ป้องกันไม่ให้chipเกิน chipMax
}

// ฟังก์ชันที่จัดการกับการลด chip และตรวจสอบโอกาส 30% สำหรับการเพิ่ม HP และ chip
void Player::handleChipReduction()
{
    // ตรวจสอบโอกาส 30% ที่จะเกิดการเพิ่ม HP และ chip
    float randomValue = (rand() % 100) / 100.0; // สุ่มค่าระหว่าง 0.0 ถึง 1.0
    if (randomValue <= 0.3)
    { // โอกาส 30%
        // เพิ่ม HP 20% ของ HP Max
        hp += hpMax * 0.25;
        if (hp > hpMax)
            hp = hpMax; // ให้ HP ไม่เกินค่าของ HP Max
        cout << name << " gained 20% HP! HP is now " << hp << endl;

        // เพิ่ม chip 20% ของ chip Max
        chip += chipMax * 0.35;
        if (chip > chipMax)
            chip = chipMax; // ให้ chip ไม่เกินค่าของ chip Max
        cout << name << " gained 20% Chip! Chip is now " << chip << endl;
    }
}

// ฟังก์ชันในการแสดงสถานะของผู้เล่น
void Player::displayStatus()
{
    cout << name << "'s attack power: " << attackPower << ", HP: " << hp << "/" << hpMax << ", Defense: " << defense
         << ", Shield Active: " << (shieldActive ? "Yes" : "No") << ", Chip: " << chip << "/" << chipMax << ", Critical Chance: " << criticalChance * 100 << "%" << endl;
}

// ฟังก์ชันในการใช้สกิล
void Player::useSkill(Skill skill)
{
    if (chip > 0)
    {
        skill.applySkill(*this); // ใช้สกิล
        chip -= 1;               // ลด chip ลงทีละ 1 ทุกครั้งที่ใช้สกิล
        cout << name << " used a skill. chip reduced to " << chip << endl;
    }
    else
    {
        cout << "Not enough chips to use a skill!" << endl;
    }
}

// ฟังก์ชันรีเซ็ตสถานะบัฟ
void Player::resetBuff()
{
    buffed = false;
    cout << name << "'s Buff x2 effect has expired!" << endl;
}



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
        void handlePurchase(int ,coin&,Player&); // ฟังก์ชันที่จัดการการซื้อสินค้า
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
    
    void store::handlePurchase(int itemIndex, coin& playerMoney, Player& player) {
        if (!forsale[itemIndex].soldOut && playerMoney.mycoin >= forsale[itemIndex].cost) {
            // Deduct money
            playerMoney.mycoin -= forsale[itemIndex].cost;
    
            // Convert item name to a corresponding skill
            Skill::SkillType skillType;
            if (forsale[itemIndex].name == "HEALACTIVE") skillType = Skill::HEALACTIVE;
            else if (forsale[itemIndex].name == "ATTACK_BOOST") skillType = Skill::ATTACK_BOOST;
            else if (forsale[itemIndex].name == "STEAL_SKILL") skillType = Skill::STEAL_SKILL;
            else if (forsale[itemIndex].name == "SHIELD") skillType = Skill::SHIELD;
            else if (forsale[itemIndex].name == "CHIP_BOOST") skillType = Skill::CHIP_BOOST;
            else if (forsale[itemIndex].name == "BUFF_X2") skillType = Skill::BUFF_X2;
            else if (forsale[itemIndex].name == "DEFENSE_BOOST") skillType = Skill::DEFENSE_BOOST;
            else if (forsale[itemIndex].name == "CRITICAL_ATTACK") skillType = Skill::CRITICAL_ATTACK;
            else if (forsale[itemIndex].name == "HEALPASSIVE") skillType = Skill::HEALPASSIVE;
            else return; // Invalid skill, return early
    
            // Add skill to player's available skills
            player.availableSkills.push_back(Skill(forsale[itemIndex].name, skillType));
    
            // Mark item as sold out
            forsale[itemIndex].soldOut = true;
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
                //cerr << "Button Pressed" << endl;
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
Text checkCondition(char* CurrentArray, Font& font, int& chips, const int maxchips, Text& ChipsText, coin& money, Text& MoneyText, Player& player) {
    Text Status;
    Status.setFont(font);
    Status.setCharacterSize(35);
    Status.setFillColor(Color::White);
    Status.setPosition(380, 200);

    if ((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1] == CurrentArray[2])) {
        switch (CurrentArray[0]) {
            case 'C':
                Status.setString("Won " + to_string(maxchips * 2 / 10) + " Chips");
                chips = chips + (maxchips * 3 / 10);
                ChipsText.setString("Chips: " + to_string(chips));
                break;

            case 'M':
                Status.setString("Won " + to_string(10) + " $");
                money.addmoney(10);
                break;

            case 'S': {
                // Define the pool of skills
                vector<Skill> skillPool = {
                    Skill("DEFENSE_BOOST", Skill::DEFENSE_BOOST),
                    Skill("HEALACTIVE", Skill::HEALACTIVE),
                    Skill("CRITICAL_ATTACK", Skill::CRITICAL_ATTACK),
                    Skill("CHIP_BOOST", Skill::CHIP_BOOST)
                };

                // Select a random skill from the pool
                int randomIndex = rand() % skillPool.size();
                Skill randomSkill = skillPool[randomIndex];

                // Add the skill to the player's available skills
                player.availableSkills.push_back(randomSkill);

                Status.setString("Won Skill: " + randomSkill.name);
                break;
            }

            default:
                Status.setString("Won!");
                break;
        }
    } else if (!virgin) {
        Status.setString("Lost!");
    } else {
        Status.setString("Press to Spin!");
        virgin = false;
    }
    return Status;
}

void SpinSlot(int chips, char* RandomizedChar, vector<char> BaseChar, int vecsize, sf::RenderWindow& window, sf::Font& font, int maxchips,bool & FinishedPhase,coin &money,Player &player) {
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

    while (chips >= 1) {
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
        Status = checkCondition(RandomizedChar, font,chips,maxchips,ChipsText,money,MoneyText,player);

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





    Player player("Player1",10);//PASTEURSHIT



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
    item ITEMA = {"HEALACTIVE", 10, false};//
    item ITEMB = {"ATTACK_BOOST", 20, false};
    item ITEMC = {"STEAL_SKILL", 30, false};
    item ITEMD = {"SHIELD", 40, false};
    item ITEME = {"CHIP_BOOST", 50, false};//
    item ITEMF = {"BUFF_X2", 50, false};
    item ITEMG = {"DEFENSE_BOOST", 50, false};//
    item ITEMH = {"CRITICAL_ATTACK", 50, false};//
    item ITEMI = {"HEALPASSIVE", 50, false};
    
/*      HEALACTIVE,active
        ATTACK_BOOST,
        STEAL_SKILL,
        SHIELD,
        CHIP_BOOST,//active
        BUFF_X2,
        DEFENSE_BOOST,//active
        CRITICAL_ATTACK//active
        */
    // Add items to store
    //game.addItem(ITEMA);
    game.addItem(ITEMB);
    game.addItem(ITEMC);
    game.addItem(ITEMD);
    //game.addItem(ITEME);
    game.addItem(ITEMF);
    //game.addItem(ITEMG);
    //game.addItem(ITEMH);
    game.addItem(ITEMI);
    
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
   // vector<char> BaseChar = {'S'};
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
                    SpinSlot(chips, RandomizedChar, BaseChar, vecsize, window, font, maxchips, FinishedPhase, money,player);
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
                                    //money.mycoin -= game.forsale[i].cost;

                                    game.handlePurchase(i, money, player);//add player skill from shop
                                    for (int i = 0; i < player.availableSkills.size(); i++)
                                    {
                                        cerr << player.availableSkills[i].name << endl;
                                    }
                                    for (int i = 0; i < 9; i++)
                                    {
                                        cerr << endl;
                                    }
                                    
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