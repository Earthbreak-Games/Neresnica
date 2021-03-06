/**
 * HexCell.cpp
 * Purpose: Class to define functionality for a hexagonal cell (stored in Cube coordinates).
 * Refer to https://www.redblobgames.com/grids/hexagons/implementation.html for more info
 * Dependencies: None
 * Primary Author: Ethan Heil
 */

#include "HexCell.h"

HexCell::~HexCell()
{
}

bool operator==(HexCell a, HexCell b)
{
	return a.GetQ() == b.GetQ() && a.GetR() == b.GetR() && a.GetS() == b.GetS();
}

bool operator!=(const HexCell& a, const HexCell& b)
{
	return !(a == b);
}

HexCell CubeToAxial(HexCell cell)
{
	// See https://www.redblobgames.com/grids/hexagons/ (Coordinate conversion section) for more info on coordinate conversions
	float q = cell.GetQ();
	float r = cell.GetR();

	return HexCell(q, r);
}

HexCell AxialToCube(HexCell cell)
{
	// See https://www.redblobgames.com/grids/hexagons/ (Coordinate conversion section) for more info on coordinate conversions
	float q = cell.GetQ();
	float r = cell.GetR();
	float s = -q-r;

	return HexCell(q, r, s);
}

/*
* AddHex
* Adds the components of two HexCells
*	- Param a: First cell to add
*	- Param b: Second cell to add
* Returns a new HexCell with the added components of a and b
*/
HexCell AddHex(HexCell a, HexCell b)
{
	return HexCell(a.GetQ() + b.GetQ(), a.GetR() + b.GetR(), a.GetS() + b.GetS());
}

/*
* SubtractHex
* Subtracts the components of two HexCells
*	- Param a: First cell to subtract
*	- Param b: Second cell to subtract
* Returns a new HexCell with the subtracted components of a and b
*/
HexCell SubtractHex(HexCell a, HexCell b)
{
	return HexCell(a.GetQ() - b.GetQ(), a.GetR() - b.GetR(), a.GetS() - b.GetS());
}

/*
* MultiplyHex
* Adds the components of two HexCells
*	- Param a: The cell to multiply
*	- Param k: Constant to multiply a's components by 
* Returns a new HexCell with it's components scaled by k
*/
HexCell MultiplyHex(HexCell a, int k)
{
	return HexCell(a.GetQ() * k, a.GetR() * k, a.GetS() * k);
}

/*
* HexLength
* Adds the components of two HexCells
*	- Param cell: The HexCell that we want to find the length of
* Returns the length of the cell
*/
int HexLength(HexCell cell)
{
	return int((FMath::Abs(cell.GetQ()) + FMath::Abs(cell.GetR()) + FMath::Abs(cell.GetS())) / 2);
}

/*
* HexDistance
* Finds the distance between two HexCells
*	- Param a: The cell to start at
*	- Param b: The target cell
* Returns the distance between a and b
*/
int HexDistance(HexCell a, HexCell b)
{
	return HexLength(SubtractHex(a, b));
}

/*
* GetHexDirection
* Gets a direction vector based on a given face (0-5)
*	- Param face: The face (direction) that you want to go in
* Returns the direction vector of a given face
*/
HexCell GetHexDirection(int face)
{
	// Checks if face is valid
	if(face >= 0 && face < 6)
		return HexDirections[face]; // Returns the direction vector of the given face
	
	return HexCell(0.0f , 0.0f, 0.0f);
}

/*
* GetNeighbor
* Gets a neighboring cell in the direction of face 
*	- Param face: The face (direction) that you want to go in
* Returns neighboring cell in the direction of face
*/
HexCell GetNeighbor(HexCell cell, int face)
{
	return AddHex(cell, GetHexDirection(face));
}
