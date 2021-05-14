// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * HexCell.h
 * Purpose: Class to hold data for a hexagonal cell (stored in cube coordinates).
 * Refer to https://www.redblobgames.com/grids/hexagons/implementation.html for more info
 * This file also holds helper functions for HexCell operations
 * Dependencies: None
 * Primary Author: Ethan Heil
 */
class ROBOTGLADIATOR_API HexCell
{
private:
	int mQ, mR, mS;

public:
	HexCell(int q, int r, int s)
		: mQ(q), mR(r), mS(s)
	{
		
	}
	~HexCell();

	int GetQ() { return mQ; }
	int GetR() { return mR; }
	int GetS() { return mS; }
};

bool operator==(HexCell a, HexCell b);
bool operator!=(HexCell a, HexCell b);
