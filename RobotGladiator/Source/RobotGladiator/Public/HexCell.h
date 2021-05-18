/**
 * HexCell.h
 * Purpose: Class to hold data for a hexagonal cell (stored in Cube coordinates).
 * Refer to https://www.redblobgames.com/grids/hexagons/implementation.html for more info
 * This file also holds helper functions for HexCell operations
 * Dependencies: None
 * Primary Author: Ethan Heil
 */

#pragma once

#include "CoreMinimal.h"

class ROBOTGLADIATOR_API HexCell
{
private:
	int mQ, mR, mS;

public:
	// Constructor for Cube coordinates
	HexCell(int q, int r, int s)
		: mQ(q), mR(r), mS(s){}
	
	// Constructor for Axial coordinates
	HexCell(int q, int r)
		:mQ(q), mR(r), mS(0){}

	~HexCell();

	int GetQ() { return mQ; }
	int GetR() { return mR; }
	int GetS() { return mS; }
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

// Convert from Cube coordinate to Axial coordinates
HexCell CubeToAxial(HexCell cell);
// Convert from Axial coordinates to Cube coordinates
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
bool operator==(HexCell a, HexCell b);
bool operator!=(HexCell a, HexCell b);
