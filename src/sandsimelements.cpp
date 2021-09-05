#include "sandsimelements.hpp"
#include "sandsim.hpp"

using namespace ssel;

//== Elements =====

//-- EmptyCell ---

EmptyCell::EmptyCell()
{
	density = 0.0;
	type = Elements::eEmptyCell;
	stateOfAggretion = StatesOfAggregation::gEmpty;
}

Element::Color EmptyCell::getColor()
{
	return color;
}
void EmptyCell::step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix)
{}

//=======

//== Solids =====

//-- MovableSolid ---

MovableSolid::MovableSolid()
{
	stateOfAggretion = StatesOfAggregation::gMovableSolid;
}

void MovableSolid::step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix)
{
	if (matrix->get(x, y + 1)->density < density && !(matrix->get(x, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x, y + 1);
	else if (matrix->get(x + 1, y + 1)->density < density && !(matrix->get(x, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x + 1, y + 1);
	else if (matrix->get(x - 1, y + 1)->density < density && !(matrix->get(x, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x - 1, y + 1);
}

//-- ImmovableSolid ---

ImmovableSolid::ImmovableSolid()
{
	stateOfAggretion = StatesOfAggregation::gImmovableSolid;
}

void ImmovableSolid::step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix)
{}

//---------------

//-- Sand ---

Sand::Sand()
{
	density = 2.0;
	type = Elements::eSand;
}

Element::Color Sand::getColor()
{
	return color;
}

//---------------

//-- Stone ---

Stone::Stone()
{
	density = 2.0;
	type = Elements::eStone;
}

Element::Color Stone::getColor()
{
	return color;
}

//=======

//== Liquids =====

Liquid::Liquid()
{
	stateOfAggretion = StatesOfAggregation::gLiquid;
}

void Liquid::step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix)
{
	if (matrix->get(x, y + 1)->density < density && !(matrix->get(x, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x, y + 1);
	else if (matrix->get(x + 1, y + 1)->density < density && !(matrix->get(x + 1, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x + 1, y + 1);
	else if (matrix->get(x - 1, y + 1)->density < density && !(matrix->get(x - 1, y + 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x - 1, y + 1);
	else
	{
		unsigned int i;

		for (i = 0; i < dispersionRate; i++)
			if (!(matrix->get(x + (i + 1), y)->density < density && !(matrix->get(x + (i + 1), y)->stateOfAggretion == StatesOfAggregation::gImmovableSolid)))
				break;

		if (i > 0)
			matrix->switchCells(x, y, x + i, y);

		for (i = 0; i < dispersionRate; i++)
			if (!(matrix->get(x - (i + 1), y)->density < density && !(matrix->get(x - (i + 1), y)->stateOfAggretion == StatesOfAggregation::gImmovableSolid)))
				break;

		if (i > 0)
			matrix->switchCells(x, y, x - i, y);
	}
}

//-- Water ---

Water::Water()
{
	density = 1.0;
	type = Elements::eWater;
	dispersionRate = 5;
}

Element::Color Water::getColor()
{
	return color;
}

//-----

//=======

//== Gases =====

Gas::Gas()
{
	stateOfAggretion = StatesOfAggregation::gGas;
}

void Gas::step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix)
{
	if (matrix->get(x, y - 1)->density > density && !(matrix->get(x, y - 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x, y - 1);
	else if (matrix->get(x + 1, y - 1)->density > density && !(matrix->get(x + 1, y - 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x + 1, y - 1);
	else if (matrix->get(x - 1, y - 1)->density > density && !(matrix->get(x - 1, y - 1)->stateOfAggretion == StatesOfAggregation::gImmovableSolid))
		matrix->switchCells(x, y, x - 1, y - 1);
	else
	{
		unsigned int i;

		for (i = 0; i < dispersionRate; i++)
			if (!(matrix->get(x + (i + 1), y)->density > density && !(matrix->get(x + (i + 1), y)->stateOfAggretion == StatesOfAggregation::gImmovableSolid)))
				break;

		if (i > 0)
			matrix->switchCells(x, y, x + i, y);

		for (i = 0; i < dispersionRate; i++)
			if (!(matrix->get(x - (i + 1), y)->density > density && !(matrix->get(x - (i + 1), y)->stateOfAggretion == StatesOfAggregation::gImmovableSolid)))
				break;

		if (i > 0)
			matrix->switchCells(x, y, x - i, y);
	}
}

//-- Air ---

Air::Air()
{
	density = -0.5;
	type = Elements::eWater;
	dispersionRate = 10;
}

Element::Color Air::getColor()
{
	return color;
}

//=======

Element* ssel::newElement(unsigned int index)
{
	switch (index)
	{
		case Elements::eEmptyCell:
			return new EmptyCell;
		case Elements::eStone:
			return new Stone;
		case Elements::eSand:
			return new Sand;
		case Elements::eWater:
			return new Water;
		case Elements::eAir:
			return new Air;
		default:
			return new EmptyCell;
	}
}