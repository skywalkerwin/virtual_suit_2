#pragma once
#include "ofMain.h"
#include "Rock.h"
#include "RockWall.h"

class Earth
{
public:
	Earth();
	~Earth();

	Rock rock;
	RockWall rockwall;

	bool rockUp = 0;

	int moveState = 0;
	//0 - rest
	//1 - rock active
};

