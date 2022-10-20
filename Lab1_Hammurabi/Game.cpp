//
// Created by Yaroslav on 10/20/2022.
//

#include <iostream>
#include <string>
#include <fstream>

#include "Command.h"

using namespace std;

const char* saveFile = "save.txt";
const int maxRounds = 10;

//Better use Json serialization + hash key
GameData *TryLoadGame() {

    auto gameData = new GameData();
    ifstream save(saveFile);
    if (save.is_open()) {
        cout << "Обнаружено сохранение. Загрузить? (1 - да, 0 - нет) \n";
        if (ReadValue<int>()) {
            int year, people, wheat, area, areaToHarvest;
            float diedPercentSum;
            save >> year
                 >> people
                 >> wheat
                 >> area
                 >> areaToHarvest
                 >> diedPercentSum;

            gameData->setYear(year);
            gameData->setPeople(people);
            gameData->setWheat(wheat);
            gameData->setArea(area);
            gameData->areaToHarvest = areaToHarvest;
            gameData->peopleDiedPercentSum = diedPercentSum;
        }

        save.close();
    }

    return gameData;
}

//Better use Json serialization + hash key
void SaveGame(GameData *gameData) {
    ofstream save(saveFile);
    save << gameData->getYear() << "\n"
         << gameData->getPeople() << "\n"
         << gameData->getWheat() << "\n"
         << gameData->getArea() << "\n"
         << gameData->areaToHarvest << "\n"
         << gameData->peopleDiedPercentSum << "\n";
    save.close();
}

void GetScore(GameData *gameData) {
    auto P = gameData->peopleDiedPercentSum / maxRounds;
    auto L = gameData->getArea() / gameData->getPeople();

    if (P <= 0.03 && L >= 10) {
        cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше";
        return;
    }
    if (P <= 0.10 && L >= 9) {
        cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, \n"
                "но многие хотели бы увидеть вас во главе города снова";
        return;
    }
    if (P <= 0.33 && L >= 7) {
        cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с \n"
                "облегчением, и никто больше не желает видеть вас правителем";
        return;
    }

    cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, \n"
            "и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании";
}

int main() {
    system("chcp 65001");

    GameData *gameData = TryLoadGame();

    AbstractCommand *info[5] = {new StartYear(gameData),
                                new PrintPeople(gameData),
                                new PrintWheat(gameData),
                                new PrintRats(gameData),
                                new PrintArea(gameData)};

    AbstractCommand *read[4] = {new ReadBuyArea(gameData),
                                new ReadSellArea(gameData),
                                new ReadEatWheat(gameData),
                                new ReadSowArea(gameData)};

    AbstractCommand *events[4] = {new FeedPeople(gameData),
                                new WelcomePeople(gameData),
                                new TryPlague(gameData),
                                new EndYear(gameData)};

    while (gameData->getYear() < maxRounds && !gameData->gameLost) {
        for (AbstractCommand *cmd: info) {
            cmd->Execute();
        }

        for (AbstractCommand *cmd: read) {
            cmd->Execute();
        }

        for (AbstractCommand *cmd: events) {
            cmd->Execute();
        }

        SaveGame(gameData);
    }

    remove(saveFile);

    if (!gameData->gameLost){
        GetScore(gameData);
    }
}

