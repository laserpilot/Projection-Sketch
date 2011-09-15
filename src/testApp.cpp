#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(false);
	ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    camWidth = 640;
    camHeight = 480;
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    colorImg.allocate(camWidth,camHeight);
	grayImg.allocate(camWidth,camHeight);
    threshold = 160;
    
    ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void testApp::update(){
    bool bNewFrame = false;
    
    vidGrabber.grabFrame();
    bNewFrame = vidGrabber.isFrameNew();
    
    if (bNewFrame){
        colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
        grayImg=colorImg;
        grayImg.threshold(threshold);
        contourFinder.findContours(grayImg, 200, (camWidth*camHeight)/3, 10, true);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    colorImg.draw(0,0,ofGetWidth(),0.75*ofGetWidth());
    ptAvg=0;
   // noise.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
   // noise.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
   // noise.setUniform2f("mouse", mouseX - ofGetWidth()/2, ofGetHeight()/2-mouseY );
    
	for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
		ofNoFill();
		ofBeginShape();
		for( int j=0; j<contourFinder.blobs[i].nPts; j++ ) {
            mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
            mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
            ofVertex( mappedX, mappedY );
            // ofCircle(mappedX, mappedY, 20);
            // ofDrawBitmapString(ofToString(i), mappedX,mappedY);
            // ofDrawBitmapString(ofToString(j), mappedX+20,mappedY);
		}
		ofEndShape();
        for( int k=0; k<contourFinder.blobs[i].nPts; k+=50){
            mappedX=ofMap(contourFinder.blobs[i].pts[k].x,0,camWidth,0,ofGetWidth());
            mappedY=ofMap(contourFinder.blobs[i].pts[k].y,0,camHeight,0,.75*ofGetWidth());
            ofCircle(mappedX, mappedY, 10);
        }
	}
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
		case 'p':
			threshold +=10;
			if (threshold > 255) threshold = 255;
			break;
		case 'o':
			threshold -=10;
			if (threshold < 0) threshold = 0;
			break;
        case OF_KEY_RETURN:
            ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}