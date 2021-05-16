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

// Convert from Cube coordinate to Axial coordinates
HexCell CubeToAxial(HexCell cell);
// Convert from Axial coordinates to Cube coordinates
HexCell AxialToCube(HexCell cell);

// Operator overloads
bool operator==(HexCell a, HexCell b);
bool operator!=(HexCell a, HexCell b);
