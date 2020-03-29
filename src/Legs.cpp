#include "Legs.h"

void Legs::legsSetup(string c) {
	coms = c;
	port.listDevices();
	setThreadName("Legs");
	//vector <ofSerialDeviceInfo> deviceList = port.getDeviceList();
	//ofLog() << deviceList << endl;
	//ofLog() << port.setup(c, 12000000) << endl;
	//ofLog() << port.isInitialized() << endl;
	port.setup(c, 20000000);
	//legData.open(ofToDataPath("legData.txt"), ofFile::ReadWrite);
	//legData.create();
	startThread();
}

void Legs::start() {
	startThread();
}

void Legs::stop() {
	//while (updated == 0) {
	//}
	std::unique_lock<std::mutex> lck(mutex);
	stopThread();
	condition.notify_all();
	port.close();

}

void Legs::threadedFunction() {
	while (isThreadRunning()) {
		//std::unique_lock<std::mutex> lock(mutex);
		legsUpdate();
		//condition.wait(lock);
	}
}

void Legs::legsUpdate() {
	if (firstContact == false) {
		char myByte = port.readByte();
		if (myByte == OF_SERIAL_NO_DATA);
			//ofLog() << "no data was read";
		else if (myByte == OF_SERIAL_ERROR);
			//ofLog() << "an error occurred";
		else {
			port.flush();
			firstContact = true;
			port.writeByte('A');
		}
	}
	else {
		if (port.available() > 133) {
			int res = port.readBytes(inBuffer, 134);
			port.writeByte('A');
			//2x magno
			for (int i = 0; i < 2; i++) {
				// ACCEL
				magno[i][0] = (int16_t)((inBuffer[0 + (i * 18)] << 8 | inBuffer[1 + (i * 18)] & 0xff));
				magno[i][1] = (int16_t)((inBuffer[2 + (i * 18)] << 8 | inBuffer[3 + (i * 18)] & 0xff));
				magno[i][2] = (int16_t)((inBuffer[4 + (i * 18)] << 8 | inBuffer[5 + (i * 18)] & 0xff));
				// GYRO
				magno[i][3] = (int16_t)((inBuffer[6 + (i * 18)] << 8 | inBuffer[7 + (i * 18)] & 0xff));
				magno[i][4] = (int16_t)((inBuffer[8 + (i * 18)] << 8 | inBuffer[9 + (i * 18)] & 0xff));
				magno[i][5] = (int16_t)((inBuffer[10 + (i * 18)] << 8 | inBuffer[11 + (i * 18)] & 0xff));
				// MAGNO
				magno[i][6] = (int16_t)((inBuffer[12 + (i * 18)] << 8 | inBuffer[13 + (i * 18)] & 0xff));
				magno[i][7] = (int16_t)((inBuffer[14 + (i * 18)] << 8 | inBuffer[15 + (i * 18)] & 0xff));
				magno[i][8] = (int16_t)((inBuffer[16 + (i * 18)] << 8 | inBuffer[17 + (i * 18)] & 0xff));
				for (int j = 0; j < 9; j++) {
					if (j < 3) magno[i][j] = magno[i][j] * ascale;
					else if (j > 2 && j < 6) magno[i][j] = magno[i][j] * gscale;
					if (j>5)  {
						//magno[i][j-6] = magno[i][j-6];
						if (magno[i][j] > magmax[i][j - 6]) { magmax[i][j - 6] = magno[i][j]; }
						if (magno[i][j] < magmin[i][j - 6]) { magmin[i][j - 6] = magno[i][j]; }
						magbias[i][j - 6] = (magmax[i][j - 6] + magmin[i][j - 6]) / 2;
						magscale[i][j - 6] = (magmax[i][j - 6] - magmin[i][j - 6]) / 2;
						avgrad[i] = (magscale[i][0] + magscale[i][1] + magscale[i][2]) / 3;
						if (magscale[i][j - 6] != 0) { scalebias[i][j - 6] = avgrad[i] / magscale[i][j - 6]; }
						magno[i][j] -= magbias[i][j - 6];
						magno[i][j] *= scalebias[i][j - 6];
					}
					//magno[i][j]=(float)(magno[i][j]*(((magadj[(-6+j)]-128) *.5)/128)+1);
					// APPEND TO HISTORIES
					nmag[i][j][off] = magno[i][j];
					mhist[i][j][hcount] = magno[i][j];
					if (j < 6) {
						if (i == 0) {
							lhist[0][j][hcount] = magno[i][j];
							leftCur[0][j] = magno[i][j];
							legCur[0][0][j] = magno[i][j];
						}
						if (i == 1) {
							rhist[0][j][hcount] = magno[i][j];
							rightCur[0][j] = magno[i][j];
							legCur[1][0][j] = magno[i][j];
						}
					}
				}
			}
			// 8x imu6
			for (int i = 0; i < 8; i++) {
				// ACCEL
				imu[i][0] = (int16_t)((inBuffer[36 + (i * 12)] << 8) | (inBuffer[37 + (i * 12)] & 0xff));
				imu[i][1] = (int16_t)((inBuffer[38 + (i * 12)] << 8) | (inBuffer[39 + (i * 12)] & 0xff));
				imu[i][2] = (int16_t)((inBuffer[40 + (i * 12)] << 8) | (inBuffer[41 + (i * 12)] & 0xff));
				// GYRO
				imu[i][3] = (int16_t)((inBuffer[42 + (i * 12)] << 8) | (inBuffer[43 + (i * 12)] & 0xff));
				imu[i][4] = (int16_t)((inBuffer[44 + (i * 12)] << 8) | (inBuffer[45 + (i * 12)] & 0xff));
				imu[i][5] = (int16_t)((inBuffer[46 + (i * 12)] << 8) | (inBuffer[47 + (i * 12)] & 0xff));

				for (int j = 0; j < 6; j++) {
					if (j < 3) { imu[i][j] = imu[i][j] * ascale; }
					else { imu[i][j] = imu[i][j] * gscale; }
					// APPEND TO HISTORIES
					nimu[i][j][off] = imu[i][j];
					ihist[i][j][hcount] = imu[i][j];
					//put left and right sensors in respective arrays for easier access
					if (i == 0 || i == 1) {
						//first left 2
						lhist[i + 1][j][hcount] = imu[i][j];
						leftCur[i + 1][j] = imu[i][j];
						legCur[0][i + 1][j] = magno[i][j];
					}
					if (i == 2 || i == 3) {
						//first right 2
						rhist[i - 1][j][hcount] = imu[i][j];
						rightCur[i - 1][j] = imu[i][j];
						legCur[1][i - 1][j] = magno[i][j];
					}
					if (i == 4 || i == 5) {
						//second left 2
						lhist[i - 1][j][hcount] = imu[i][j];
						leftCur[i - 1][j] = imu[i][j];
						legCur[0][i - 1][j] = magno[i][j];
					}
					if (i == 6 || i == 7) {
						//second right 2
						rhist[i - 3][j][hcount] = imu[i][j];
						rightCur[i - 3][j] = imu[i][j];
						legCur[1][i - 3][j] = magno[i][j];
					}
				}
			}
			ttime = ((inBuffer[132] << 8) | (inBuffer[133] & 0xff));
			calcAvgs();
			calcSavgs();
			calcXYZavgs();
			off = off + 1;
			if (off == nframes) { off = 0; }
			//deltat = ttime / 1000000.0f;
			hcount++;
			if (hcount == histlength) { hcount = 0; }
			//sensorfusion();
		}
	}
	//ofSetColor(255);
	//ofDrawBitmapString(lSavg[0][0], 20, 20);
	//ofDrawBitmapString(lSavg[0][1], 40, 20);

	//ofDrawBitmapString(lSavg[1][0], 20, 40);
	//ofDrawBitmapString(lSavg[1][1], 40, 40);

	//ofDrawBitmapString(lSavg[2][0], 20, 60);
	//ofDrawBitmapString(lSavg[2][1], 40, 60);

	//ofDrawBitmapString(lSavg[3][0], 20, 80);
	//ofDrawBitmapString(lSavg[3][1], 40, 80);

	//ofDrawBitmapString(lSavg[4][0], 20, 100);
	//ofDrawBitmapString(lSavg[4][1], 40, 100);
	
	
	//ofLog() << "ttime : "<<ttime << endl;
	//ofLog() << "test : " << magno[0][0] << endl;
	//ofLog() << coms << "deltat: " << deltat << endl;
	//for (int i = 0; i < 2; i++) {
	//	for (int j = 0; j < 9; j++) {
	//		legData << magno[i][j] << ",";
	//	}
	//}
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 6; j++) {
	//		legData << imu[i][j] << ",";
	//	}
	//}
	//legData << endl;
}


