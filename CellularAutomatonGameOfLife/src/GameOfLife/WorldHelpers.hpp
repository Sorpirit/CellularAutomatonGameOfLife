#pragma once

#include <tuple>

#include "CellDefaults.hpp"
#include "../Core/World.hpp"

using namespace std;

static constexpr tuple<int, int> NeighborDirections[] = {
		make_tuple(-1, 1),
		make_tuple(0, 1),
		make_tuple(1, 1),
		make_tuple(1, 0),
		make_tuple(1, -1),
		make_tuple(0, -1),
		make_tuple(-1, -1),
		make_tuple(-1, 0),
};

inline void GenerateRandomMap(Core::World<Core::Cell>& world)
{
    for (int i = 0; i < world.GetWidth(); ++i)
    {
        for (int j = 0; j < world.GetHeight(); ++j)
        {
            world.SetCell(i, j, rand() % 2 == 0 ? FullCell : EmptyCell);
        }
    }
}

inline void ClearMap(Core::World<Core::Cell>& world)
{
    for (int i = 0; i < world.GetWidth(); ++i)
    {
        for (int j = 0; j < world.GetHeight(); ++j)
        {
            world.SetCell(i, j, EmptyCell);
        }
    }
}