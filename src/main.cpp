#include "PCH.hpp"
#include "sandsim.hpp"
#include "sandsimelements.hpp"

#include "C:\emsdk\upstream\emscripten\system\include\emscripten.h"


ss::CellularMatrix cellularMatrix(384, 216, 1.0);
ss::CellularMatrixEditor matrixEditor(&cellularMatrix);

extern "C"
{

EMSCRIPTEN_KEEPALIVE
void update()
{
	cellularMatrix.update();
}


EMSCRIPTEN_KEEPALIVE
unsigned char getCellR(unsigned int x, unsigned int y)
{
	return cellularMatrix.get(x, y)->getColor().r;
}
EMSCRIPTEN_KEEPALIVE
unsigned char getCellG(unsigned int x, unsigned int y)
{
	return cellularMatrix.get(x, y)->getColor().g;
}
EMSCRIPTEN_KEEPALIVE
unsigned char getCellB(unsigned int x, unsigned int y)
{
	return cellularMatrix.get(x, y)->getColor().b;
}
EMSCRIPTEN_KEEPALIVE
unsigned char getCellA(unsigned int x, unsigned int y)
{
	return cellularMatrix.get(x, y)->getColor().a;
}


EMSCRIPTEN_KEEPALIVE
void matrixDraw(unsigned int x, unsigned int y)
{
	matrixEditor.draw(x, y);
}
EMSCRIPTEN_KEEPALIVE
void matrixErase(unsigned int x, unsigned int y)
{
	matrixEditor.erase(x, y);
}
EMSCRIPTEN_KEEPALIVE
void increaseBrushSize(int m = 1)
{
	matrixEditor.increaseBrushSize(m);
}
EMSCRIPTEN_KEEPALIVE
void decreaseBrushSize(int m = 1)
{
	matrixEditor.decreaseBrushSize(m);
}
EMSCRIPTEN_KEEPALIVE
void increaseSelectedIndex(int m = 1)
{
	matrixEditor.increaseSelectedIndex(m);
}
EMSCRIPTEN_KEEPALIVE
void decreaseSelectedIndex(int m = 1)
{
	matrixEditor.decreaseSelectedIndex(m);
}

EMSCRIPTEN_KEEPALIVE
int getBrushSize()
{
	return matrixEditor.brushSize * 2;
}
EMSCRIPTEN_KEEPALIVE
int getSelectedIndex()
{
	return matrixEditor.selectedIndex;
}


}