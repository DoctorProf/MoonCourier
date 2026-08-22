#include "../include/Cell.h"

float getCellTraversalCost(CellType type)
{
    switch (type)
    {
    case CellType::Regolith:
        return 1.0f;

    case CellType::LooseDust:
        return 1.5f;

    case CellType::CompactGround:
        return 0.8f;

    case CellType::RockyGround:
        return 2.0f;

    case CellType::Crater:
        return 3.0f;

    case CellType::Impassable:
        return 0.0f;
    }

    return 1.0f;
}

float getCellTraversalTime(CellType type)
{
    switch (type)
    {
    case CellType::Regolith:
        return 1.0f;

    case CellType::LooseDust:
        return 2.0f;

    case CellType::CompactGround:
        return 0.8f;

    case CellType::RockyGround:
        return 3.0f;

    case CellType::Crater:
        return 4.0f;

    case CellType::Impassable:
        return 0.0f;
    }

    return 1.0f;
}

float getCellBatteryCost(CellType type)
{
    switch (type)
    {
    case CellType::Regolith:
        return 1.0f;

    case CellType::LooseDust:
        return 2.0f;

    case CellType::CompactGround:
        return 0.5f;

    case CellType::RockyGround:
        return 3.0f;

    case CellType::Crater:
        return 4.0f;

    case CellType::Impassable:
        return 0.0f;
    }

    return 1.0f;
}

Cell::Cell(int x, int y, CellType type)
    : x(x), y(y), type(type)
{
}

int Cell::getX() const
{
    return x;
}

int Cell::getY() const
{
    return y;
}

CellType Cell::getType() const
{
    return type;
}

void Cell::setType(CellType newType)
{
    type = newType;
}

bool Cell::isPassable() const
{
    return type != CellType::Impassable;
}