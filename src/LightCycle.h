#pragma once
#include "ofMain.h"
using namespace glm;

class LightCycle
{
public:
	void setupCycle();
	void update();
	void brake();
	void accelerate(float a);
	void steer(float d);
	void drawTrail();
	vec3 drawCycle();
	LightCycle();
	~LightCycle();

	ofCylinderPrimitive wheel1;
	ofCylinderPrimitive wheel2;
	ofBoxPrimitive core;
	vector<vec3> trailhist;
	vector<vec3> boxhist;
	ofMesh mtrail;
	float acceleration;
	float vel;
	float dir = 0;
};