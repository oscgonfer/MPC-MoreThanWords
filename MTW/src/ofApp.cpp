#include "ofApp.h"
#include "lists.h"

#include <sstream>
#include <string>

//--------------------------------------------------------------

void ofApp::setup(){

	status = TITLE_PRELOAD;

	//Serial
    Serial.listDevices();
    
    vector <ofSerialDeviceInfo> deviceList = Serial.getDeviceList();
    cout <<"Device list size "<< ofToString(deviceList.size())<< endl;
    
    //FIND ARDUINO'S INDEX HERE
    Serial.setup(0, 9600);
    Serial.flush();

    // Some viz stuff
    ofBackground(40,41,35);

    // Init everything to not pressed
    for (int row = 0; row< totRows; row ++) {
    	for (int column = 0; column<totColumns; column++ ){
    		arrayButton[row][column] = false;
    	}
    }
}

//--------------------------------------------------------------
void ofApp::update(){

	bool pressed;
	bool isplaying;
	bool istitleplaying;
	bool isambientplaying;
	int indexList;
	float speedAmbient;

	// Read Serial
	int bytesRequired = 33;
	unsigned char bytes[bytesRequired];
	int bytesRemaining = bytesRequired;
	// loop until we've read everything
	while (bytesRemaining > 0)
	{
		// check for OF_SERIAL_NO_DATA
	  	if ( Serial.available() > 0 ) {
	    // try to read - note offset into the bytes[] array, this is so
	    // that we don't overwrite the bytes we already have
	    int bytesArrayOffset = bytesRequired - bytesRemaining;
	    int result = Serial.readBytes( &bytes[bytesArrayOffset],
	      bytesRemaining );

	    // check for error code
	    if ( result == OF_SERIAL_ERROR ) {
	    	// something bad happened
	    	ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
	      	// bail out
	    	break;
	    } else if ( result == OF_SERIAL_NO_DATA ) {
	    	// nothing was read, try again
	    } else {
	    	// we read some data!
	    	bytesRemaining -= result;
	    }
	  }
	}

	// look for the '\n' character
	int pos1 = 0;
	bool reset = true;
	for (int i = 0; i < bytesRequired; i++) {
        if ((bytes[i]== '\n') && pos1 == 0) {
            pos1 = i;
        }
    }

    // Check who has been pressed
	for (int i = 0; i < 16; i++) {

		int row = floor(i/totRows);
		int column = i-row*totRows;

		pressed = arrayButton[row][column];
		if (bytes[pos1 + i + 1] == '1') {
    		arrayButton[row][column] = true;
    	} else {
    		reset = false;
    		arrayButton[row][column] = false;
     		if (pressed) {
				timeLastReleased[row][column] = ofGetElapsedTimef();
			}
    	}
	}

	// Define reset status
    if (reset) {
    	cout << "RESET" << endl;
    	status = FADE_OUT_AND_STOP; 
    	reset = false;
    }

    // 'State machine'
	switch (status) {
		
		case TITLE_PRELOAD:
		{
			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					indexList = row * totRows + column;
					arrayButton[row][column] = false;
					soundList[row][column].load(pathtoSound + soundListTitle[indexList]);
					volumeList[row][column] = soundListTitleVol[indexList];
					// No time constraint
					timeLastReleased[row][column] = 0;
				}
			} 
			status = TITLE_LOADED;
			
			break;
		}
		
		case TITLE_LOADED:
		{
			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					pressed = arrayButton[row][column];
					isplaying = soundList[row][column].isPlaying(); // Not really necessary

					// Time Pressed check
					float lapseOff = ofGetElapsedTimef() - timeLastReleased[row][column];

					if (pressed && lapseOff > timeOffMin) {
						soundList[row][column].stop();
						soundList[row][column].play();
						soundList[row][column].setVolume(volumeList[row][column]);
						soundList[row][column].setMultiPlay(false);

						indexTitle = row * totRows + column;
						storyNumber = listTitleNumber[indexTitle];
						
						status = AMBIENT_PRELOAD;

						break;
					}
				}
			} 

			break;
		}

		case AMBIENT_PRELOAD:
		{
			// Define list Ambient from title selected
			std::vector<string> listAmbient;
			std::vector<float> listAmbientVol;

			switch (storyNumber){
				case 1:
					listAmbient = listAmbient_T1;
					listAmbientVol = listAmbient_T1Vol;
					break;
				case 2:
					listAmbient = listAmbient_T2;
					listAmbientVol = listAmbient_T2Vol;
					break;
				case 3: 
					listAmbient = listAmbient_T3;
					listAmbientVol = listAmbient_T3Vol;
					break;
				case 4: 
					listAmbient = listAmbient_T4;
					listAmbientVol = listAmbient_T4Vol;
					break;
			}

			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					indexList = row * totRows + column;

					// Unload everything except the title
					if (indexList!=indexTitle) {
						soundList[row][column].unload();
						// Load ambients
						soundList[row][column].load(pathtoSound + listAmbient[indexList]);	
						volumeList[row][column] = listAmbientVol[indexList];				
					}
				}
			} 
			
			status = AMBIENT_LOADED;
			
			break;
		}

		case AMBIENT_LOADED:
		{
			// Check if title is playing
			rowTitle = floor(indexTitle/totRows);
			colTitle = indexTitle-rowTitle*totRows;

			istitleplaying = soundList[rowTitle][colTitle].isPlaying();

			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					pressed = arrayButton[row][column];

					indexList = row * totRows + column;

					// Time Pressed check
					float lapseOff = ofGetElapsedTimef() - timeLastReleased[row][column];

					// If title pressed and it has been released
					if (pressed && indexList==indexTitle && lapseOff>timeOffMin) {
						soundList[row][column].stop();
						soundList[row][column].play();
						soundList[row][column].setVolume(volumeList[row][column]);
						soundList[row][column].setMultiPlay(false);

					}

					// Play ambient
					if (pressed && indexList!=indexTitle) {
						soundList[row][column].play();
						soundList[row][column].setVolume(volumeList[row][column]);

						indexAmbient = indexList;
						ambientHasBeenPressed = true;
						ambientHasBeenReleased = false;
						status = STORY_PRELOAD;

						break;
					}
				}
			}
			break;
		}
		case STORY_PRELOAD:
		{
			// Define list sounds
			std::vector<string> listSounds;
			std::vector<int> listType;
			std::vector<float> listSoundVol;

			switch (storyNumber){
				case 1:
					listSounds = listSounds_T1;
					listType = listSoundsTypes_T1;
					listSoundVol = listSounds_T1Vol;

					break;
				case 2:
					listSounds = listSounds_T2;
					listType = listSoundsTypes_T2;
					listSoundVol = listSounds_T2Vol;

					break;
				case 3: 
					listSounds = listSounds_T3;
					listType = listSoundsTypes_T3;
					listSoundVol = listSounds_T3Vol;

					break;
				case 4: 
					listSounds = listSounds_T4;
					listType = listSoundsTypes_T4;
					listSoundVol = listSounds_T4Vol;

					break;
			}

			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					indexList = row * totRows + column;

					// Unload everything except the title and ambient
					if (indexList!=indexTitle && indexList!=indexAmbient) {
						soundList[row][column].unload();
						// Load stories
						soundList[row][column].load(pathtoSound + listSounds[indexList]);	
						typeList[row][column] = listType[indexList];
						volumeList[row][column] = listSoundVol[indexList];
						// TO CHECK WHERE TO PUT THE OTHER AMBIENTS					
					}
				}
			} 
			
			status = STORY_LOADED;
			
			break;
		}
		case STORY_LOADED:
		{
			// Check if title is playing
			rowTitle = floor(indexTitle/totRows);
			colTitle = indexTitle-rowTitle*totRows;

			istitleplaying = soundList[rowTitle][colTitle].isPlaying();

			// Check if ambient is playing
			rowAmbient = floor(indexAmbient/totRows);
			colAmbient = indexAmbient-rowAmbient*totRows;

			isambientplaying = soundList[rowAmbient][colAmbient].isPlaying();
			speedAmbient = soundList[rowAmbient][colAmbient].getSpeed();

			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					indexList = row * totRows + column;

					pressed = arrayButton[row][column];

					// Time Pressed check
					float lapseOff = ofGetElapsedTimef() - timeLastReleased[row][column];
					
					// Title case
					if (pressed && indexList==indexTitle && lapseOff>timeOffMin) {
						soundList[row][column].stop();
						soundList[row][column].play();
						soundList[row][column].setVolume(volumeList[row][column]);
						soundList[row][column].setMultiPlay(false);
					}

					// Case when ambient is done on it's own
					if (pressed && indexList==indexAmbient && !isambientplaying) {
						soundList[row][column].play();
						soundList[row][column].setVolume(volumeList[row][column]);
						soundList[row][column].setMultiPlay(false);
						soundList[row][column].setSpeed(1.0f);
						ambientHasBeenReleased = false;
						ambientHasBeenPressed = true;
					}

					// Check when ambient has been released after pressed
					if (!pressed && indexList == indexAmbient && ambientHasBeenPressed){
						// Ambient has been released
						ambientHasBeenReleased = true;
						ambientHasBeenPressed = false;
						ambientReleaseTime = ofGetElapsedTimef();
					}

					// Check when ambient has been pressed after released
					if (pressed && indexList == indexAmbient && ambientHasBeenReleased) {
						ambientHasBeenPressed = true;
						ambientHasBeenReleased = false;
						ambientPressTime = ofGetElapsedTimef();
					}

					// Change speed of ambient in case of release
					if (!pressed && ambientHasBeenReleased && speedAmbient > 1 && indexList == indexAmbient && isambientplaying) {
						float newSpeedAmbient = fmax(1, targetSpeedAmbient - (ofGetElapsedTimef() - ambientReleaseTime)/timeTransientSpeedAmbient);
						// Put speed back to normal
						soundList[row][column].setSpeed(newSpeedAmbient);
					}

					// Change speed of ambient in case of re-press
					if (pressed && ambientHasBeenPressed && indexList==indexAmbient && isambientplaying) {
						float newSpeedAmbient = fmax(targetSpeedAmbient, (ofGetElapsedTimef()-ambientPressTime)/timeTransientSpeedAmbient + 1);
						// Set the desired speed
						soundList[row][column].setSpeed(newSpeedAmbient);
					}

					// TO CHECK WITH REBECCA: IF WE HIT ANOTHER AMBIENT DOES IT STOP THE OTHER AND START THE NEW ONE?
					// if (pressed && typeList[row][column]==3 && isambientplaying && indexList!=indexAmbient && indexList!=indexTitle) {
						// soundList[rowAmbient][colAmbient].stop();
						// soundList[row][column].play();
						// indexAmbient = indexList;
					// }

					// Normal pressed 
					if (pressed && indexList!=indexTitle && indexList!=indexAmbient) {
						
						// If it's not already playing, start it
						if (!soundList[row][column].isPlaying()) {

							soundList[row][column].play();
							soundList[row][column].setVolume(volumeList[row][column]);

							if (typeList[row][column]==2){
								// The sound type is loop
								soundList[row][column].setLoop(true);
								soundList[row][column].setMultiPlay(false);

							} else if (typeList[row][column]==1) {
								// The sound type is single
								soundList[row][column].setLoop(false);
								// soundList[row][column].setMultiPlay(true); // be careful with this

							}

						} else if (soundList[row][column].isPlaying() && lapseOff > timeOffMin){
							// If we press-release-press one that is in multiplay, single, stop and play again
							if (typeList[row][column]==1) {
								soundList[row][column].stop(); // be careful with this
								soundList[row][column].play();
								soundList[row][column].setVolume(volumeList[row][column]);

							} else if (typeList[row][column] == 2) {
								// If we press one that is in loop, but it is still playing
								soundList[row][column].setLoop(true);
							}

						}

					}

					if (!pressed && indexList!=indexTitle && indexList!=indexAmbient && typeList[row][column]==2) {
						// If it's a loop sound, let it finish once is not pressed
						if (soundList[row][column].isPlaying()){
							soundList[row][column].setLoop(false);
						}
					}

					// Do we want to let the type==1 stop by themselves -> then do nothing
					// or do we want to stop when they step off of them? -> then stop it below
				}
			}

			break;
		}

		case FADE_OUT_AND_STOP:
		{
			float timeLeftFadeOut = 0;
			float timeStartFadeOut = 0;
			float targetVolume = 1;
			
			// Set time left for fade out as max time for fade out
			timeLeftFadeOut = timeFadeOut;
			// Get time at which we will start fading out
			timeStartFadeOut = ofGetElapsedTimef(); 

			while (timeLeftFadeOut>0) {
				// Re calculate time left
				timeLeftFadeOut = timeFadeOut - (ofGetElapsedTimef() - timeStartFadeOut);
				// Calculate the target volume
				targetVolume = timeLeftFadeOut/timeFadeOut;

				for (int row = 0; row < totRows; row++){
					for (int column = 0; column < totColumns; column++){

						if (targetVolume == 0) {
							soundList[row][column].stop();
						} else {
							soundList[row][column].setVolume(targetVolume);
						}
					}
				}
			}
		
			status = TITLE_PRELOAD;

			break;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){


	if (viewGraph) {
		for (int i = 0; i<totRows; i++){
			for (int j = 0; j<totColumns; j++) {

				bool pressed = arrayButton[i][j];

				if (pressed) {
					// cout << i << " - " << j << " - " << pressed;
					ofSetColor(248,35,114);
				} else {
					ofSetColor(235,237,239);
				}
	    		ofFill();

				ofRectangle rect;

	    		rect.x = minRectPos + i*(rectSide+rectOffset);
	    		rect.y = minRectPos + j*(rectSide+rectOffset);
	    		rect.width = rectSide;
	    		rect.height = rectSide;

	    		ofDrawRectangle(rect);

	   //  		// Text in center if is playing
	   //  		int textOnX = rect.x + rectSide/2;
	   //  		int textOnY = rect.y + rectSide/2;

	   //  		bool playing;
	   //  		playing = soundList[i][j].isPlaying();
				ofSetColor(40,41,35);
	   //  		stringstream text;
	   //  		if (playing) {
	   //  			text << "ON" << endl;
	   //  		} else {
	   //  			text << "OFF" << endl;
	   //  		}

	   //          ofDrawBitmapString(text.str(), textOnX, textOnY);

	            // Text below for time last released
	            int textReleasedX = rect.x + rectSide/10;
	    		int textReleasedY = rect.y + rectSide/10; 

	    		float time = timeLastReleased[i][j];
	    		stringstream textTime;
	    		textTime << "Time Rel: " << time << endl;

				ofDrawBitmapString(textTime.str(), textReleasedX, textReleasedY);

				// // Text below for time last released
	   //          int textSpeedX = rect.x + rectSide/10;
	   //  		int textSpeedY = rect.y + 8*rectSide/10; 


				// float speed = 0;
	   //  		speed = soundList[i][j].getSpeed();
	   //  		stringstream textSpeed;
	   //  		textSpeed << "Speed: " << speed << endl;

				// ofDrawBitmapString(textSpeed.str(), textSpeedX, textSpeedY);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
        case 'v':
        	if (!viewGraph){
        		viewGraph = true;
        	} else {
        		viewGraph = false;
        	}
            break;
    }
}