// ######################################################################################################################################################################################

void Legs::calcAvgs() {
	//average values of each axis on sensor over a window of size histlength
	//REMOVED ABSOLUTE VALUES FOR CALCULATIONS
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			lavg[i][j] = 0;
			ravg[i][j] = 0;
			avgs[0][i][j] = 0;
			avgs[1][i][j] = 0;
			for (int k = 0; k < histlength; k++) {
				lavg[i][j] += (lhist[i][j][k]);
				ravg[i][j] += (rhist[i][j][k]);
				avgs[0][i][j] += (lhist[i][j][k]);
				avgs[1][i][j] += (rhist[i][j][k]);
			}
			lavg[i][j] /= histlength;
			ravg[i][j] /= histlength;
			avgs[0][i][j] /= histlength;
			avgs[1][i][j] /= histlength;
		}
	}
}

void Legs::calcSavgs() {
	//average values of all accel and gyro measurements for each sensor (x+y+z)/3
	for (int i = 0; i < 5; i++) {
		lSavg[i][0] = 0;
		rSavg[i][0] = 0;
		lSavg[i][1] = 0;
		rSavg[i][1] = 0;
		savgs[0][i][0] = 0;
		savgs[1][i][0] = 0;
		savgs[0][i][1] = 0;
		savgs[1][i][1] = 0;
		for (int j = 0; j < 3; j++) {
			lSavg[i][0] += lavg[i][j];
			rSavg[i][0] += ravg[i][j];
			savgs[0][i][0] += lavg[i][j];
			savgs[1][i][0] += ravg[i][j];
		}
		for (int j = 3; j < 6; j++) {
			lSavg[i][1] += lavg[i][j];
			rSavg[i][1] += ravg[i][j];
			savgs[0][i][1] += lavg[i][j];
			savgs[1][i][1] += ravg[i][j];
		}
		lSavg[i][0] /= 3;
		rSavg[i][0] /= 3;
		lSavg[i][1] /= 3;
		rSavg[i][1] /= 3;
		savgs[0][i][0] /= 3;
		savgs[1][i][0] /= 3;
		savgs[0][i][1] /= 3;
		savgs[1][i][1] /= 3;
	}
}

