cmake_minimum_required(VERSION 3.14)
project(autobattler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(objects STATIC include/weapon.hpp
                           include/units.hpp
                           include/abilities.hpp
                           include/battle.hpp
                           src/battle.cpp
                           src/abilities.cpp
                           src/weapon.cpp
                           src/units.cpp) 
                         
target_include_directories(objects PUBLIC include)

add_executable(game src/main.cpp)

target_link_libraries(game PRIVATE objects)