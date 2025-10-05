#include <abilities.hpp>
#include <unordered_map>
#include <string>

static inline int clamp_nonneg(int x) { return x < 0 ? 0 : x; }

std::unordered_map<int, std::string> monsters{
    {1, "Гоблик"}, {2, "Скелет"}, {3, "Слайм"},
    {4, "Призрак"}, {5, "Голем"}, {6, "Дракон"}
};

int AttackModifier::HiddenAttack(Hero& hero, Monster& monster, int, std::string attacker) {
    int add = 0;
    if (attacker == "Герой") {
        if (hero.GetStats().agility > monster.GetStats().agility) ++add;
    } else {
        if (monster.GetStats().agility > hero.GetStats().agility) ++add;
    }
    return add;
}

int AttackModifier::PoisonAttack(Hero&, Monster&, int round, std::string) { return round; }
int AttackModifier::RageAttack(Hero&, Monster&, int round, std::string) { return (round <= 3) ? 2 : -1; }
int AttackModifier::RushAttack(Hero& hero, Monster&, int round, std::string) { return (round == 1) ? hero.GetDamage() : 0; }
int AttackModifier::FireAttack(Hero&, Monster&, int round, std::string) { return (round % 3 == 0) ? 3 : 0; }
int AttackModifier::VoidChange(Hero&, Monster&, int, std::string) { return 0; }

int AttackBlocker::StoneSkin(Hero& hero, Monster& monster, int, int damage, std::string attacker) {
    if (attacker == "Герой") damage -= monster.GetStats().stamina;
    else damage -= hero.GetStats().stamina;
    return clamp_nonneg(damage);
}
int AttackBlocker::Shield(Hero& hero, Monster& monster, int, int damage, std::string) {
    if (hero.GetStats().strength > monster.GetStats().strength) damage -= 3;
    return clamp_nonneg(damage);
}
int AttackBlocker::SkeletonWeak(Hero& hero, Monster&, int, int damage, std::string) {
    return (hero.GetWeapon().dmg_t() == "Дробящий") ? damage * 2 : damage;
}
int AttackBlocker::SlimeBlock(Hero& hero, Monster&, int, int damage, std::string) {
    if (hero.GetWeapon().dmg_t() == "Рубящий") damage -= hero.GetWeapon().dmg();
    return clamp_nonneg(damage);
}

std::unordered_map<std::pair<std::string,int>, DamageModifier, PairHash> herosAttackModifies{
    {{"Разбойник",1}, [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::HiddenAttack(h,m,r,a); }},
    {{"Разбойник",2}, [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::VoidChange(h,m,r,a); }},
    {{"Разбойник",3}, [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::PoisonAttack(h,m,r,a); }},
    {{"Воин",1},     [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::RushAttack(h,m,r,a); }},
    {{"Воин",3},     [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::VoidChange(h,m,r,a); }},
    {{"Варвар",1},   [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::RageAttack(h,m,r,a); }},
    {{"Варвар",3},   [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::VoidChange(h,m,r,a); }},
};

std::unordered_map<std::pair<std::string,int>, DamageBlocker, PairHash> herosBlock{
    {{"Воин",2},   [](Hero& h, Monster& m, int r, int dmg, std::string a){ return AttackBlocker::Shield(h,m,r,dmg,a); }},
    {{"Варвар",2}, [](Hero& h, Monster& m, int r, int dmg, std::string a){ return AttackBlocker::StoneSkin(h,m,r,dmg,a); }},
};

std::unordered_map<std::string, DamageModifier> monstersAttackModifies{
    {"Гоблик",  [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::VoidChange(h,m,r,a); }},
    {"Призрак", [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::HiddenAttack(h,m,r,a); }},
    {"Дракон",  [](Hero& h, Monster& m, int r, std::string a){ return AttackModifier::FireAttack(h,m,r,a); }},
};

std::unordered_map<std::string, DamageBlocker> monstersBlock{
    {"Скелет", [](Hero& h, Monster& m, int r, int dmg, std::string a){ return AttackBlocker::SkeletonWeak(h,m,r,dmg,a); }},
    {"Слайм",  [](Hero& h, Monster& m, int r, int dmg, std::string a){ return AttackBlocker::SlimeBlock(h,m,r,dmg,a); }},
    {"Голем",  [](Hero& h, Monster& m, int r, int dmg, std::string a){ return AttackBlocker::StoneSkin(h,m,r,dmg,a); }},
};
