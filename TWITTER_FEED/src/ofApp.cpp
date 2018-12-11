#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0,255,0);

	std::string file = json_file_name;
	ttf.load("verdana.ttf", 12, true, true, true, 0.3f, 0);
	bool parsingSuccessful = json.open(file);

	if (parsingSuccessful) {
		ofLogNotice("ofApp::setup") << json.getRawString();
	} else {
		ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
	}
		// for(auto & stroke: js){
		// 	if(!stroke.empty()){
		// 		path.moveTo(stroke[0]["x"], stroke[0]["y"]);
		// 		for(auto & p: stroke){
		// 			path.lineTo(p["x"], p["y"]);
		// 		}
		// 	}
		// }
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
	
	// cout << js_size << endl;
	if (js_size > 0) {
		min_index = js_size - tot_num_tweets;
		if (min_index < 0) min_index = 0;
		for (unsigned int i = min_index; i < js_size; i++) {
			// cout << json[ofToString(i)] << endl;
			const Json::Value author_full = json[ofToString(i)]["author"];
			const Json::Value text_full = json[ofToString(i)]["text"];
			// cout << "Tweet # " << ofToString(i) <<  " | Author " << author_full << " | Text " << text_full << endl;
			
			string author = ofToString(author_full);
			author.erase(
			    remove( author.begin(), author.end(), '\"' ), author.end());

			// cout << author << endl;

			string text = ofToString(text_full);
			text.erase(
			    remove( text.begin(), text.end(), '\"' ), text.end());

			ss << ofToString(author) + ": " + ofToString(text) << endl;
			ofSetColor(0);
			ttf.drawString(ss.str(), 20, 20);
		}
	}

	// ttf.drawStringAsShapes("hello", 20, 20);
	// ofDrawBitmapString(ss.str(), 10, 150);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}