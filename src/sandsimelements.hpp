#ifndef SANDSIMELEMENTS_HPP
#define SANDSIMELEMENTS_HPP

//== forward declarations ===
namespace ss
{
class CellularMatrix;
}
//=====

namespace ssel
{

enum Elements : int
{
	eEmptyCell,
	eStone,
	eSand,
	eWater,
	eAir,

	size
};

enum StatesOfAggregation : int
{
	gEmpty,
	gMovableSolid,
	gImmovableSolid,
	gLiquid,
	gGas
};

//== Elements =====

class Element
{
public:
	virtual ~Element() = default;

public:
	struct Color
	{
		unsigned char r, g, b, a;
	};

	struct Vector2f
	{
		float x, y;
	};

public:
	float density;
	Elements type;
	StatesOfAggregation stateOfAggretion;

	bool lastUpdateFrame = 0;

	Vector2f velocity;

protected:
	Color color { 5, 5, 5, 255 };

public:
	virtual Color getColor() = 0;
	virtual void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) = 0;

protected:
};

//---

class EmptyCell final : public Element
{
public:
	EmptyCell();

protected:
	Color color { 0, 0, 0, 255 };

public:
	Color getColor() override;
	void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) override;
};

//-----

//-- Solids -----

class Solid : public Element
{
};

class MovableSolid : public Solid
{
public:
	MovableSolid();

public:
	void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) override;
};

class ImmovableSolid : public Solid
{
public:
	ImmovableSolid();

public:
	void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) override;
};

//---

class Sand final : public MovableSolid
{
public:
	Sand();

protected:
	Color color { 255, 215, 85, 255 };

public:
	Color getColor() override;
};

//---

class Stone final : public ImmovableSolid
{
public:
	Stone();

protected:
	Color color { 120, 120, 120, 255 };

public:
	Color getColor() override;
};

//-----

//-- Fluids -----

class Liquid : public Element
{
public:
	Liquid();

protected:
	unsigned int dispersionRate;

public:
	void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) override;
};

//---

class Water : public Liquid
{
public:
	Water();

protected:
	Color color { 100, 155, 240, 255 };

public:
	Color getColor() override;
};

//-----

//-- Gases -----

class Gas : public Element
{
public:
	Gas();

protected:
	unsigned int dispersionRate;

public:
	void step(unsigned int x, unsigned int y, ss::CellularMatrix* matrix) override;
};

//---

class Air : public Gas
{
public:
	Air();

protected:
	Color color { 100, 100, 100, 50 };

public:
	Color getColor() override;
};

//-----

Element*
newElement(unsigned int index);

}

#endif