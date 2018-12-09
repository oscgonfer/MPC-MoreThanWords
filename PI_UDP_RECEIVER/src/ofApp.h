#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofUtils.h"
#include "ofPath.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		ofxUDPManager udpConnection;
		ofTrueTypeFont  mono;
		ofTrueTypeFont  monosm;
		int lengthUdp;

		#define totRows 4
		#define totColumns 4

		int array[totRows][totColumns];
		float arrayUpdateTime[totRows][totColumns];
		float resetTime;
		int reset_counter;
		int timeFadeOutUpdate = 6; // Time in seconds to fade out after new status
		int timeFadeOutAll = 3; // Time in seconds to fade out after reset

		// Draw
		int minRectPosx = 45;
		int minRectPosy = 45;
		int rectOffset = 75;
		int rectSide = 180;
		int thickness = 9;
};

