#pragma once

#include "ofMain.h"
#include "ofSoundPlayer.h"
#include "ofxSimpleSerial.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"


#include <sstream>
#include <string>

#include <math.h> //floor
#include <list> //list

#define HOST "localhost"
#define PORT_OSC_SENDER_PYTHON 8000
#define PORT_OSC_RECEIVER_PYTHON 53000

using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void onNewMessage(string & message);

		enum GLOBAL_STATUS
		{
			FADE_OUT_AND_STOP,
			STORY_PRELOAD,
			STORY_LOADED
		};

		GLOBAL_STATUS status = STORY_PRELOAD;

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
		float volumeList[totRows][totColumns];
		float timeLastReleased[totRows][totColumns]; // time since it has been off
		float timeLastPressed[totRows][totColumns]; // time since it has been on
		bool soundHasBeenReleased[totRows][totColumns];
		bool soundHasBeenPressed[totRows][totColumns];
		bool fadingOut[totRows][totColumns];
		float timeLeftFadeOut[totRows][totColumns];
		float timeStartFadeOut[totRows][totColumns];
		float targetFactor[totRows][totColumns];
		float timeFadeOut = 3; // Time in seconds to fade out looping sounds


		float timeOffMin = 0.5; // Minimum time off for replay
		float timeOnMin = 0.5; // Minimum time off for replay

		bool soundLoaded = false;

		// Fade Out when reset button is pressed
		int timeFadeOutAll = 1; // Time in seconds to fade out

		// Ambient specifics
		float speedAmbient = 1;
		bool ambientHasBeenReleased = false;
		bool ambientHasBeenPressed = false;
		bool ambientHasBeenOnceReleased = false;
		float targetSpeedAmbient = 1.5;
		float timeTransientSpeedAmbient = 2;
		float ambientReleaseTime = 0;
		float ambientPressTime = 0;

		// Draw
		int minRectPos = 40;
		int rectOffset = 30;
		int rectSide = 140;
		bool viewGraph = false;

		//SERIAL
    	ofSerial Serial;
    	ofSerialDeviceInfo SerialInfo;

    	// UDP
		ofxUDPManager udpConnection;

		// used for sending the osc messages to qlab
    	ofxOscSender sender_PYTHON;
    	ofxOscReceiver receiver_PYTHON;
    
};