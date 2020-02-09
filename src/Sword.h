#pragma once
#include "ofMain.h"
using namespace glm;

class Sword
{
public:

	void setupSword();
	void drawSword(quat q, vec3 handv, int sb);

	Sword();
	~Sword();

	ofCylinderPrimitive handle;
	ofCylinderPrimitive blade;
	float bladeLen=0;
	float maxBladeLen = 560;
	vec3 startpos;
	vec3 startrot;
	vec3 handlepos;
	vec3 handlerot;
	vec3 travelv=vec3(0,0,0);
	bool inHand = 0;
	bool isOn = 0;
	bool inAir = 0;
	bool inGround = 1;

	ofSoundPlayer  saberOn;
	ofSoundPlayer  saberOff;
	bool playSounds = 1;
};

