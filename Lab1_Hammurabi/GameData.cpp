//
// Created by Yaroslav on 10/20/2022.
//

#include "GameData.h"

GameData::GameData() {
    year = 0;
    people = 100;
    wheat = 2800;
    area = 1000;
}

int GameData::getYear() const {
    return year;
}

void GameData::setYear(int newYear) {
    GameData::year = newYear;
}

int GameData::getPeople() const {
    return people;
}

void GameData::setPeople(int newPeople) {
    GameData::people = newPeople;
}

int GameData::getWheat() const {
    return wheat;
}

void GameData::setWheat(int newWheat) {
    GameData::wheat = newWheat;
}

int GameData::getArea() const {
    return area;
}

void GameData::setArea(int newArea) {
    GameData::area = newArea;
}

