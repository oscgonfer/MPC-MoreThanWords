#pragma once

#include "ofMain.h"
#include "ofSoundPlayer.h"
#include "ofxSimpleSerial.h"
#include <sstream>
#include <string>

#include <math.h> //floor
#include <list> //list
// #include <algorithm>    // std::max

using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void onNewMessage(string & message);

		enum GLOBAL_STATUS
		{
			AMBIENT_PRELOAD,
			AMBIENT_LOADED,
			FADE_OUT_AND_STOP,
			STORY_PRELOAD,
			STORY_LOADED,
			TITLE_PRELOAD,
			TITLE_LOADED
		};

		GLOBAL_STATUS status = TITLE_PRELOAD;

		// Global constants
		#define totRows 4
		#define totColumns 4

		// Indexes and others
		int indexTitle = -1;
		int indexAmbient = -1;

		int storyNumber = 0;
		int rowTitle;
		int colTitle;
		int rowAmbient;
		int colAmbient;

		// If pressed or not
		bool arrayButton[totRows][totColumns];

		ofSoundPlayer soundList[totRows][totColumns];
		int typeList[totRows][totColumns]; // 1: single / 2: loop / 3: ambient relocated
		float timeLastReleased[totRows][totColumns]; // time since it has been off

		float timeOffMin = 1; // Minimum time off for replay
		int timeFadeOut = 2; // Time in seconds to fade out
		float speedAmbient = 1;
		bool ambientHasBeenReleased = false;
		bool ambientHasBeenPressed = false;
		float targetSpeedAmbient = 2;
		float timeTransientSpeedAmbient = 1;
		float ambientReleaseTime = 0;
		float ambientPressTime = 0;
		// Draw

		int minRectPos = 40;
		int rectOffset = 30;
		int rectSide = 140;
		bool viewGraph = false;

		//SERIAL
    	ofxSimpleSerial    Serial;
    	ofSerialDeviceInfo SerialInfo;
    
    	string        messageSerial;
};