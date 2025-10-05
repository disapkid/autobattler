#include <units.hpp>

std::unordered_map<std::string, std::string> heros_weapon {
    { "Разбойник", "Кинжал"},
    { "Воин",      "Меч"},
    { "Варвар",    "Дубина"}  
};

std::unordered_map<std::string, Stats> monster_stats {
    { "Гоблик",  {1,1,1,5}},
    { "Скелет",  {2,2,1,10}},
    { "Слайм",   {3,1,2,8}},
    { "Призрак", {1,3,1,6}},
    { "Голем",   {3,1,3,10}},
    { "Дракон",  {3,3,3,20}}
};

std::unordered_map<std::string, int> monster_dmg {
    { "Гоблик",  2},
    { "Скелет",  2},
    { "Слайм",   1},
    { "Призрак", 3},
    { "Голем",   1},
    { "Дракон",  4}
};

std::unordered_map<std::string, Weapon> monster_loot {
    { "Гоблик",  Weapon("Кинжал")},
    { "Скелет",  Weapon("Дубина")},
    { "Слайм",   Weapon("Копье")},
    { "Призрак", Weapon("Меч")},
    { "Голем",   Weapon("Топор")},
    { "Дракон",  Weapon("Легендарный меч")}
};

std::unordered_map<std::string, int> heros_HP_gain {
    { "Разбойник", 4},
    { "Воин",      5},
    { "Варвар",    6}
};

void Hero::Gain_Extra(const std::string &extra_type_)
{
    extra_type = extra_type_;
    extra_lvl = lvl + 1 <=2 ? lvl + 1 : lvl;
}

int Hero::GetDamage() const
{
    int result = Hero::stats.strength + Hero::weapon.dmg();
    return result;
}

void Hero::UpdateHP()
{
    Hero::maxHp += heros_HP_gain[Hero::type];
}

void Hero::Heal()
{
    stats.HP = maxHp;
}

int Hero::GetLvl() const
{
    return Hero::lvl;
}

int Hero::GetExtraLvl() const
{
    return Hero::extra_lvl;
}

void Hero::LvlUp()
{
    Hero::lvl++;
}

void Hero::TakeDamage(int damage) 
{   
    stats.HP -= damage;
}

Weapon Hero::GetWeapon() const
{
    return Weapon();
}

std::string Hero::GetType() const
{
    return Hero::type;
}

std::string Hero::GetExtraType() const
{
    return Hero::extra_type;
}

Stats Hero::GetStats() const
{
    return Hero::stats;
}

void Hero::ChangeStats(const std::string &stat, int value)
{
    if(stat == "сила") {
        stats.strength += value;
    } else if(stat == "ловкость") {
        stats.agility += value;
    } else if(stat == "выносливость") {
        stats.stamina += value;
    } 
}

void Hero::ChangeWeapon(const Weapon &new_weapon)
{
    Hero::weapon = new_weapon;
}

Weapon Monster::Loot() const
{
    return monster_loot[type];
}

Stats Monster::GetStats() const
{
    return Monster::stats;
}

void Monster::TakeDamage(int damage) 
{
    Monster::stats.HP -= damage;
}

std::string Monster::GetType() const
{
    return Monster::type;
}

int Monster::GetDamage() const
{
    return Monster::damage;
}
