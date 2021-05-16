// Fill out your copyright notice in the Description page of Project Settings.


#include "HexCell.h"

HexCell::~HexCell()
{
}

/*
* Operator==
* Overloads the == operator for a HexCell object
*	- Param a: lhs object to be compared
*	- Param b: rhs object to be compared
* Returns true if all components of each HexCell are equal, otherwise returns false
*/
bool operator==(HexCell a, HexCell b)
{
	return a.GetQ() == b.GetQ() && a.GetR() == b.GetR() && a.GetS() == b.GetS();
}

/*
* Operator!=
* Overloads the != operator for a HexCell object
*	- Param a: lhs object to be compared
*	- Param b: rhs object to be compared
* Returns true if all components of each HexCell are not equal, otherwise returns false
*/
bool operator!=(const HexCell& a, const HexCell& b)
{
	return !(a == b);
}

/*
* CubeToAxial
* Converts the coordinates from a given HexCell into Axial coordinates
*	- Param cell: Cell to be converted (stored in Cube coordinates)
* Returns a new HexCell stored in Axial coordinates
*/
HexCell CubeToAxial(HexCell cell)
{
	// See https://www.redblobgames.com/grids/hexagons/ (Coordinate conversion section) for more info on coordinate conversions
	float q = cell.GetQ();
	float r = cell.GetR();

	return HexCell(q, r);
}

/*
* AxialToCube
* Converts the coordinates from a given HexCell into Cube coordinates
*	- Param cell: Cell to be converted (stored in Axial coordinates)
* Returns a new HexCell stored in Cube coordinates
*/
HexCell AxialToCube(HexCell cell)
{
	// See https://www.redblobgames.com/grids/hexagons/ (Coordinate conversion section) for more info on coordinate conversions
	float q = cell.GetQ();
	float r = cell.GetR();
	float s = -q-r;

	return HexCell(q, r, s);
}