void Legs::calcXYZavgs() {
	//average values of all (accel and gyro) average measurements for all sensors on each leg 
	for (int j = 0; j < 6; j++) {
		lxyzavg[j] = 0;
		rxyzavg[j] = 0;
		xyzavgs[0][j] = 0;
		xyzavgs[1][j] = 0;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			lxyzavg[i] += lavg[j][i];
			rxyzavg[i] += ravg[j][i];
			xyzavgs[0][i] += lavg[j][i];
			xyzavgs[1][i] += ravg[j][i];
		}
		lxyzavg[i] /= 5;
		rxyzavg[i] /= 5;
		xyzavgs[0][i] /= 5;
		xyzavgs[1][i] /= 5;
	}
}

int Legs::rockCheck1() {
	// right now i am using this check command to simply check for the first launching of the rock, but Ill eventually need to check
	// for other types of actions as well, not just from the legs, but from the whole body as well and im not sure how to do this yet.

	//this function returns a 0 (LEFT) or a 1 (RIGHT), telling the body which leg to grab the sensor data from
	if (abs(lxyzavg[3]) > 100) {
		return 0;
	}
	if (abs(rxyzavg[3]) > 100) {
		return 1;
	}
	return 1;
}

int Legs::rockCheck2() {
	//this function returns a 0 (LEFT) or a 1 (RIGHT), telling the body which leg to grab the sensor data from
	//this function is checking for the second sequence of actions - to propel the rock using the sensor on the foot
	
	//INFUSE MORE SENSORS PLEASE

	if (lavg[0][2] > 3) {
		return 0;
	}
	if (ravg[0][2] > 3) {
		return 1;
	}
	return -1;
}

