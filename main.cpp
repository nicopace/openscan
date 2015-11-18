/**
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include<string>
#include "cvmethods.hpp"
#include "capture.hpp"

#define DESKTOP
#define TEST


#ifdef DESKTOP

// -------------- Window Manager Class -------------

class WindowManager {
 private:
    vector<*Mat> OUT;
    vector<string> NAMES;
 public:
    WindowManager(vector<*Mat> inputs, vector<string> names) :
        OUT(inputs), NAMES(names) {
        for (string n : names) {
            namedWindow(n, WINDOW_NORMAL);
        }
    }
    void update() {
        for (int i = 0; i < OUT.size(); i++) {
            imshow(NAMES[i], *OUT[i]);
        }
    }
    void close() {
        destroyAllWindows();
    }
}

// ------ Video and Image Processing Methods ---------------

void imageFile (char *filepath, vector<Mat> (*process)(Mat)) {
    // Variable Declaration
    Mat frame, drawing, preview;
    vector<Mat> proc;
    string filename,filepath;
    bool found = false; bool saved = false;

    // Create Windows
    vector<*Mat> images = vector<*Mat>{&drawing, &preview};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save."};
    WindowManager win = WindowManager(images, names);

    vector<Mat> proc = process(frame);

    // Save processed frame to appropriate outputs
    if (!proc.empty()) {
        drawing = proc[0];
        preview = proc[2];
        found = true;
    } else {
        drawing = frame;
    }

    win.update();

    // Save processed frame to appropriate outputs
    if (!proc.empty()) {
        drawing = proc[0];
        preview = proc[2];
    } else {
        cout << "No scan found." << endl;
        cap.release();
        win.close();
        return;
    }

    // Show frame out and preview
    win.update();

    for (;;) {
        // Save File
        if (cvWaitKey(10) == 's') {
            filename = std::tmpnam(nullptr);
            filepath = "scans/" + filename + ".jpg";
            imwrite(filepath, preview);
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

void videoProcess(VideoCapture cap, vector<Mat> (*process)(Mat)) {
#ifdef TEST
    cout << "Running Capture::webCam..." << endl;
#endif
    // Variable Declaration
    Mat frame, drawing, preview;
    vector<Mat> proc;
    string filename, filepath;
    bool found = false; bool saved = false;

    // Create Windows
    vector<*Mat> images = vector<*Mat>{&drawing, &preview};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save."};
    WindowManager win = WindowManager(images, names);

#ifdef TEST
    cout << "Video: Beginning Main Loop..." << endl;
#endif

    for (;;) {
        // Process Frame
        cap >> frame;
        if ( frame.empty() ) {break;}  // end of video stream
        vector<Mat> proc = process(frame);

#ifdef TEST
        cout << "webCam: Process Complete!" << endl;
#endif

        // Save processed frame to appropriate outputs
        if (!proc.empty()) {
            drawing = proc[0];
            preview = proc[2];
            found = true;
            saved = false;
        } else {
            drawing = frame;
        }

        // Show frame out and preview
        win.update();

        // Save File
        if (cvWaitKey(10) == 's' && found && !saved) {
            filename = std::tmpnam(nullptr);
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
void webCam (vector<Mat> (*process)(Mat)) {
    VideoCapture cap(0);
    if(!cap.open(0)) {
        cout << "Camera failed to open..." << endl;
        return;
    }
    videoProcess(cap,process);
}

void videoFile (char *filepath, vector<Mat> (*process)(Mat)) {
    VideoCapture cap(filepath);
    if (!cap.open(filepath)) {
        std::cout << "!!! Failed to open file: " << avifile << std::endl;
        return;
    }
    videoProcess(cap,process);
}

#endif

// ---------------- Test Methods -----------------
#ifdef TEST
void testGeometry() {
    cout << "running testGeometry" << endl;
    cout << "Creating Variables" << endl;

    // Variable Declarations
    // Points
    const Point a = Point(0, 0), b = Point(100, 100), c = Point(0, 100);
    const Point e = Point(200, 0), d = Point(100, 0), f = Point(100, 200);
    // Poly's
    const cnt tri {a, e, b};  // non-equal sides
    const cnt square {a, c, b, d};
    const cnt rect {a, c*2, f, d};
    const cnt bigSquare {a*2-b, c*2-b, b*2-b, d*2-b};
    // Fp's
    const Fp testFp1 = Fp({bigSquare, square});
    const Fp testFp2 = Fp({bigSquare});  // not a valid Fp
    const Fp testFp3 = Fp({square});  // not a valid Fp

    cout << "Creating Vectors" << endl;

    // Vectors
    // Poly's
    const vector<cnt> vpoly1 {tri, square, bigSquare};
    const vector<cnt> vpoly2 {tri, square};
    const vector<cnt> vpoly3 {square, bigSquare};
    const vector<cnt> vpoly4 {tri, bigSquare};
    const vector<cnt> vpoly5 {tri};
    const vector<cnt> vpoly6 {square};
    const vector<cnt> vpoly7 {bigSquare};
    // Fp's
    const vector<Fp> vfp1 {testFp1, testFp2, testFp3};
    const vector<Fp> vfp2 {testFp1, testFp2};
    const vector<Fp> vfp3 {testFp2, testFp3};
    const vector<Fp> vfp4 {testFp1, testFp3};
    const vector<Fp> vfp5 {testFp1};
    const vector<Fp> vfp6 {testFp2};
    const vector<Fp> vfp7 {testFp3};

    cout << "Test Methods..." << endl;

    // Test Methods
    // Centroid
    Point tric = centroid(tri);
    Point squarec = centroid(square);
    Point bigsc = centroid(bigSquare);
    cout << "Triangle" << tostr(tri) << tostr(tric) << endl;
    cout << "Square" << tostr(square) << tostr(squarec) << endl;
    cout << "Big Square" << tostr(bigSquare) << tostr(bigsc) << endl;
    cout << "Square & BigSquare Centroid: " << tostr(centroid(vpoly3)) << endl;

    // Dist & Angs
    vector<double> dts = dists(tri);
    vector<double> ans = angles(tri);
    cout << "Triangle lengths: " << vtostr(dts) << endl;
    cout << "Triangle angles: " << vtostr(ans) << endl;
    cout << "Unit Angle: " << "Origin - " << tostr(a) << "; Point - " << tostr(b) << "; Ang - " << angle(a,b) << endl;

    // All same length
    cout << "Triangle all same length? " << allSameLength(tri, (double)0.0) << endl;
    cout << "Square all same length? " << allSameLength(square, (double)0.0) << endl;

    // isPoly
    bool test1 = isPoly(tri, 3, false, 0, 0);  // True
    bool test2 = isPoly(tri, 3, true, 0, 0);  // False
    bool test3 = isPoly(tri, 4, false, 0, 0);  // False
    bool test4 = isRectangle(rect, false, 0, 0);  // True
    bool test5 = isRectangle(rect, true, 0, 0);  // False
    bool test6 = isSquare(square, 0, 0);  // True
    cout << test1 << test2 << test3 << test4 << test5 << test6 << endl;
}
#endif

// ------------------ Main Method --------------

int main(int argc,char *argv[]) {
#ifdef TEST
    cout << "Running main..." << endl;
    testGeometry();
#endif
    Capture C;
    if (argc == 1) {
        webCam(C.focusPointCorners);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'h') {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'v') {
        videoFile(C.focusPointCorners, argv[3]);
	} else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'i') {
        imageFile(C.focusPointCorners, argv[3]);
    } else {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    }
}
