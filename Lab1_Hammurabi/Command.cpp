//
// Created by Yaroslav on 10/20/2022.
//
#include <iostream>
#include <string>
#include <cmath>

#include "Command.h"

using namespace std;

template<class T>
T ReadValue() {
    T value;
    while (true) {
        if (cin >> value) {
            break;
        } else {
            cout << "Введены данные в неверном формате: \n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    }
    return value;
}

void StartYear::Execute() {
    auto newYear = gameData->getYear() + 1;
    gameData->setYear(newYear);

    cout << " Мой повелитель, позволь поведать тебе \n";
    cout << "\t В году " << newYear << " твоего высочайшего правления\n";
}

void PrintPeople::Execute() {
    cout << "\t Население города составляет " << gameData->getPeople() << " человек \n";
}

void PrintWheat::Execute() {
    if (gameData->getYear() == 1) {
        cout << "\t В амбарах находится " << gameData->getWheat() << " бушелей пшеницы \n";
        return;
    }

    gameData->wheatPerArea = rand() % 6 + 1;
    auto maxArea = gameData->getPeople() * areaPerPerson;
    auto harvestedArea = min(maxArea, gameData->areaToHarvest);
    auto harvestedWheat = harvestedArea * gameData->wheatPerArea;

    cout << "\t Мы собрали " << harvestedWheat << " бушелей пшеницы, по " << gameData->wheatPerArea
         << " бушеля с акра \n";
    gameData->setWheat(gameData->getWheat() + harvestedWheat);
}

void PrintRats::Execute() {
    if (gameData->getYear() == 1) {
        return;
    }

    auto ratCoefficient = rand() % 8;
    auto currentWheat = gameData->getWheat();

    auto eatenWheat = ratCoefficient * currentWheat / 100;
    gameData->setWheat(currentWheat - eatenWheat);

    cout << " \t Крысы истребили " << eatenWheat << " бушелей пшеницы, оставив " << gameData->getWheat()
         << " бушеля в амбарах \n";
}

void PrintArea::Execute() {
    gameData->areaPrice = rand() % 10 + 17;
    cout << "\t Город занимает " << gameData->getArea() << " акров \n";
    cout << "\t 1 акр земли стоит " << gameData->areaPrice << " бушель \n";
}

void ReadBuyArea::Execute() {
    auto currentWheat = gameData->getWheat();
    cout << "\t Сколько акров земли повелеваешь купить? \n";

    while (true) {
        auto value = ReadValue<int>();
        auto price = value * gameData->areaPrice;
        if (price >= 0 && price <= currentWheat) {
            gameData->setWheat(currentWheat - price);
            gameData->setArea(gameData->getArea() + value);
            break;
        } else if (price < 0) {
            cout << "\t Введите корректное значение... \n";
        } else {
            cout << "\t Повелитель, у нас нет столько пшеницы, обдумайте еще... \n";
        }
    }
}

void ReadSellArea::Execute() {
    auto currentArea = gameData->getArea();
    cout << "\t Сколько акров земли повелеваешь продать? \n";

    while (true) {
        auto value = ReadValue<int>();
        if (value >= 0 && value <= currentArea) {
            gameData->setArea(gameData->getArea() - value);
            gameData->setWheat(gameData->getWheat() + value * gameData->areaPrice);
            break;
        } else if (value < 0) {
            cout << "\t Введите корректное значение... \n";
        } else {
            cout << "\t Повелитель, у нас нет столько земли, обдумайте еще... \n";
        }
    }
}

void ReadEatWheat::Execute() {
    auto currentWheat = gameData->getWheat();
    cout << "\t Сколько бушелей пшеницы повелеваешь съесть? \n";

    while (true) {
        auto value = ReadValue<int>();
        if (value >= 0 && value <= currentWheat) {
            gameData->peopleToFeed = value / wheatPerPerson;
            gameData->setWheat(currentWheat - value);
            break;
        } else if (value < 0) {
            cout << "\t Введите корректное значение... \n";
        } else {
            cout << "\t Повелитель, у нас нет столько пшеницы, обдумайте еще... \n";
        }
    }
}

void ReadSowArea::Execute() {
    auto currentArea = gameData->getArea();
    cout << "\t Сколько акров земли повелеваешь засеять? \n";

    while (true) {
        auto value = ReadValue<int>();
        if (value >= 0 && value <= currentArea) {
            auto wheatPrice = value / areaPerWheat;
            if (value / areaPerPerson > gameData->getPeople()) {
                cout << "\t Повелитель, сжалься, у нас нет столько людей... \n";
            } else if (wheatPrice > gameData->getWheat()) {
                cout << "\t Повелитель, у нас нет столько пшеницы, обдумайте еще... \n";
            } else {
                gameData->setWheat(gameData->getWheat() - wheatPrice);
                gameData->areaToHarvest = value;
                break;
            }
        } else if (value < 0) {
            cout << "\t Введите корректное значение... \n";
        } else {
            cout << "\t Повелитель, у нас нет столько земли, обдумайте еще... \n";
        }
    }
}

void FeedPeople::Execute() {
    auto currentPeople = gameData->getPeople();
    gameData->peopleDied = currentPeople - gameData->peopleToFeed;
    gameData->peopleDiedPercent = (float) gameData->peopleDied / (float) currentPeople;

    gameData->setPeople(gameData->peopleToFeed);
    cout << gameData->peopleDied << "\t человек умерли с голоду \n";
}

void WelcomePeople::Execute() {
    auto deadValue = gameData->peopleDied / 2;
    auto harvestValue = (5 + gameData->wheatPerArea) * gameData->getWheat() / 600;
    auto totalValue = deadValue + harvestValue + 1;
    auto newPeople = totalValue > 50 ? 50 : 0;
    gameData->setPeople(gameData->getPeople() + newPeople);
    cout << newPeople << "\t человек прибыли в наш великий город \n";
}

void TryPlague::Execute() {
    auto doPlague = rand() % 101 <= plagueChance * 100;
    if (doPlague) {
        gameData->setPeople(gameData->getPeople() / 2);
        cout << "\t Чума уничтожила половинну населения \n";
    }
}

void EndYear::Execute() {
    gameData->peopleDiedPercentSum += gameData->peopleDiedPercent;

    if (gameData->peopleDiedPercent > deadPercentToLose || gameData->getPeople() == 0) {
        cout << "Ну гг, вы облажались и на вас настучали в ООН. Game Over \n";
        gameData->gameLost = true;
    }
}