void Legs::plotimu(float x, float y, int c, int h) {
	float xoff = (ofGetWidth() / 2) / histlength;
	float yoff = y / 60000;
	ofNoFill();
	ofPushMatrix();
	ofTranslate((h*ofGetWidth()/2), y / 2, 0);
	ofSetColor(255, 255, 255);
	ofLine(vec3(x, 0, 0), vec3(x+ofGetWidth()/2, 0, 0));
	for (int j = 0; j < 8; j++) {
		for (int k = c; k < c+3; k++) {
			if (k == 0 || k == 3) {
				ofSetColor(255, 0, 0);
			}
			else if (k == 1 || k == 4) {
				ofSetColor(0, 255, 0);
			}
			else {
				ofSetColor(0, 100, 255);
			}
			ofPushMatrix();
			ofBeginShape();
			for (int i = hcount; i < histlength; i++) {
				ofVertex((ofGetWidth() / 2) - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
			}
			ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
			for (int i = 0; i < hcount; i++) {
				ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
			}
			ofEndShape(false);
			ofPopMatrix();
		}
	}
	ofPopMatrix();
}

void Legs::plotimu2(float x, float y) {
	float xoff = (x / 2) / histlength;
	float yoff = y/4/16;
	ofNoFill();
	ofPushMatrix();
	//LEFT LEGT
	ofTranslate(0, y / 4, 0);
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 8; j++) {
		if (j == 0 || j == 1 || j == 4 || j == 5) {
			for (int k = 0; k < 3; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				if (j == 0) {
					ofPushMatrix();
					ofBeginShape();
					for (int i = hcount; i < histlength; i++) {
						ofVertex((x / 2) - ((i - hcount) * xoff), mhist[j][k][i] * yoff);
					}
					ofVertex((hcount)* xoff, mhist[j][k][histlength - 1] * yoff);
					for (int i = 0; i < hcount; i++) {
						ofVertex((hcount - i)*xoff, mhist[j][k][i] * yoff);
					}
					ofEndShape(false);
					ofPopMatrix();
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofTranslate(0, y / 2, 0);
	ofSetColor(255, 255, 255);
	yoff = y / 4 / 2000;
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 8; j++) {
		if (j == 0 || j == 1 || j == 4 || j == 5) {
			for (int k = 3; k < 6; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				if (j == 0) {
					ofPushMatrix();
					ofBeginShape();
					for (int i = hcount; i < histlength; i++) {
						ofVertex((x / 2) - ((i - hcount) * xoff), mhist[j][k][i] * yoff);
					}
					ofVertex((hcount)* xoff, mhist[j][k][histlength - 1] * yoff);
					for (int i = 0; i < hcount; i++) {
						ofVertex((hcount - i)*xoff, mhist[j][k][i] * yoff);
					}
					ofEndShape(false);
					ofPopMatrix();
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofPopMatrix();
	//RIGHT LEG
	ofPushMatrix();
	yoff = y / 4 / 16;
	ofTranslate(x/2, y / 4, 0);
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 2; j < 8; j++) {
		if (j == 2 || j == 3 || j == 6 || j == 7) {
			for (int k = 0; k < 3; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				if (j == 2) {
					ofPushMatrix();
					ofBeginShape();
					for (int i = hcount; i < histlength; i++) {
						ofVertex((x / 2) - ((i - hcount) * xoff), mhist[1][k][i] * yoff);
					}
					ofVertex((hcount)* xoff, mhist[1][k][histlength - 1] * yoff);
					for (int i = 0; i < hcount; i++) {
						ofVertex((hcount - i)*xoff, mhist[1][k][i] * yoff);
					}
					ofEndShape(false);
					ofPopMatrix();
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofTranslate(0, y / 2, 0);
	yoff = y / 4 / 2000;
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 2; j < 8; j++) {
		if (j == 2 || j == 3 || j == 6 || j == 7) {
			for (int k = 3; k < 6; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				if (j == 2) {
					ofPushMatrix();
					ofBeginShape();
					for (int i = hcount; i < histlength; i++) {
						ofVertex((x / 2) - ((i - hcount) * xoff), mhist[1][k][i] * yoff);
					}
					ofVertex((hcount)* xoff, mhist[1][k][histlength - 1] * yoff);
					for (int i = 0; i < hcount; i++) {
						ofVertex((hcount - i)*xoff, mhist[1][k][i] * yoff);
					}
					ofEndShape(false);
					ofPopMatrix();
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofPopMatrix();
}

void Legs::plotimu3(float x, float y, bool exclu[5]) {
	ofPushMatrix();
	float xoff = (x / 2) / histlength;
	float yoff = y / 4 / 16;
	ofNoFill();
	ofPushMatrix();
	//LEFT LEGT
	ofTranslate(0, y / 4, 0);
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 5; j++) {
		if (exclu[j]) {
			for (int k = 0; k < 3; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), lhist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, lhist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, lhist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofTranslate(0, y / 2, 0);
	ofSetColor(255, 255, 255);
	yoff = y / 4 / 2000;
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 5; j++) {
		if (exclu[j]) {
			for (int k = 3; k < 6; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), lhist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, lhist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, lhist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofPopMatrix();
	//RIGHT LEG
	ofPushMatrix();
	yoff = y / 4 / 16;
	ofTranslate(x / 2, y / 4, 0);
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 5; j++) {
		if (exclu[j]) {
			for (int k = 0; k < 3; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), rhist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, rhist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, rhist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofTranslate(0, y / 2, 0);
	yoff = y / 4 / 2000;
	ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x / 2, 0, 0));
	for (int j = 0; j < 5; j++) {
		if (exclu[j]) {
			for (int k = 3; k < 6; k++) {
				if (k == 0 || k == 3) {
					ofSetColor(255, 0, 0);
				}
				else if (k == 1 || k == 4) {
					ofSetColor(0, 255, 0);
				}
				else {
					ofSetColor(0, 100, 255);
				}
				ofPushMatrix();
				ofBeginShape();
				for (int i = hcount; i < histlength; i++) {
					ofVertex((x / 2) - ((i - hcount) * xoff), rhist[j][k][i] * yoff);
				}
				ofVertex((hcount)* xoff, rhist[j][k][histlength - 1] * yoff);
				for (int i = 0; i < hcount; i++) {
					ofVertex((hcount - i)*xoff, rhist[j][k][i] * yoff);
				}
				ofEndShape(false);
				ofPopMatrix();
			}
		}
	}
	ofPopMatrix();
	ofPopMatrix();
}

void Legs::plotAvg(float savg[5][2], float avg[5][6], float xyzavg[6], float cur[5][6], int index) {
	float yscale = 0;
	if (index == 0) yscale = 20; 
	if (index == 1) yscale = .25;
	//ofFill();
	ofNoFill();
	ofPushMatrix();
	float allAvg = 0;
	for (int i = 0; i < 5; i++) {
		if (index == 0) ofSetColor(255, 0, 0, 150);
		else ofSetColor(0, 255, 0, 150);
		allAvg += savg[i][index];
		ofRectangle rect;
		rect.x = i * 90;
		rect.y = 0;
		rect.width = 90;
		rect.height = -savg[i][index] * yscale;
		ofDrawRectangle(rect);
		ofSetLineWidth(i+1);
		ofSetColor(255);
		ofDrawLine(rect.x, -50, rect.x, 50);
		ofSetLineWidth(5);
		for (int j = index*3; j < (index*3)+3; j++) {
			if (j == 0 || j == 3) ofSetColor(255, 0, 0);
			if (j == 1 || j == 4) ofSetColor(0, 255, 0);
			if (j == 2 || j == 5) ofSetColor(30, 30, 255);
			int m = j;
			if (index == 1) {
				m = j - 3;
			}
			rect.x = i * 90 + (m * 30);
			rect.width = 30;
			rect.height = -avg[i][j] * yscale;
			ofDrawRectangle(rect);
			ofFill();
			rect.x = i * 90 + (m * 30) + 10;
			rect.width = 8;
			rect.height = -cur[i][j] * yscale;
			ofDrawRectangle(rect);
			ofNoFill();
		}
		if (index == 0) ofSetColor(255, 0, 0, 150);
		else ofSetColor(0, 255, 0, 150);
		rect.x = 450;
		rect.width = 120;
		rect.height = -(allAvg / 5) * yscale;
		ofDrawRectangle(rect);
		ofSetLineWidth(6);
		ofSetColor(255);
		ofDrawLine(rect.x, -50, rect.x, 50);
		ofSetLineWidth(5);
		for (int j = index * 3; j < (index * 3) + 3; j++) {
			if (j == 0 || j == 3) ofSetColor(255, 0, 0);
			if (j == 1 || j == 4) ofSetColor(0, 255, 0);
			if (j == 2 || j == 5) ofSetColor(30, 30, 255);
			int m = j;
			if (index == 1) {
				m = j-3;
			}
			rect.x = 450 + (m * 40);
			rect.width = 40;
			rect.height = -xyzavg[j] * yscale;
			ofDrawRectangle(rect);
		}
	}
	ofPopMatrix();
}

void Legs::plotAvgs() {
	//ofFill();
	ofSetLineWidth(5);
	ofNoFill();
	//left ACCEL
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() / 4);
	plotAvg(lSavg, lavg, lxyzavg, leftCur, 0);
	ofPopMatrix();

	//right ACCEL
	ofPushMatrix();
	ofTranslate(3*ofGetWidth()/4 + 50, ofGetHeight()/4);
	plotAvg(rSavg, ravg, rxyzavg, rightCur, 0);
	ofPopMatrix();

	//left GYRO
	ofPushMatrix();
	ofTranslate(0, ofGetHeight()*3/4);
	plotAvg(lSavg, lavg, lxyzavg, leftCur, 1);
	ofPopMatrix();

	//right GYRO
	ofPushMatrix();
	ofTranslate(3 * ofGetWidth() / 4 + 50, ofGetHeight()*3/4);
	plotAvg(rSavg, ravg, rxyzavg, rightCur, 1);
	ofPopMatrix();
	ofSetLineWidth(1);
}

Legs::Legs()
{
}


Legs::~Legs()
{
	//legData.close();
	stop();
	waitForThread(true);
}
