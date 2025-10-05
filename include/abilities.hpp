#pragma once
#include <units.hpp>
#include <functional>
#include <string>
#include <unordered_map>

struct PairHash {
    size_t operator()(const std::pair<std::string,int>& p) const noexcept {
        size_t h1 = std::hash<std::string>{}(p.first);
        size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1<<6) + (h1>>2));
    }
};

using DamageModifier = std::function<int(Hero&, Monster&, int, std::string)>;
using DamageBlocker  = std::function<int(Hero&, Monster&, int, int, std::string)>;

struct AttackModifier {
    static int HiddenAttack(Hero&, Monster&, int, std::string);
    static int PoisonAttack(Hero&, Monster&, int, std::string);
    static int RageAttack(Hero&, Monster&, int, std::string);
    static int RushAttack(Hero&, Monster&, int, std::string);
    static int FireAttack(Hero&, Monster&, int, std::string);
    static int VoidChange(Hero&, Monster&, int, std::string);
};

struct AttackBlocker {
    static int StoneSkin(Hero&, Monster&, int, int, std::string);
    static int Shield(Hero&, Monster&, int, int, std::string);
    static int SkeletonWeak(Hero&, Monster&, int, int, std::string);
    static int SlimeBlock(Hero&, Monster&, int, int, std::string);
};

extern std::unordered_map<int, std::string> monsters;

extern std::unordered_map<std::pair<std::string,int>, DamageModifier, PairHash> herosAttackModifies;
extern std::unordered_map<std::pair<std::string,int>, DamageBlocker,   PairHash> herosBlock;

extern std::unordered_map<std::string, DamageModifier> monstersAttackModifies;
extern std::unordered_map<std::string, DamageBlocker>  monstersBlock;
