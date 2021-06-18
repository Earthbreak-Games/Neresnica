 /**
  * @file HexCell.h
  * @brief Class to hold data for a hexagonal cell (stored in Cube coordinates).
  *		   This file also holds helper functions for HexCell operations
  * @dependencies None
  *
  * @author Ethan Heil
  * @credits
  *	https://www.redblobgames.com/grids/hexagons/
  **/

#pragma once

#include "CoreMinimal.h"

class ROBOTGLADIATOR_API HexCell
{
private:
	int mQ, mR, mS;
	int mID;

public:
	// Constructor for Cube coordinates
	HexCell(int q, int r, int s)
		: mQ(q), mR(r), mS(s), mID(0){}
	
	// Constructor for Axial coordinates
	HexCell(int q, int r)
		:mQ(q), mR(r), mS(0), mID(0){}

	~HexCell();

	int GetQ() { return mQ; }
	int GetR() { return mR; }
	int GetS() { return mS; }
	int GetID() {return mID;}

	void SetID(int id) { mID = id; }
};

// List of all 6 hex directions
const TArray<HexCell> HexDirections = 
{
	HexCell(1, 0, -1), // Right
	HexCell(1, -1, 0), // Top-Right
	HexCell(0, -1, 1), // Top-Left
	HexCell(-1, 0, 1), // Left
	HexCell(-1, 1, 0), // Bottom-Left
	HexCell(0, 1, -1), // Bottom-Right
};

/** @brief Converts the coordinates from a given HexCell into Axial coordinates
 *  @param {HexCell} cell - Cell to be converted (stored in Cube coordinates)
 *  @return {HexCell} - Returns a new HexCell stored in Axial coordinates
 */
HexCell CubeToAxial(HexCell cell);
/** @brief Converts the coordinates from a given HexCell into Cube coordinates
 *  @param {HexCell} cell - Cell to be converted (stored in Axial coordinates)
 *  @return {HexCell} - Returns a new HexCell stored in Cube coordinates
 */
HexCell AxialToCube(HexCell cell);
// Adds the components of two HexCells
HexCell AddHex(HexCell a, HexCell b);
// Subtracts the components of two HexCells
HexCell SubtractHex(HexCell a, HexCell b);
// Multiplies the components of a HexCell by k
HexCell MultiplyHex(HexCell a, int k);
// Returns the length of a HexCell vector
int HexLength(HexCell cell);
// Returns the distance between two HexCells
int HexDistance(HexCell a, HexCell b);
// Gets a direction vector of a HexCell based on the given face (0-5)
HexCell GetHexDirection(int face);
// Gets the neighboring cell to the given cell
HexCell GetNeighbor(HexCell cell, int face);

// Operator overloads

/** @brief Operator==
 *  @param {HexCell} a - lhs object to be compared
 *  @param {HexCell} b - rhs object to be compared
 *  @return {bool} - Returns true if all components of each HexCell are equal, otherwise returns false
 */
bool operator==(HexCell a, HexCell b);
/** @brief Operator!=
 *  @param {HexCell} a - lhs object to be compared
 *  @param {HexCell} b - rhs object to be compared
 *  @return {bool} - Returns true if all components of each HexCell are not equal, otherwise returns false
 */
bool operator!=(HexCell a, HexCell b);
