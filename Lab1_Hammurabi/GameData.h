//
// Created by Yaroslav on 10/20/2022.
//

#ifndef LAB1_HAMMURABI_GAMEDATA_H
#define LAB1_HAMMURABI_GAMEDATA_H

class GameData {

public:
    int areaPrice = 0;
    int peopleToFeed = 0;
    int areaToHarvest = 0;
    int wheatPerArea = 0;
    int peopleDied = 0;
    float peopleDiedPercent = 0;
    float peopleDiedPercentSum = 0;

    bool gameLost = false;

private:
    int year = 0;
    int people = 0;
    int wheat = 0;
    int area = 0;

public:
    GameData();

    int getYear() const;

    void setYear(int newYear);

    int getPeople() const;

    void setPeople(int newPeople);

    int getWheat() const;

    void setWheat(int newWheat);

    int getArea() const;

    void setArea(int newArea);
};


#endif //LAB1_HAMMURABI_GAMEDATA_H
