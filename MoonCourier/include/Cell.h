#pragma once

enum class CellType
{
    Regolith,
    LooseDust,
    CompactGround,
    RockyGround,
    Crater,
    Impassable
};

float getCellTraversalCost(CellType type);
float getCellTraversalTime(CellType type);
float getCellBatteryCost(CellType type);

class Cell
{
public:
    Cell(int x, int y, CellType type = CellType::Regolith);

    int getX() const;
    int getY() const;

    CellType getType() const;
    void setType(CellType new_type);

    bool isPassable() const;

private:
    int x;
    int y;
    CellType type;
};