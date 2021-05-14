// Fill out your copyright notice in the Description page of Project Settings.


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
