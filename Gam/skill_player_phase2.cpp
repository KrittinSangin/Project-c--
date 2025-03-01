#include <iostream>
#include <vector>
#include <cstdlib> // สำหรับการสุ่ม
#include <ctime>   // สำหรับการตั้งค่า seed ของการสุ่ม
using namespace std;

// Class สำหรับจัดการสกิล
class Skill
{
public:
    string name;
    enum SkillType
    {
        HEAL,
        ATTACK_BOOST,
        STEAL_SKILL,
        SHIELD,
        SHIP_BOOST,
        BUFF_X2,
        DEFENSE_BOOST,
        CRITICAL_ATTACK
    } type;

    Skill(string n, SkillType t) : name(n), type(t) {}

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
    int ship;                      // จำนวนเหรียญที่ผู้เล่นมี
    int shipMax;                   // จำนวนเหรียญสูงสุดที่สามารถสะสมได้
    bool buffed;                   // เช็คว่าอยู่ในสถานะบัฟหรือไม่
    vector<Skill> availableSkills; // สกิลที่ผู้เล่นสามารถเลือกใช้
    Skill *stolenSkill;            // สกิลที่ขโมยจากฝ่ายตรงข้าม
    float criticalChance;          // โอกาสในการโจมตีแบบ Critical (20%)
    bool isCritical;               // ฟีเจอร์เพื่อเก็บสถานะ Critical
    void handleShipReduction();    // ฟังก์ชันจัดการการลด ship

    Player(string n, float power) : name(n), attackPower(power), hp(100), hpMax(100), defense(10), shieldActive(false), ship(10), shipMax(100), buffed(false), stolenSkill(nullptr), criticalChance(0.2), isCritical(false) {}

    void useSkill(Skill skill);
    void stealSkill(Skill *opponentSkill);
    void takeDamage(float damage);
    void displayStatus();
    void activateShield();    // ฟังก์ชันในการเปิดโล่
    void addShip(int amount); // ฟังก์ชันเพิ่มเหรียญ
    void resetBuff();         // ฟังก์ชันในการรีเซ็ตสถานะบัฟ
    void checkCritical();     // ฟังก์ชันเช็คโอกาสการโจมตีแบบ Critical
    void attack();            // ฟังก์ชันโจมตี
};

// ฟังก์ชันที่ทำงานกับแต่ละสกิล
void Skill::applySkill(Player &p)
{
    switch (type)
    {
    case HEAL:
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

    case SHIP_BOOST:
        // เพิ่มเหรียญ 20% ของ shipMax
        p.addShip(p.buffed ? p.shipMax * 0.4 : p.shipMax * 0.2);
        cout << p.name << " used Ship Boost. Ship is now " << p.ship << endl;
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

// ฟังก์ชันในการเพิ่มship
void Player::addShip(int amount)
{
    ship += amount;
    if (ship > shipMax)
        ship = shipMax; // ป้องกันไม่ให้shipเกิน shipMax
}

// ฟังก์ชันที่จัดการกับการลด ship และตรวจสอบโอกาส 30% สำหรับการเพิ่ม HP และ ship
void Player::handleShipReduction()
{
    // ตรวจสอบโอกาส 30% ที่จะเกิดการเพิ่ม HP และ ship
    float randomValue = (rand() % 100) / 100.0; // สุ่มค่าระหว่าง 0.0 ถึง 1.0
    if (randomValue <= 0.3)
    { // โอกาส 30%
        // เพิ่ม HP 20% ของ HP Max
        hp += hpMax * 0.25;
        if (hp > hpMax)
            hp = hpMax; // ให้ HP ไม่เกินค่าของ HP Max
        cout << name << " gained 20% HP! HP is now " << hp << endl;

        // เพิ่ม ship 20% ของ ship Max
        ship += shipMax * 0.35;
        if (ship > shipMax)
            ship = shipMax; // ให้ ship ไม่เกินค่าของ ship Max
        cout << name << " gained 20% Ship! Ship is now " << ship << endl;
    }
}

// ฟังก์ชันในการแสดงสถานะของผู้เล่น
void Player::displayStatus()
{
    cout << name << "'s attack power: " << attackPower << ", HP: " << hp << "/" << hpMax << ", Defense: " << defense
         << ", Shield Active: " << (shieldActive ? "Yes" : "No") << ", Ship: " << ship << "/" << shipMax << ", Critical Chance: " << criticalChance * 100 << "%" << endl;
}

// ฟังก์ชันในการใช้สกิล
void Player::useSkill(Skill skill)
{
    if (ship > 0)
    {
        skill.applySkill(*this); // ใช้สกิล
        ship -= 1;               // ลด ship ลงทีละ 1 ทุกครั้งที่ใช้สกิล
        cout << name << " used a skill. ship reduced to " << ship << endl;
    }
    else
    {
        cout << "Not enough ships to use a skill!" << endl;
    }
}

// ฟังก์ชันรีเซ็ตสถานะบัฟ
void Player::resetBuff()
{
    buffed = false;
    cout << name << "'s Buff x2 effect has expired!" << endl;
}

int main()
{
    // สร้างผู้เล่น
    Player player("John", 100);
    Player opponent("Enemy", 100);

    // สร้างสกิลที่ผู้เล่นและฝ่ายตรงข้ามสามารถเลือกได้
    Skill healSkill("Heal", Skill::HEAL);
    Skill attackBoostSkill("Attack Boost", Skill::ATTACK_BOOST);
    Skill stealSkill("Steal Skill", Skill::STEAL_SKILL);
    Skill shieldSkill("Shield", Skill::SHIELD);
    Skill shipBoostSkill("Ship Boost", Skill::SHIP_BOOST); // สกิลเพิ่มเหรียญ
    Skill buffX2Skill("Buff x2", Skill::BUFF_X2);          // สกิลบัฟ x2
    Skill defenseBoostSkill("Defense Boost", Skill::DEFENSE_BOOST);

    // ลด ship และตรวจสอบว่าโอกาส 30% จะเกิดขึ้นหรือไม่
    player.ship -= 1;             // ลด ship
    player.handleShipReduction(); // ตรวจสอบการเพิ่ม HP และ ship

    // ผู้เล่นใช้สกิลเพิ่ม Defense
    player.useSkill(defenseBoostSkill);

    // ผู้เล่นเลือกใช้สกิลบัฟ x2
    player.useSkill(buffX2Skill);

    // ฝ่ายตรงข้ามเลือกใช้สกิล
    opponent.availableSkills.push_back(healSkill);
    opponent.availableSkills.push_back(attackBoostSkill);

    // แสดงสถานะของฝ่ายตรงข้ามก่อน
    opponent.displayStatus();

    // ผู้เล่นขโมยสกิลจากฝ่ายตรงข้าม
    player.stealSkill(&opponent.availableSkills[0]); // ขโมยสกิล Heal

    // แสดงสถานะของผู้เล่น
    player.displayStatus();

    // ผู้เล่นใช้สกิลเพิ่มเหรียญ
    player.useSkill(shipBoostSkill);

    // แสดงสถานะหลังจากใช้สกิล
    player.displayStatus();

    return 0;
}
