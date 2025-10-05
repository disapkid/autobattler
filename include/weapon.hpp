#pragma once
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, std::pair<int, std::string>> weapons;

class Weapon {
public:
    Weapon() = default;

    Weapon(const std::string& type_) : type(type_) {
        if(!type_.empty()) {
            damage = weapons[type_].first;
            damage_type = weapons[type_].second;
        } else {
            damage = 0;
            damage_type = "";
        }
    };

    int dmg() const;

    std::string t() const;

    std::string dmg_t() const;

private:
    int damage;

    std::string type;

    std::string damage_type;
};