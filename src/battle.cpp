#include <battle.hpp>

static inline int clamp_nonneg(int x) { return x < 0 ? 0 : x; }

void Battle::MonsterTurn(Monster &monster, Hero &hero, int round)
{
    std::cout << "Ход монстра!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, monster.GetStats().agility + hero.GetStats().agility);

    int attackChance = dist(gen);
    if (attackChance <= hero.GetStats().agility) {
        std::cout << "Промах!" << std::endl;
        return;
    }

    int damage = monster.GetDamage();

    if (auto it = monstersAttackModifies.find(monster.GetType()); it != monstersAttackModifies.end()) {
        damage += it->second(hero, monster, round, "Монстр");
    }

    if (auto it = herosBlock.find({hero.GetType(), hero.GetLvl()}); it != herosBlock.end()) {
        damage = it->second(hero, monster, round, damage, "Монстр");
    }

    if (!hero.GetExtraType().empty()) {
        if (auto it = herosBlock.find({hero.GetExtraType(), hero.GetLvl() + 1}); it != herosBlock.end()) {
            damage = it->second(hero, monster, round, damage, "Монстр");
        }
    }

    damage = clamp_nonneg(damage);
    hero.TakeDamage(damage);
    std::cout << "Монстр наносит " << damage << " урона герою!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Battle::HeroTurn(Hero &hero, Monster &monster, int round)
{
    std::cout << "Ход героя!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, hero.GetStats().agility + monster.GetStats().agility);

    int attackChance = dist(gen);
    if (attackChance <= monster.GetStats().agility) {
        std::cout << "Промах!" << std::endl;
        return;
    }

    int damage = hero.GetDamage();
    if (auto it = herosAttackModifies.find({hero.GetType(), hero.GetLvl()}); it != herosAttackModifies.end()) {
        damage += it->second(hero, monster, round, "Герой");
    }
    if (!hero.GetExtraType().empty()) {
        if (auto jt = herosAttackModifies.find({hero.GetExtraType(), hero.GetLvl() + 1}); jt != herosAttackModifies.end()) {
            damage += jt->second(hero, monster, round, "Герой");
        }
    }
    if (auto kb = monstersBlock.find(monster.GetType()); kb != monstersBlock.end()) {
        damage = kb->second(hero, monster, round, damage, "Герой");
    }

    damage = clamp_nonneg(damage);
    monster.TakeDamage(damage);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "Герой наносит " << damage << " урона монстру!" << std::endl;

    if (monster.GetStats().HP <= 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "Монстр повержен!" << std::endl;

        if (hero.GetLvl() < 3) {
            hero.LvlUp();
            std::cout << "Герой повышает уровень до " << hero.GetLvl() << "!" << std::endl;
            hero.UpdateHP();

            if (hero.GetLvl() == 2) {
                std::cout << "Выберите дополнительный класс: Воин - 1, Разбойник - 2, Варвар - 3" << std::endl;
                int choice = 0;
                std::unordered_map<int, std::string> classes = {
                    {1, "Воин"},
                    {2, "Разбойник"},
                    {3, "Варвар"}
                };
                while (true) {
                    if (!(std::cin >> choice) || classes.find(choice) == classes.end()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Неверный ввод, попробуйте снова: ";
                        continue;
                    }
                    break;
                }
                hero.Gain_Extra(classes[choice]);
            }
        }

        hero.Heal();
        std::cout << "Герой восстанавливает здоровье!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::cout << "Из монстра выпадает " << monster.Loot().t() << "!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::cout << "Заменить оружие? (y/n)" << std::endl;
        char choice = 'n';
        if (std::cin >> choice; choice == 'y' || choice == 'Y') {
            hero.ChangeWeapon(monster.Loot());
            std::cout << "Оружие заменено на " << hero.GetWeapon().t() << "!" << std::endl;
        }
    }
}

Hero Battle::ChooseHeroClass() {
    std::unordered_map<int, std::string> classes = {
        {1, "Воин"},
        {2, "Разбойник"},
        {3, "Варвар"}
    };
    int choice = 0;
    while (true) {
        std::cout << "Выберите класс героя: Воин - 1, Разбойник - 2, Варвар - 3" << std::endl;
        if (!(std::cin >> choice) || classes.find(choice) == classes.end()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод, попробуйте снова.\n";
            continue;
        }
        break;
    }
    return Hero(classes[choice]);
}

Monster Battle::GenerateMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 6);
    return Monster(monsters[dist(gen)]);
}

bool Battle::BattleRound(Hero& hero, Monster& monster, bool heroFirst) {
    int round = 1;
    while (hero.GetStats().HP > 0 && monster.GetStats().HP > 0) {
        if (heroFirst) {
            HeroTurn(hero, monster, round);
            if (monster.GetStats().HP <= 0) return true;
            MonsterTurn(monster, hero, round);
            if (hero.GetStats().HP <= 0) return false;
        } else {
            MonsterTurn(monster, hero, round);
            if (hero.GetStats().HP <= 0) return false;
            HeroTurn(hero, monster, round);
            if (monster.GetStats().HP <= 0) return true;
        }
        ++round;
    }
    return hero.GetStats().HP > 0;
}

bool Battle::RestartGame(int& winsInRow) {
    std::cout << "Игра окончена! Вы победили " << winsInRow << " монстров подряд!" << std::endl;
    std::cout << "Начать заново? (y/n)" << std::endl;
    char restart = 'n';
    if (std::cin >> restart; restart == 'y' || restart == 'Y') {
        winsInRow = 0;
        return true;
    }
    return false;
}

void Battle::Start_game() {
    int winsInRow = 0;
    while (winsInRow < 5) {
        Hero hero = ChooseHeroClass();
        while (hero.GetStats().HP > 0) {
            std::cout << "Раунд " << (winsInRow + 1) << std::endl;
            Monster monster = GenerateMonster();
            std::cout << "Появляется монстр: " << monster.GetType() << "!" << std::endl;
            const bool heroFirst = (hero.GetStats().agility >= monster.GetStats().agility);
            if (BattleRound(hero, monster, heroFirst)) {
                ++winsInRow;
                if(winsInRow == 5) {
                    std::cout << "Поздравляем! Вы победили всех монстров!" << std::endl;
                    if (!RestartGame(winsInRow)) return;
                    break;
                }
            } else {
                if (!RestartGame(winsInRow)) return;
                break;
            }
        }
    }
}
