#include "Body.h"


void Body::bodySetup() {
	//torso.set(50);
	legs.legsSetup(legComs);
	larm.armSetup(leftcom, torso, -1);
	rarm.armSetup(rightcom, torso, 1);
	//*lcycle = c;
	//torso.setParent(lcycle->core);
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(v + offset);

}

void Body::checkMoves() {
	if (rarm.switches[0] == 1) {
		legs.rockCheck1();
	}
	if (!legs.rock.drawRock()) {
		legs.rockUp = 0;
	}
}

void Body::legsSetup() {
	legs.legsSetup(legComs);
}

void Body::armsSetup() {
	//torso.set(50);
	larm.armSetup(leftcom, torso, -1);
	rarm.armSetup(rightcom, torso, 1);
	//*lcycle = c;
	//torso.setParent(lcycle->core);
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(v + offset);

}

void Body::bodyDraw() {
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(lcycle->getGlobalPosition()+offset);
	torsopos = torso.getGlobalPosition();
	//neckpos = torso.getGlobalPosition();
	larm.armDraw(torsopos);
	rarm.armDraw(torsopos);
	ofSetColor(255,0,0);
	//ofSetLineWidth(5);
	ofDrawLine(larm.handpos, rarm.handpos);
	//ofSetColor(255);
	torso.draw();
	torso.resetTransform();

}

void Body::bodyUpdate(vec3 v) {
	//larm.armUpdate();
	//rarm.armUpdate();
	curLine++;
	if (curLine == maxLines) {
		curLine = 0;
	}
	//auto velVector = torso.getYAxis() * vel;
	//torso.move(velVector);
	torso.setGlobalPosition(v);
}
void Body::torsoDraw() {
	vec3 origin(0, 0, 0);
	torso.setPosition(origin);
}

void Body::testimus() {
	//ofPushMatrix();
	//ofTranslate(1 * ofGetWidth() / 4, 0, 0);
	//larm.testimus();
	//ofPopMatrix();
	//ofPushMatrix();
	//ofTranslate(3 * ofGetWidth() / 4, 0, 0);
	//rarm.testimus();
	//ofPopMatrix();
	ofSetColor(255);
	ofDrawBitmapString(rarm.pitch[3], ofGetWidth() - 100, 20);
	ofDrawBitmapString(rarm.pitch[4], ofGetWidth() - 100, 40);
	ofDrawBitmapString(rarm.pitch[5], ofGetWidth() - 100, 60);
	ofDrawBitmapString(rarm.pitch[6], ofGetWidth() - 100, 80);
	ofDrawBitmapString(rarm.pitch[7], ofGetWidth() - 100, 100);

	ofDrawBitmapString(rarm.roll[3], ofGetWidth() - 100, 140);
	ofDrawBitmapString(rarm.roll[5], ofGetWidth() - 100, 160);
	ofDrawBitmapString(rarm.roll[4], ofGetWidth() - 100, 180);
	ofDrawBitmapString(rarm.roll[6], ofGetWidth() - 100, 200);
	ofDrawBitmapString(rarm.roll[7], ofGetWidth() - 100, 220);

}

void Body::drawHandPlots() {
	testimus();
	larm.plotimu(vec3(0, 0, 0), ofGetWidth(), ofGetHeight(), 0);
	rarm.plotimu(vec3(0, 0, 0), ofGetWidth(), ofGetHeight(), 0);
}



Body::Body()
{
}


Body::~Body()
{
	//larm.~Arm();
	//rarm.~Arm();
	//delete &larm;
	//delete &rarm;
}
