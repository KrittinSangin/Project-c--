#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<iomanip>

using namespace std;

class Equipment{
    int hpmax, atk, def;
public:
    Equipment(int, int, int);
    vector<int> getStat();
};

Equipment::Equipment(int hmax, int a, int d) : hpmax(hmax), atk(a), def(d) {}

vector<int> Equipment::getStat(){
    return {hpmax, atk, def};
}

class Unit{
    string name, type, stolenSkill;
    int hp, hpmax, atk, def, chip, turnCount;
    bool guard_on, dodge_on, immortal_on;
    Equipment *equipment;
public:
    Unit(string, string);
    void showStatus();
    void newTurn();
    int attack(Unit &);
    int ultimateAttack(Unit &);
    int lifeStealAttack(Unit &);
    int criticalStrike(Unit &);
    int beAttacked(int);
    int heal();
    void guard();
    void dodge();
    void activateImmortal();
    bool isDead();
    void equip(Equipment *);
    void stealSkill(string);
    void stealChip(Unit &);
    void useSkill(string, Unit &);
    void useStolenSkill(Unit &);
};

Unit::Unit(string t, string n) : type(t), name(n), equipment(NULL), stolenSkill(""), immortal_on(false), turnCount(0) {
    if(type == "Hero"){
        hpmax = rand()%20 + 120;
        atk = rand()%5 + 14;
        def = rand()%3 + 9;
        chip = rand() % 100 + 50;
    } else {
        hpmax = rand()%20 + 250;
        atk = rand()%5 + 25;
        def = rand()%3 + 5;
        chip = rand() % 200 + 100;
    }
    hp = hpmax;
    guard_on = dodge_on = false;
}

void Unit::showStatus(){
    cout << "---------------------------------------\n" << name;
    cout << "\nHP: " << hp << "/" << hpmax << "\tATK: " << atk << "\tDEF: " << def;
    cout << "\nGold: " << chip;
    cout << "\n---------------------------------------\n";
}

void Unit::newTurn(){
    guard_on = dodge_on = immortal_on = false;
    turnCount++;
}

int Unit::beAttacked(int oppatk){
    if(immortal_on){
        cout << name << " is IMMORTAL! No damage taken!\n";
        return 0;
    }

    int dmg = 0;
    if(oppatk > def){
        dmg = oppatk - def;
        if(guard_on) dmg /= 3;
    }
    if(dodge_on){
        if(rand() % 2 == 0){
            cout << name << " dodged the attack!\n";
            dmg = 0;
        } else {
            cout << name << " failed to dodge!\n";
            dmg *= 2;
        }
    }
    hp = max(hp - dmg, 0);
    cout << name << " took " << dmg << " damage!\n";
    return dmg;
}

int Unit::attack(Unit &opp){ return opp.beAttacked(atk); }
int Unit::ultimateAttack(Unit &opp){ return opp.beAttacked(atk * 2); }

int Unit::lifeStealAttack(Unit &opp){
    int dmg = opp.beAttacked(atk);
    int healAmount = dmg / 2;
    hp = min(hp + healAmount, hpmax);
    cout << name << " drained " << healAmount << " HP from " << opp.name << "!\n";
    return dmg;
}

int Unit::criticalStrike(Unit &opp){
    if(rand() % 4 == 0){ 
        cout << name << " landed a CRITICAL STRIKE!\n";
        return opp.beAttacked(atk * 1.5);
    } else {
        return attack(opp);
    }
}

int Unit::heal(){
    int h = rand()%21 + 10;
    h = min(h, hpmax - hp);
    hp += h;
    return h;
}

void Unit::guard(){ guard_on = true; }
void Unit::dodge(){ dodge_on = true; }
void Unit::activateImmortal(){
    immortal_on = true;
    cout << name << " has become IMMORTAL for this turn!\n";
}
bool Unit::isDead(){ return hp <= 0; }

void Unit::equip(Equipment *Neweq){
    if(equipment){
        vector<int> oldstat = equipment->getStat();
        hpmax -= oldstat[0]; atk -= oldstat[1]; def -= oldstat[2];
    }
    equipment = Neweq;
    vector<int> newstat = equipment->getStat();
    hpmax += newstat[0]; atk += newstat[1]; def += newstat[2];
    hp = min(hp, hpmax);
}

void Unit::stealSkill(string skill){
    stolenSkill = skill;
    cout << name << " has stolen " << skill << "!\n";
}

void Unit::stealChip(Unit &target){
    int stolenAmount = rand() % 20 + 10;
    if(target.chip >= stolenAmount){
        target.chip -= stolenAmount;
        chip += stolenAmount;
        cout << name << " stole " << stolenAmount << " chip from " << target.name << "!\n";
    } else {
        cout << target.name << " has no chip to steal!\n";
    }
}

void Unit::useSkill(string skill, Unit &target){
    cout << name << " used " << skill << "!\n";
    if(skill == "Heal") heal();
    else if(skill == "Attack") attack(target);
    else if(skill == "Immortal") activateImmortal();
    else if(skill == "LifeSteal") lifeStealAttack(target);
    else if(skill == "CriticalStrike") criticalStrike(target);
    else if(skill == "RemoveGold") stealChip(target);
    else if(skill == "ReduceToOne"){
        target.hp = 1;
        cout << target.name << "'s HP is reduced to 1!\n";
    }
    if(target.type == "Monster" && rand()%3 == 0) target.stealSkill(skill);
}
