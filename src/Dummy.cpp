#include "Dummy.h"

void Dummy::setupDummy(vec3 v) {
	pos = v;
	dead = 0;
	maxHP = 10;
	currentHP = maxHP;
	colorSwitch = ofRandom(0, 4);
	curColor = colors[colorSwitch];

	//hitbox.set(300, 700, 90);
	//hitbox.setGlobalPosition(pos);
	hitbox.set(150, 700);
	hitbox.setResolutionRadius(20);
	hitbox.setGlobalPosition(pos);
	healthbar.setupHealthbar(pos, 600, 400, maxHP);
	xPos = hitbox.getX();
	yPos = hitbox.getY();
	zPos = hitbox.getZ();


	xRange = ofRandom(1000, 4000);
	xStepSize = ofRandom(5, 15);
	xMin = xPos - xRange;
	xMax = xPos + xRange;

	yRange = ofRandom(1000, 2000);
	yStepSize = ofRandom(2, 4);
	yMin = ofGetWidth() / 2 - 700;
	yMax = yPos - yRange;

	zRange = ofRandom(500, 1000);
	zStepSize = ofRandom(4, 8);
	zMin = zPos - zRange;
	zMax = zPos + zRange;

	rotX = 0;

}

void Dummy::updateDummy() {
	if (hitbox.getX() <= xMin) {
		xDir = 1;
	}
	if (hitbox.getX() >= xMax) {
		xDir = -1;
	}
	if (hitbox.getY() <= yMax) {
		yDir = 1;
	}
	if (hitbox.getY() >= yMin) {
		yDir = -1;
	}
	if (hitbox.getZ() <= zMin) {
		zDir = 1;
	}
	if (hitbox.getZ() >= zMax) {
		zDir = -1;
	}
	if (!dead) {
		if (colorSwitch == 3) {
			hitbox.setGlobalPosition(vec3((hitbox.getX() + xDir * xStepSize), (hitbox.getY() + yDir * yStepSize), (hitbox.getZ() + zDir * zStepSize)));
		}
		else {
			hitbox.setGlobalPosition(vec3((hitbox.getX() + xDir * xStepSize), ofGetHeight() / 2, (hitbox.getZ() + zDir * zStepSize)));
		}
	}
}

void Dummy::updateDummyHP(float dmg) {
	currentHP -= dmg;
	healthbar.updateHealthbar(dmg);
	if (currentHP < 0) {
		currentHP = 0;
		currentHP = maxHP;
		colorSwitch += 1;
		if (colorSwitch == 4) colorSwitch = 0;
		curColor = colors[colorSwitch];
		//xscale += .2;
		//yscale += .2;
		//zscale += .2;
		//dead = 1;
		//rotX = 80;
	}
}

void Dummy::drawDummy() {
	//pos = v;
	//torso
	ofSetColor(255);
	ofSetLineWidth(.1);
	//hitbox.drawWireframe();
	ofSetLineWidth(1);

	ofPushMatrix();
	ofEnableDepthTest();
	ofTranslate(hitbox.getGlobalPosition());

	ofRotateXDeg(rotX);
	//draw HEALTHBAR HERE
	//healthbar.drawHealthbar();

	//ofNoFill();
	//ofDrawBox(300, 700, 150);
	ofTranslate(0, -100, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(200, 280, 80);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(201, 281, 81);
	ofSetLineWidth(1);
		//legs
		ofPushMatrix();
		ofTranslate(-50, 270, 0);
		ofFill();
		ofSetColor(255, 0, 0);
		ofSetColor(curColor);
		ofDrawBox(70, 350, 50);
		ofSetColor(255);
		ofSetColor(0);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawBox(71, 351, 51);
		ofSetLineWidth(1);

		ofTranslate(100, 0, 0);
		ofFill();
		ofSetColor(255, 0, 0);
		ofSetColor(curColor);
		ofDrawBox(70, 350, 50);
		ofSetColor(255);
		ofSetColor(0);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawBox(71, 351, 51);
		ofSetLineWidth(1);
		ofPopMatrix();

		//arms
		ofPushMatrix();
		ofTranslate(-100, 40, 0);
		ofFill();
		ofSetColor(255, 0, 0);
		ofSetColor(curColor);
		ofDrawBox(100, 310, 40);
		ofSetColor(255);
		ofSetColor(0);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawBox(101, 311, 41);
		ofSetLineWidth(1);

		ofTranslate(200, 0, 0);
		ofFill();
		ofSetColor(255, 0, 0);
		ofSetColor(curColor);
		ofDrawBox(100, 310, 40);
		ofSetColor(255);
		ofSetColor(0);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawBox(101, 311, 41);
		ofSetLineWidth(1);
		ofPopMatrix();

		//head
		ofPushMatrix();
		ofTranslate(0, -190, 0);
		ofFill();
		ofSetColor(255, 0, 0);
		ofSetColor(curColor);
		ofDrawBox(80, 120, 50);
		ofSetColor(255);
		ofSetColor(0);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawBox(91, 121, 51);
		ofSetLineWidth(1);
		ofPopMatrix();

		//box
		//ofTranslate(0, 100, 0);
		//ofNoFill();
		//ofSetColor(255);
		//ofDrawBox(300, 700, 150);
	ofDisableDepthTest();
	ofPopMatrix();
	//ofScale(1, 1, 1);

}

Dummy::Dummy()
{
}


Dummy::~Dummy()
{
}
