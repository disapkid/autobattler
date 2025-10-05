#pragma once
#include <weapon.hpp>
#include <random>
#include <iostream>

struct Stats
{
    Stats() = default;

    Stats(int a, int b, int c, int d = 1) : strength(a), agility(b), stamina(c), HP(d) {};

    int strength;

    int agility;

    int stamina;

    int HP;
};

extern std::unordered_map<std::string, std::string> heros_weapon;
extern std::unordered_map<std::string, Stats> monster_stats;
extern std::unordered_map<std::string, int> monster_dmg;
extern std::unordered_map<std::string, Weapon> monster_loot;
extern std::unordered_map<std::string, int> heros_HP_gain;

class Monster {
public:
    Monster() = default;

    Monster(const std::string& type_) : type(type_) {
        stats = monster_stats[type_];
        damage = monster_dmg[type_];
    }

    Weapon Loot() const;

    Stats GetStats() const;

    void TakeDamage(int damage);

    std::string GetType() const;

    int GetDamage() const;
private:
    Stats stats;

    std::string type;

    int damage;
};

class Hero {
public:
    Hero() = default;

    Hero(const std::string& type_) : type(type_) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1,3);

        stats = Stats(dist(gen), dist(gen), dist(gen), heros_HP_gain[type_]);

        weapon = Weapon(heros_weapon[type_]);

        stats.HP += stats.stamina;

        maxHp = stats.HP;
    }

    void Gain_Extra(const std::string& extra_type_);

    int GetDamage() const;

    void UpdateHP();

    void Heal();

    int GetLvl() const;

    int GetExtraLvl() const;

    void LvlUp();

    void TakeDamage(int damage);

    Weapon GetWeapon() const;

    std::string GetType() const;

    std::string GetExtraType() const;

    Stats GetStats() const;

    void ChangeStats(const std::string& stat, int value);

    void ChangeWeapon(const Weapon& new_weapon);
    
    ~Hero() = default;
private:
    Stats stats;

    int lvl = 1;

    int extra_lvl;

    int maxHp;

    std::string extra_type;

    std::string type;

    Weapon weapon;
};