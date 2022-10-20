//
// Created by Yaroslav on 10/20/2022.
//

#ifndef LAB1_HAMMURABI_COMMAND_H
#define LAB1_HAMMURABI_COMMAND_H

#include "GameData.h"

template<typename T>
T ReadValue();

class AbstractCommand {
protected:
    GameData *gameData;

public:
    AbstractCommand(GameData *newData) : gameData(newData) {};

    virtual void Execute() = 0;
};

class StartYear : public AbstractCommand {
public:
    StartYear(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class EndYear : public AbstractCommand {
private:
    float deadPercentToLose = 0.45;
public:
    EndYear(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class FeedPeople : public AbstractCommand {
public:
    FeedPeople(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class WelcomePeople : public AbstractCommand {
public:
    WelcomePeople(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class TryPlague : public AbstractCommand {
private:
    const float plagueChance = 0.15f;
public:
    TryPlague(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class PrintPeople : public AbstractCommand {
public:
    PrintPeople(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class PrintWheat : public AbstractCommand {
private:
    const int areaPerPerson = 10;

public:
    PrintWheat(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class PrintRats : public AbstractCommand {
public:
    PrintRats(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class PrintArea : public AbstractCommand {
public:
    PrintArea(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class ReadBuyArea : public AbstractCommand {
public:
    ReadBuyArea(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class ReadSellArea : public AbstractCommand {
public:
    ReadSellArea(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class ReadEatWheat : public AbstractCommand {
private:
    const int wheatPerPerson = 20;

public:
    ReadEatWheat(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

class ReadSowArea : public AbstractCommand {
private:
    const int areaPerPerson = 10;
    const int areaPerWheat = 2;
public:
    ReadSowArea(GameData *newData) : AbstractCommand(newData) {};

    void Execute() override;
};

#endif //LAB1_HAMMURABI_COMMAND_H
