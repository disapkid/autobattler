#pragma once
#include <units.hpp>
#include <thread>
#include <abilities.hpp>
#include <chrono>
#include <unordered_map>
#include <string>

class Battle {
public:
    static void MonsterTurn(Monster& monster, Hero& hero, int round);
    static void HeroTurn(Hero& hero, Monster& monster, int round);
    static void Start_game();

private:
    static Hero ChooseHeroClass();
    static Monster GenerateMonster();
    static bool BattleRound(Hero& hero, Monster& monster, bool heroFirst);
    static bool RestartGame(int& winsInRow);
};

extern std::unordered_map<int, std::string> monsters;
