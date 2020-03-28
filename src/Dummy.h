#pragma once
#include "ofMain.h"
#include "Healthbar.h"

using namespace glm;

class Dummy
{
public:
	void setupDummy(vec3 v);
	void updateDummy();
	void updateDummyHP(float dmg);
	void drawDummy();

	Dummy();
	~Dummy();

	vec3 pos;
	float xPos = 0;
	float yPos = 0;
	float zPos = 0;
	
	//pathing
	float xRange = 2000;
	float xMin = 0;
	float xMax = 0;
	float xDir = 1;
	float xStepSize = 20;

	float yRange = 2000;
	float yMin = 0;
	float yMax = 0;
	float yDir = 1;
	float yStepSize = 20;

	float zRange = 400;
	float zMin = 0;
	float zMax = 0;
	float zDir = 1;
	float zStepSize = 4;
	//ofBoxPrimitive hitbox;
	ofCylinderPrimitive hitbox;
	
	//health
	Healthbar healthbar;
	float maxHP=100;
	float currentHP;
	float rotX=0;
	bool dead = 0;

	ofColor red = ofColor(255, 0, 0);
	ofColor green = ofColor(0, 255, 0);
	ofColor blue = ofColor(0, 50, 255);
	ofColor white = ofColor(255);
	//ofColor black = ofColor(0);

	ofColor	colors[4] = { red, green, blue, white};
	int colorSwitch = 0;
	ofColor curColor = colors[colorSwitch];

	float xscale = 1;
	float yscale = 1;
	float zscale = 1;



};

