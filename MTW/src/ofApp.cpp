#include "ofApp.h"
#include "lists.h"
#include <math.h> //floor
#include <list> //list

//--------------------------------------------------------------

void ofApp::setup(){

	status = TITLE_PRELOAD;

	//Serial
    Serial.listDevices();
    
    vector <ofSerialDeviceInfo> deviceList = Serial.getDeviceList();
    cout <<"Device list size "<< ofToString(deviceList.size())<< endl;
    
    //FIND ARDUINO'S INDEX HERE
    Serial.setup(0, 9600); //FIX IF THIS
    Serial.startContinuousRead();
    ofAddListener(Serial.NEW_MESSAGE, this, &ofApp::onNewMessage);

    messageSerial = "";

    // Some viz stuff
    ofBackground(40,41,35);
}

//--------------------------------------------------------------
void ofApp::update(){
	bool pressed;
	bool isplaying;
	bool istitleplaying;
	bool isambientplaying;
	int indexList;

	switch (status) {
		
		case TITLE_PRELOAD:
		{
			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){

					indexList = row * totRows + column;
					arrayButton[row][column] = false;
					soundList[row][column].load(pathtoSound + soundListTitle[indexList]);
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
			switch (storyNumber){
				case 1:
					listAmbient = listAmbient_T1;
					break;
				case 2:
					listAmbient = listAmbient_T2;
					break;
				case 3: 
					listAmbient = listAmbient_T3;
					break;
				case 4: 
					listAmbient = listAmbient_T4;
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
						soundList[row][column].setMultiPlay(false);
					}

					// Play ambient
					if (pressed && indexList!=indexTitle) {
						soundList[row][column].play();

						indexAmbient = indexList;

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

			switch (storyNumber){
				case 1:
					listSounds = listSounds_T1;
					listType = listSoundsTypes_T1;

					break;
				case 2:
					listSounds = listSounds_T2;
					listType = listSoundsTypes_T2;

					break;
				case 3: 
					listSounds = listSounds_T3;
					listType = listSoundsTypes_T3;

					break;
				case 4: 
					listSounds = listSounds_T4;
					listType = listSoundsTypes_T4;

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
						soundList[row][column].setMultiPlay(false);
					}

					// TO CHECK WITH REBECCA: IF WE HIT THE SAME AMBIENT IS PLAYING DOES IT START OVER?
					if (pressed && indexList==indexAmbient && !isambientplaying) {
						soundList[row][column].play();
						soundList[row][column].setMultiPlay(false);
					}

					if (pressed && indexList==indexAmbient && isambientplaying) {
						// Do something to accelerate the ambient (modify it)
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
							} else if (typeList[row][column] ==2) {
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
			for (int row = 0; row < totRows; row++){
				for (int column = 0; column < totColumns; column++){
					// Implement something for fade out and stop
					soundList[row][column].stop();
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

	    		// Text in center if is playing
	    		int textOnX = rect.x + rectSide/2;
	    		int textOnY = rect.y + rectSide/2;

	    		bool playing;
	    		playing = soundList[i][j].isPlaying();
				ofSetColor(40,41,35);
	    		stringstream text;
	    		if (playing) {
	    			text << "ON" << endl;
	    		} else {
	    			text << "OFF" << endl;
	    		}

	            // reportStream2 << "P position" <<p.x<<" - "<<p.y<< endl;
	            
	            ofDrawBitmapString(text.str(), textOnX, textOnY);

	            // Text below for time last released
	            int textReleasedX = rect.x + rectSide/10;
	    		int textReleasedY = rect.y + rectSide/10; 

	    		float time = 0;
	    		stringstream textTime;
	    		textTime << "Time Rel: " << time << endl;

				ofDrawBitmapString(textTime.str(), textReleasedX, textReleasedY);

				// Text below for time last released
	            int textSpeedX = rect.x + rectSide/10;
	    		int textSpeedY = rect.y + 8*rectSide/10; 


				float speed = 0;
	    		speed = soundList[i][j].getSpeed();
	    		stringstream textSpeed;
	    		textSpeed << "Speed: " << speed << endl;

				ofDrawBitmapString(textSpeed.str(), textSpeedX, textSpeedY);


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

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


void ofApp::onNewMessage(string & message)
{
	bool pressed;
	cout << "onNewMessage, message: " << message << "\n";

	if (message == "RESET") {
		status = FADE_OUT_AND_STOP;
	}
	
	for (int row=0; row<totRows; row++){
		for (int column=0; column<totColumns; column++){

			int indexList = row * totRows + column;
			pressed = arrayButton[row][column];

			if (int(message[indexList]) == 1){
				arrayButton[row][column] = true;
			} else {
				arrayButton[row][column] = false;
				// if the previous state was on, then set release time
				if (pressed) {
					timeLastReleased[row][column] = ofGetElapsedTimef();
				}
			}
		}
	}
}

