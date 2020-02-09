#pragma once
#include "ofMain.h"
using namespace glm;

class Rock
{
public:
	
	void setupRock(float sz);
	void updateRock();
	int drawRock();
	Rock();
	~Rock();

	ofSpherePrimitive rock;

	bool show = 0;
	int timeUp = 0;
	float rockSize;
	float rockSpeed;
	int res = 5;
	float rockRad = 200;
	float rockRotate = 0;
	float rockHeight = ofGetHeight();

	int rockState = 0;
	//0 - inactive
	//1 - initial jump
	//2 = hovering
	//3 = launching
};

