#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0,255,0);
	ofTrueTypeFont::setGlobalDpi(72);


	std::string file = json_file_name;
	ttf.load("verdana.ttf", 14, true, true);
	ttf.setLineHeight(20.0f);
	bool parsingSuccessful = json.open(file);

	if (parsingSuccessful) {
		ofLogNotice("ofApp::setup") << json.getRawString();
	} else {
		ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
	}

	ofSetVerticalSync(true);
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	std::string file = json_file_name;
	std::stringstream ss;

	bool parsingSuccessful = json.open(file);
	int min_index;

	int js_size;
	if (parsingSuccessful) {
		js_size = json.size();
	} else {
		js_size = 0;
	}
	
	if (js_size > 0) {
		min_index = js_size - tot_num_tweets;
		if (min_index < 0) min_index = 0;
		for (unsigned int i = js_size-1; i > min_index; i--) {
			// cout << json[ofToString(i)] << endl;
			const Json::Value author_full = json[ofToString(i)]["author"];
			const Json::Value text_full = json[ofToString(i)]["text"];
			// cout << "Tweet # " << ofToString(i) <<  " | Author " << author_full << " | Text " << text_full << endl;
			
			string author = ofToString(author_full);
			author.erase(
			    remove( author.begin(), author.end(), '\"' ), author.end());
			author.erase(
			    remove( author.begin(), author.end(), '\n' ), author.end());

			// cout << author << endl;

			string text = ofToString(text_full);
			text.erase(
			    remove( text.begin(), text.end(), '\"' ), text.end());
			text.erase(
			    remove( text.begin(), text.end(), '\n' ), text.end());
			text.erase(
			    remove( text.begin(), text.end(), '\\' ), text.end());

			ss << ofToString(author) + ": " + ofToString(text) << endl;
			
		}
		// ofSetColor(255, 255, 255);
		// ttf.drawString(ss.str(), posX, posY);
		ofSetColor(red, green, blue);
		ttf.drawString(ss.str(), posX, posY);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case OF_KEY_RIGHT:
			posX ++;
			break; 
		case OF_KEY_LEFT:
			posX --;
			break; 
		case OF_KEY_DOWN:
			posY ++;
			break; 
		case OF_KEY_UP:
			posY --;
			break; 
		case 'a':
			tot_num_tweets++;
			break;
		case 's':
			tot_num_tweets--;
			break;
		case 'r':
			red++;
			if (red > 255) red = 255;
			break;
		case 't':
			red--;
			if (red < 0) red = 0;
			break;
		case 'g':
			green++;
			if (green > 255) green = 255;
			break;
		case 'h':
			green--;
			if (green < 0) green = 0;
			break;
		case 'b':
			blue++;
			if (blue > 255) blue = 255;
			break;
		case 'n':
			blue--;
			if (blue < 0) blue = 0;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}