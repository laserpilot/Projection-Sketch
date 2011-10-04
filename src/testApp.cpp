#include "testApp.h"

GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
	//ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    glShadeModel (GL_SMOOTH);
    
    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
    
    camWidth = 640;
    camHeight = 480;
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    colorImg.allocate(camWidth,camHeight);
	grayImg.allocate(camWidth,camHeight);
    threshold = 120;
    
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
    ofSetRectMode(OF_RECTMODE_CORNER);
    colorImg.draw(0,0,ofGetWidth(),0.75*ofGetWidth());
    
    switch (keyPress) {
        case 'q':
            //Thickness as a function of point value in blob
                for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                    ofNoFill();
                    ofBeginShape();
                    linethick=0.1;
                    for( int j=0; j<contourFinder.blobs[i].nPts; j=j+30 ) {
                        mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                        mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                        linethick=ofMap(j,0,contourFinder.blobs[i].nPts,0.0,4.0);
                        ofSetLineWidth(linethick);
                        ofVertex( mappedX, mappedY );
                        ofCircle(mappedX, mappedY, 20);
                    }
                    ofEndShape();
                    
                }
                ofNoFill();
            break;
        case 'w' :
            //Centroid to circle
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                ofBeginShape();
                ofSetLineWidth(1);
                for( int k=0; k<contourFinder.blobs[i].nPts; k+=50){
                    mappedX=ofMap(contourFinder.blobs[i].pts[k].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[k].y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex( mappedX, mappedY );
                    mapCentX = ofMap(contourFinder.blobs[i].centroid.x,0,camWidth,0,ofGetWidth());
                    mapCentY = ofMap(contourFinder.blobs[i].centroid.y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex( mapCentX, mapCentY);
                    ofCircle(mappedX, mappedY, 10);
                }
                ofEndShape();
            }
            break;
        case 'e' :
            //Plain Draw
            //Number values of points
            ofSetColor(255,255,255);
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                    ofNoFill();
                    ofBeginShape();
                    linethick=0.1;
                    for( int j=0; j<contourFinder.blobs[i].nPts; j=j+30 ) {
                        mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                        mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                        ofVertex( mappedX, mappedY );
                        ofDrawBitmapString(ofToString(i), mappedX,mappedY);
                         ofDrawBitmapString(ofToString(j), mappedX+20,mappedY);
                    }
                    ofEndShape();
            }
            break;
        case 'r' :
            //Plain Draw
            //Fuzzy Caterpillar
            ofEnableSmoothing();
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                //ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j++ ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofSetColor(255, 255, ofMap(j, 0, contourFinder.blobs[i].nPts, 40, 255));
                    ofLine(mappedX, 
                           mappedY,
                           mappedX+(sin(ofGetElapsedTimef()*ofMap(j, 0, contourFinder.blobs[i].nPts, 5, 40))*(ofMap(j, 0, contourFinder.blobs[i].nPts, 5, 40))), 
                           mappedY+(cos(ofGetElapsedTimef()*ofMap(j, 0, contourFinder.blobs[i].nPts, 5, 40))*10));
                }
               // ofEndShape();
                ofSetColor(255,255,255);
            }
            ofNoFill();
            break;
        case 't' :
            //Draw Squares
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
               // ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+15 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofRect(mappedX,mappedY,ofMap(mappedX,0,ofGetWidth(),1,30),ofMap(mappedY,0,ofGetHeight(),1,30));
                }
               // ofEndShape(); 
            }  
            break;
            
        case 'y' :
            //Draw Squares
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofFill();
                // ofBeginShape();
                for( int j=10; j<contourFinder.blobs[i].nPts; j=j+10 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofPushMatrix();
                    ofTranslate(mappedX, mappedY);
                    ofRotateZ(ofRadToDeg(atan2((mappedX-contourFinder.blobs[i].pts[j-10].x),(mappedY-contourFinder.blobs[i].pts[j-10].y))*(PI/2)));
                    ofRect(0,0,20,20);
                    ofPopMatrix();
                }
                // ofEndShape(); 
            }  
            break;
            
        case 'u' :
            //Centroid
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+5 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex(mappedX,mappedY);
                }
                ofEndShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+10 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    mapCentX = ofMap(contourFinder.blobs[i].centroid.x,0,camWidth,0,ofGetWidth());
                    mapCentY = ofMap(contourFinder.blobs[i].centroid.y,0,camHeight,0,.75*ofGetWidth());
                    ofLine(mappedX,
                           mappedY,
                           mapCentX+(20*sin(ofGetElapsedTimef())),
                           mapCentY+(20*cos(ofGetElapsedTimef()))
                                     );
                }
            }  
            break;
            
        case 'i' :
            //Centroid
            tog=0;
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+5 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex(mappedX,mappedY);
                }
                ofEndShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+30 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    mapCentX = ofMap(contourFinder.blobs[i].centroid.x,0,camWidth,0,ofGetWidth());
                    mapCentY = ofMap(contourFinder.blobs[i].centroid.y,0,camHeight,0,.75*ofGetWidth());
                    if((tog%2)==0){
                        ofFill();
                        cout << "Even" <<endl;
                    }
                    else{
                        ofNoFill();
                        cout << "Odd" <<endl;
                    }
                    ofTriangle(mappedX, mappedY, 
                               ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth()), 
                               ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth()), 
                               mapCentX, mapCentY);     
                    tog++;
                }
            }  
            break;

        case 'a' :
            //GL FIRE
            glEnable(GL_DEPTH_TEST);
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofFill();
                //ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
                ofBeginShape();
                for( int j=0; j<(contourFinder.blobs[i].nPts-50); j=j+15 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    //ofBox( mappedX, mappedY,0,30 );
                    glBegin(GL_QUADS);
                        glColor4f(1.0, 0, 0, 1.0);
                        glVertex3f(mappedX,mappedY ,0.0 );
                        glColor4f(1.0, 1.0, 0,1.0);
                        glVertex3f(ofMap(contourFinder.blobs[i].pts[j+50].x,0,camWidth,0,ofGetWidth()),
                                   ofMap(contourFinder.blobs[i].pts[j+50].y,0,camHeight,0,.75*ofGetWidth()) ,
                                   0.0 );
                        glVertex3f(ofMap(contourFinder.blobs[i].pts[j+30].x,0,camWidth,0,ofGetWidth()),
                                   ofMap(contourFinder.blobs[i].pts[j+30].y,0,camHeight,0,.75*ofGetWidth()) ,
                                   0.0 );
                        glVertex3f(ofMap(contourFinder.blobs[i].pts[j+15].x,0,camWidth,0,ofGetWidth()),
                                   ofMap(contourFinder.blobs[i].pts[j+15].y,0,camHeight,0,.75*ofGetWidth()) ,
                                   0.0 );
                        glEnd();
                    
                }
            
                ofEndShape();  
            } 
            glDisable(GL_DEPTH_TEST);
            break;
        case 's' :
            //GL FIRE Centroid
            glEnable(GL_DEPTH_TEST);
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofFill();
                //ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
                ofBeginShape();
                for( int j=0; j<(contourFinder.blobs[i].nPts-50); j=j+15 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    mapCentX = ofMap(contourFinder.blobs[i].centroid.x,0,camWidth,0,ofGetWidth());
                    mapCentY = ofMap(contourFinder.blobs[i].centroid.y,0,camHeight,0,.75*ofGetWidth());

                    //ofBox( mappedX, mappedY,0,30 );
                    glBegin(GL_QUADS);
                    glColor4f(1.0, 0, 0, 1.0);
                    glVertex3f(mappedX,mappedY ,0.0 );
                    glColor4f(1.0, 1.0, 0,1.0);
                    glVertex3f(ofMap(contourFinder.blobs[i].pts[j+50].x,0,camWidth,0,ofGetWidth()),
                               ofMap(contourFinder.blobs[i].pts[j+50].y,0,camHeight,0,.75*ofGetWidth()) ,
                               0.0 );
                    glVertex3f(ofMap(contourFinder.blobs[i].pts[j+30].x,0,camWidth,0,ofGetWidth()),
                               ofMap(contourFinder.blobs[i].pts[j+30].y,0,camHeight,0,.75*ofGetWidth()) ,
                               0.0 );
                    glVertex3f(mapCentX,
                               mapCentY ,
                               0.0 );
                    glEnd();
                    
                }
                ofEndShape();  
            }  
            glDisable(GL_DEPTH_TEST);
            break;
            case 'd':
            //Plain Draw`
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j=j+4 ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex( mappedX, mappedY );
                    ofVertex( 0, mappedY );

                }
                ofEndShape();  
            }  
            break;

        default:
            //Plain Draw`
            for( int i=0; i<(int)contourFinder.blobs.size(); i++ ) {
                ofNoFill();
                ofBeginShape();
                for( int j=0; j<contourFinder.blobs[i].nPts; j++ ) {
                    mappedX=ofMap(contourFinder.blobs[i].pts[j].x,0,camWidth,0,ofGetWidth());
                    mappedY=ofMap(contourFinder.blobs[i].pts[j].y,0,camHeight,0,.75*ofGetWidth());
                    ofVertex( mappedX, mappedY );
                }
                ofEndShape();  
            }  
            break;
    }
    ofSetColor(255,255,255);
    
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
    keyPress=key;
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