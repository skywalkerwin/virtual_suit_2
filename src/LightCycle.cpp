#include "LightCycle.h"


void LightCycle::setupCycle() {
	wheel1.set(300, 300, 40, 1, 2, true);
	wheel2.set(300, 300, 40, 1, 2, true);
	//wheel1.setCylinderColor(ofColor(24, 202, 230));
	//wheel1.setTopCapColor(ofColor(24, 202, 230));
	//wheel2.setCylinderColor(ofColor(24, 202, 230));
	//wheel2.setTopCapColor(ofColor(24, 202, 230));
	//ofFloatColor(254, 1, 154, 50)
	wheel1.setCylinderColor(ofColor(255));
	wheel1.setTopCapColor(ofColor(255));
	wheel1.setBottomCapColor(ofColor(255));
	//wheel2.setCylinderColor(ofColor(0,0,255));
	//wheel2.setTopCapColor(ofColor(0,0,255));
	//wheel2.setBottomCapColor(ofColor(0,0,255));
	wheel2.setCylinderColor(ofColor(255, 0, 0));
	wheel2.setTopCapColor(ofColor(255, 0, 0));
	wheel2.setBottomCapColor(ofColor(255, 0, 0));
	core.set(250, 1200, 250);
	core.setGlobalPosition(0, 0, 300.5);

	for(int i=0; i<6;i++){
		core.setSideColor(i, ofColor(50));
	}
	wheel1.setParent(core);
	wheel2.setParent(core);
}

void LightCycle::update() {
	vel += acceleration;
	vel *= 0.9;
	acceleration *= 0.99;
	// here we are defining the vector that contains the direction
	// in which the car should move, that is defined by the z-axis of the car and the velocity
	vec3 velVector = core.getYAxis() * vel;
	//core.setGlobalPosition(core.getGlobalPosition()+velVector);
	core.move(velVector);
	//core.rotateDeg(dir, 0, 1, 0);
}

void LightCycle::brake() {
	acceleration -= 0.5;
}

void LightCycle::accelerate(float a) {
	acceleration += 1.25* a;
}

void LightCycle::steer(float d) {
	//dir += (d*.00012);
	//dir = d;
	core.rotateDeg(dir, 0, 0, 1);
	//if (dir > 60) {
	//	dir = 60;
	//}
	//if (dir < -60) {
	//	dir = -60;
	//}
	dir += d/8;
	core.rotateDeg(d/8, 0, 0, 1);
}

void LightCycle::drawTrail() {
	//ofSetColor(24, 202, 230);
	//ofSetColor(255);
	//for (auto point : trailhist) {
	//	if (distance(core.getGlobalPosition(), point) < 500) {
	//		boxhist.push_back(point);
	//	}
	//}
	int tlen = trailhist.size();
	for (int i = 0; i < tlen; i++) {
		if (i< (tlen-10) && distance(core.getGlobalPosition(), trailhist[i]) < 1100) {
			boxhist.push_back(trailhist[i]);
		}
	}
	ofSetColor(255);
	for (auto point : boxhist) {
		ofDrawBox(point, 2000);
	}
	mtrail.draw();
	//mtrail.drawVertices();
	//mtrail.drawWireframe();
}

vec3 LightCycle::drawCycle() {
	drawTrail();
	vec3 p = core.getGlobalPosition();
	core.draw();
	wheel1.boom(600);
	wheel2.boom(-600);
	wheel1.rotateDeg(90, core.getZAxis());
	wheel2.rotateDeg(90, core.getZAxis());

	wheel1.draw();
	wheel2.draw();
	//core.resetTransform();
	wheel1.resetTransform();
	wheel2.resetTransform();
	core.rotateDeg(-dir, 0, 0, 1);
	return p;
}

LightCycle::LightCycle()
{
}


LightCycle::~LightCycle()
{
}
