#pragma once
#include "ofMain.h"
using namespace glm;

class Healthbar
{
public:
	Healthbar();
	~Healthbar();

	void setupHealthbar(vec3 p, float height, float width, float hp);
	void updateHealthbar(float dmg);
	void drawHealthbar();

	vec3 pos;
	float maxHP;
	float currentHP;
	float barHeight;
	float barWidth;
	float widthPerHP;
};

