cmake_minimum_required(VERSION 3.14)
project(pta)

set(CMAKE_CXX_STANDARD 14)

add_executable(pta
        tpc/a-easy-task.cc
        tpc/b-easy-number-game.cc
        tpc/c-how-many-nines.cc
        tpc/c-how-many-nines-table.cc
        tpc/d-magic-multiplication.cc
        tpc/main.cc)
