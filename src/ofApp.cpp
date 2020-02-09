#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup() {
	oracle.setupOracle();
	camWidth = 1920;  // try to grab at this size.
	camHeight = 1080;
	//camWidth = 2560;  // try to grab at this size.
	//camHeight = 1440;

	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth, camHeight);

	ofSetVerticalSync(false);
	ofSetFrameRate(120);
	//ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	ofSetColor(0, 255, 0);
	stepClock.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	stepClock2.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	//stepClock.setMode(OF_PRIMITIVE_LINE_STRIP);

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	//ofLog() << serial.listDevices() << endl;
	//ofLog() << deviceList.size() << endl;
	mybody.bodySetup();
	//mybody.legsSetup();
}

//--------------------------------------------------------------
void ofApp::update() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	ofBackground(100, 100, 100);
	if (showCam == 1 && mybody.legs.rock.rockState!=3) {
		vidGrabber.update();
		if (vidGrabber.isFrameNew()) {
			ofPixels & pixels = vidGrabber.getPixels();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackground(0);
	if (showCam == 1 && mybody.legs.rock.rockState!=3) {
		ofSetHexColor(0xffffff);
		vidGrabber.draw(2560, 0, -2560, 1440);
	}
	else {
		ofBackground(0);
	}
	//vidGrabber.draw(0,0);
	ofSetColor(255);
	//ofDrawBitmapString(framecount, 20, 20);

	// ----------------------------------------------------------------- FRAME CLOCK -------------------------------------------------

	ofPushMatrix();
	ofTranslate(19*ofGetWidth()/20, ofGetHeight()/10, 0);
	if (framecount == 0) {
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(0, 0, 0));
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(30, 0, 0));
	}
	ofRotateZRad(-PI / 2);
	stepClock.addColor(ofColor(255));
	stepClock.addVertex(vec3(30*cos(stepClockAngle), 30*sin(stepClockAngle),0));
	stepClock.draw();
	framecount += 1;
	stepClockAngle = framecount * (2 * PI / cycleFrames);
	if (framecount == cycleFrames) {
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(30, 0, 0));
		stepClock.draw();
		framecount = 0;
		stepClock.clearVertices();
	}
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(17 * ofGetWidth() / 20, ofGetHeight() / 10, 0);
	if (framecount2 == 0) {
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(0, 0, 0));
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(30, 0, 0));
	}
	ofRotateZRad(-PI / 2);
	stepClock2.addColor(ofColor(255));
	stepClock2.addVertex(vec3(30 * cos(stepClockAngle2), 30 * sin(stepClockAngle2), 0));
	stepClock2.draw();
	framecount2 += 1;
	stepClockAngle2 = framecount2 * (2 * PI / cycleFrames2);
	if (framecount2 == cycleFrames2) {
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(30, 0, 0));
		stepClock2.draw();
		framecount2 = 0;
		stepClock2.clearVertices();
	}
	ofPopMatrix();

	//------------------------------------------DRAWING BODY HERE------------------------------------------------------------

	mybody.drawHandPlots();
	mybody.legs.plotimu3(ofGetWidth(), ofGetHeight(), exclu);
	mybody.legs.plotAvgs();
	mybody.checkMoves();
	//if (mybody.legs.rock.drawRock()) {
	//	
	//}
	//else {
	//	mybody.legs.rockUp = 0;
	//}
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	vec3 v;
	int move = 50;
	switch (key) {
	case 'c':
		showCam = !showCam;
		break;
	case '1':
		exclu[0] = !exclu[0];
		break;
	case '2':
		exclu[1] = !exclu[1];
		break;
	case '3':
		exclu[2] = !exclu[2];
		break;
	case '4':
		exclu[3] = !exclu[3];
		break;
	case '5':
		exclu[4] = !exclu[4];
		break;
	case 'a':
		v = vec3(-move, 0, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'q':
		v = vec3(0, move, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'd':
		v = vec3(move, 0, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'e':
		v = vec3(0, -move, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 's':
		v = vec3(0, 0, move);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'w':
		v = vec3(0, 0, -move);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'l':
		//v = vec3(0, 0, -move);
		cam.lookAt(cycle.core);
		break;
	case 'z':
		res--;
		break;
	case 'x':
		res++;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::exit() {
	//mybody.~Body();
}
