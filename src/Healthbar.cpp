#include "Healthbar.h"


void Healthbar::setupHealthbar(vec3 p, float height, float width, float hp) {
	pos = p;
	barHeight = height;
	barWidth = width;
	maxHP = hp;
	currentHP = hp;
	widthPerHP = barWidth / maxHP;
}

void Healthbar::updateHealthbar(float dmg) {
	currentHP -= dmg;
	if (currentHP < 0) {
		currentHP = 0;
		currentHP = maxHP;
	}
}

void Healthbar::drawHealthbar() {
	ofPushMatrix();
	ofTranslate(0, -barHeight, 0);
	ofSetColor(255, 0, 0);
	ofFill();
	ofDrawBox(-(maxHP-currentHP)*widthPerHP/2,0,0, currentHP*widthPerHP, 75, 75);
	//ofDrawBox(0, 0, 0, 300, 75, 75);
	ofNoFill();
	ofSetColor(255);
	ofSetLineWidth(2);
	ofDrawBox(0, 0, 0, barWidth, 76, 76);
	ofSetLineWidth(1);
	ofPopMatrix();
}

Healthbar::Healthbar()
{
}


Healthbar::~Healthbar()
{
}
