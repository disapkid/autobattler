#include <weapon.hpp>

int Weapon::dmg() const {
    return Weapon::damage;
}

std::string Weapon::t() const {
    return Weapon::type;
}

std::string Weapon::dmg_t() const {
    return Weapon::damage_type;
}

std::unordered_map<std::string, std::pair<int, std::string>> weapons {
    { "Меч",             { 3, "Рубящий" } },
    { "Дубина",          { 3, "Дробящий" } },
    { "Кинжал",          { 2, "Колющий" } },
    { "Топор",           { 4, "Рубящий" } },
    { "Копье",           { 3, "Колющий" } },
    { "Легендарный меч", { 10, "Рубящий" } }
};