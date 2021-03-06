/**
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include "tests.hpp"
#include "sliders.hpp"
#include "logger.hpp"


#define DESKTOP
#define TEST

#ifdef DESKTOP

// -------------- Window Manager Class -------------

class WindowManager {
 private:
    vector<Mat*> OUT;
    vector<string> NAMES;
 public:
    WindowManager(vector<Mat*> out, vector<string> names) : OUT(out), NAMES(names) {
        for (string n : names) {
            namedWindow(n, WINDOW_NORMAL);
        }
    }
    void update() {
        for (unsigned int i = 0; i < OUT.size(); i++) {
            if(OUT[i]!=nullptr && !(*OUT[i]).empty()) {imshow(NAMES[i], *(OUT[i]));}
            else if (OUT[i]==nullptr) {imshow(NAMES[i],Mat());}
        }
    }
    void close() {
        destroyAllWindows();
    }
};

// ------ Video and Image Processing Methods ---------------

void videoProcess(VideoCapture cap, Capture* c) {
#ifdef TEST
    cout << "Running Capture::webCam..." << endl;
#endif
    // Variable Declaration
    Mat drawing, preview, canny, info;
    vector<Mat> proc;
    string filename, filepath;
    bool found = false; bool saved = false;

    // Create Windows
    vector<Mat*> images = vector<Mat*>{&drawing, &preview, &canny, &info};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save.",
        "Canny Edge Detection",
        "Data"};
    WindowManager win = WindowManager(images, names);

    if(!cap.isOpened()){  // check if we succeeded
        cout << "Camera failed to open!" << endl;
        return;
    }

    //Create Trackbars
    //Canny
    Slider e1("eTol1", "Canny Edge Detection", ETOL1, c, 255);
    Slider e2("eTol2", "Canny Edge Detection", ETOL2, c, 255);
    Slider es("eSize", "Canny Edge Detection", ESIZE, c, 21);
    Slider bs("bSize", "Canny Edge Detection", BSIZE, c, 21);
    Slider bg("bSigma", "Canny Edge Detection", BSIGMA, c, 21);

    //Poly Approx
    Slider pt("polyTol", "Data", POLYTOL, c, 50);
    Slider at("angleTol", "Data", ANGLETOL, c, 50);
    Slider dt("distRatio", "Data", DISTRATIO, c, PtoInt(0.2));

    // SizeRatio
    Slider sr("sizeRatio", "Frame: Press 'q' to exit.", SIZERATIO, c, PtoInt(1.0));

#ifdef TEST
    cout << "Video: Beginning Main Loop..." << endl;
#endif

    for (;;) {
        // Process Frame
    	Mat frame;
        cap >> frame;
        if ( frame.empty() ) {break;}  // end of video stream
        c->Frame(frame);
        proc = c->process();

#ifdef TEST
        cout << "webCam: Process Complete!" << endl;
#endif

        // Save processed frame to appropriate outputs
        if (proc.size()!=1) {
            drawing = proc[0];
            preview = proc[1];
            found = true;
            saved = false;
        } else {
            drawing = proc[0];
        }
        canny = c->drawEdges();
        info = c->drawInfo();

        // Show frame out and preview
        win.update();

        // Save File
        if (cvWaitKey(10) == 's' && found && !saved) {
            filename = std::tmpnam(NULL);
            filepath = "scans/" + filename + ".jpg";
            imwrite(filepath, preview);
            saved = true;
#ifdef TEST
            cout << "webCam: Saved as: " << filepath << endl;
#endif
        }

        // Quit
        if (cvWaitKey(10) == 'q') {break;}
    }
    cap.release();
    win.close();
}

// ---------- Derivative Video Methods -------------
void webCam (Capture* c) {
    VideoCapture cap(0);
    if(!cap.open(0)) {
        cout << "Camera failed to open..." << endl;
        return;
    }
    videoProcess(cap, c);
}

void videoFile (char* filepath, Capture* c) {
    VideoCapture cap(filepath);
    if (!cap.open(filepath)) {
        std::cout << "!!! Failed to open file: " << filepath << std::endl;
        return;
    }
    videoProcess(cap, c);
}


// ------------------ Main Method --------------

int main(int argc,char *argv[]) {

	initializeLogger();

// This is intended for newcomers to this project to play around with the logger 
// and understand the logging practices followed in this project.Uncomment and 
// compile to invoke the tests. 

//    testlogger(); 

#ifdef TEST
    cout << "Running main..." << endl;
    testGeometry();
#endif
    Capture *C = new Capture();

    if (argc == 1) {
        webCam(C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'h') {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'v') {
        videoFile(argv[3], C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'i') {
        //imageFile(argv[3], &C);
    } else {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    }
    delete C;

// Logger cleanup.
     log4cpp::Category::shutdown();
}

#endif
