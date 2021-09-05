#include "sandsim.hpp"
#include "sandsimelements.hpp"

using namespace ss;

#define BASE_ELEMENT ssel::EmptyCell

//== Cellular Matrix =====

CellularMatrix::CellularMatrix(unsigned int _width, unsigned int _height, float _gravity) :
	width(_width),
	height(_height),
	outOfBoundCell(new BASE_ELEMENT),
	gravity(_gravity)
{
	initializeMatrix();
}

void CellularMatrix::update()
{
	updateFrame = !updateFrame;
	std::random_shuffle(std::begin(xUpdateArray), std::end(xUpdateArray));
	for (unsigned int x = 0; x < width; x++)
	{
		for (int y = height - 1; y >= 0; y--)
		{
			unsigned int xPos = xUpdateArray[x];

			ssel::Element* element = matrix[xPos][y];

			if (element->lastUpdateFrame)
				continue;
			element->lastUpdateFrame = true;

			element->step(xPos, y, this);
		}
	}
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			matrix[x][y]->lastUpdateFrame = false;
		}
	}
}

ssel::Element* CellularMatrix::get(unsigned int x, unsigned int y)
{
	if (x >= width || y >= height)
		return outOfBoundCell;
	return matrix[x][y];
}

void CellularMatrix::setExisting(unsigned int x, unsigned int y, ssel::Element* element)
{
	if (x >= width || y >= height)
		return;
	delete matrix[x][y];
	matrix[x][y] = element;
}

void CellularMatrix::setNew(unsigned int x, unsigned int y, ssel::Elements element)
{
	if (x >= width || y >= height)
		return;
	delete matrix[x][y];
	matrix[x][y] = ssel::newElement(element);
}

void CellularMatrix::switchCells(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	if ((x1 >= width || y1 >= height) && (x2 >= width || y2 >= height))
		return;

	if (x2 >= width || y2 >= height)
	{
		setExisting(x1, y1, new BASE_ELEMENT);
		return;
	}
	if (x1 >= width || y1 >= height)
	{
		setExisting(x2, y2, new BASE_ELEMENT);
		return;
	}

	ssel::Element* cell1 = matrix[x1][y1];
	matrix[x1][y1] = matrix[x2][y2];
	matrix[x2][y2] = cell1;
}

void CellularMatrix::initializeMatrix()
{
	for (unsigned int i = 0; i < width; i++)
	{
		xUpdateArray.push_back(i);
		std::vector<ssel::Element*> a;
		matrix.push_back(a);
		for (unsigned int j = 0; j < height; j++)
		{
			matrix[i].push_back(new BASE_ELEMENT);
		}
	}
}

//=======

//== Cellular Matrix Editor =====

CellularMatrixEditor::CellularMatrixEditor(CellularMatrix* mte) :
	matrixToEdit(mte),
	numberOfDrawableElements(ssel::Elements::size - 1)
{}

void CellularMatrixEditor::increaseBrushSize(int m)
{
	brushSize += m;
}
void CellularMatrixEditor::decreaseBrushSize(int m)
{
	brushSize -= m;
	if (brushSize <= 0)
		brushSize = 1;
}

void CellularMatrixEditor::increaseSelectedIndex(int m)
{
	selectedIndex += m;
	selectedIndex = selectedIndex % numberOfDrawableElements;
}
void CellularMatrixEditor::decreaseSelectedIndex(int m)
{
	selectedIndex -= m;
	if (selectedIndex < 0)
		selectedIndex = numberOfDrawableElements - 1;
}

void CellularMatrixEditor::draw(unsigned int x, unsigned int y)
{
	for (int i = -brushSize; i < brushSize; i++)
	{
		for (int j = -brushSize; j < brushSize; j++)
		{
			int posX = x + i;
			int posY = y + j;

			if (posX < 0 || posX > (int)matrixToEdit->width - 1 || posY < 0 || posY > (int)matrixToEdit->height)
				continue;

			ssel::Element* drawingElement = ssel::newElement(selectedIndex + 1);

			if (typeid(matrixToEdit->matrix[posX][posY]) == typeid(*drawingElement))
			{
				delete drawingElement;
				continue;
			}

			matrixToEdit->setExisting(posX, posY, drawingElement);
		}
	}
}
void CellularMatrixEditor::erase(unsigned int x, unsigned int y)
{
	for (int i = -brushSize; i < brushSize; i++)
	{
		for (int j = -brushSize; j < brushSize; j++)
		{
			int posX = x + i;
			int posY = y + j;

			if (posX < 0 || posX > (int)matrixToEdit->width - 1 || posY < 0 || posY > (int)matrixToEdit->height)
				continue;

			if (typeid(matrixToEdit->matrix[posX][posY]) == typeid(BASE_ELEMENT))
				continue;

			matrixToEdit->setExisting(posX, posY, new BASE_ELEMENT);
		}
	}
}

//=======
