/**
 * geometry.cpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "geometry.hpp"
#define TEST

double dist(Point a, Point b){
#ifdef TEST 
    cout << "Running dist..." << endl;
#endif
    Point diff = a-b;
    double pow1 = pow((double)diff.x,2.0);
    double pow2 = pow((double)diff.y,2.0);
    double out = sqrt(pow1+pow2);
 
    return out;
}

double angle(Point origin, Point a){
#ifdef TEST 
    cout << "Running angle(2)..." << endl;
#endif
    Point v = a - origin;
    double out = atan2((float)v.x,(float)v.y) * 180.0 / PI;

    return out;
}

double angle(Point origin, Point c2, Point c3) {
#ifdef TEST 
    cout << "Running angle(3)..." << endl;
#endif
    Point a1 = origin; Point a2 = c2; Point a3 = origin; Point a4 = c3;
    Point diff1 = a2-a1; Point diff2 = a4-a3;
    double norm1 = norm(diff1); double norm2 = norm(diff2);
    double out = acos(diff1.dot(diff2)/(norm1*norm2)) * 180.0 / PI;

    return out;
}

vector<double> angles(cnt poly) {
#ifdef TEST 
    cout << "Running angles..." << endl;
#endif
    unsigned int a = 0; unsigned int b = poly.size()-1; unsigned int c = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(angle(poly[a],poly[b],poly[c]));
        a++;b++;c++;
        if (c==poly.size()) {c=0;}
        if (b==poly.size()) {b=0;}
    }

    return out;
}

vector<double> dists(cnt poly) {
#ifdef TEST 
    cout << "Running dists..." << endl;
#endif
    unsigned int a = 0; unsigned int b = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(dist(poly[a],poly[b]));
        a++; b++;
        if (b==poly.size()) {b=0;}
    }

    return out;
}

Point centroid(vector<Point> c) {
#ifdef TEST 
    cout << "Running Centroid..." << endl;
#endif
    Point sum = Point(0,0);

    for (Point p : c) {
    	sum += p;
    }

    int s = c.size();
    Point out;
    out.x = sum.x / s;
    out.y = sum.y / s;

    return out;
}

Point centroid(vector<cnt> vec) {
#ifdef TEST 
    cout << "Running Centroid..." << endl;
#endif
    cnt temp; Point cent;
    for (cnt c : vec) {
        cent = centroid(c);
        temp.push_back(cent);
    }
    Point out = centroid(temp);
    
    return out;
}

Point centroid(Cnts c){return centroid(c.contours);}

bool allSameLength(cnt poly, int distTol){
#ifdef TEST 
    cout << "Running allSameLength" << endl;
#endif
    vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; unsigned int i = 0; int mean = 0;

    //Get a list of all lines in poly
    pairs.push_back({poly[poly.size()-1],poly[0]});                                          //Add the first pair to the list
    for (i = 1; i < poly.size(); i++) {pairs.push_back({poly[i-1],poly[i]});}                //Add the rest
    for (i = 0; i < poly.size(); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));}    //Get a list of the length of all pairs in poly

    //Calculate Mean
    for (i = 0; i<lengths.size(); i++) {mean += lengths[i];}
    mean = mean / lengths.size();

    //Get error from mean and test if it is within tolerance
    for (i = 0; i < poly.size(); i++) {error.push_back(abs(lengths[i]-mean));}               //Get the error from the mean of each length
    for (i = 0; i < poly.size(); i++) {test.push_back(error[i] < distTol);}                      //Check if the error is within tolerance
    return find(test.begin(), test.end(), false)!=test.end();                                //Test and return to see if there is a false within the test vector
}

bool isPoly(cnt poly, int size, bool regular, double angleTol, double distTol) {
#ifdef TEST 
    cout << "Running isPoly" << endl;
#endif
    vector<double> angs;
    if (poly.size()==(unsigned int)size && isContourConvex(poly)) {
        if (size == 4) {
            angs = angles(poly);
            for (double a : angs) {if (abs(a-90.0)>angleTol) {return false;}}    //Test that all angles are within tolerance of 90
        }
        if (regular) {return allSameLength(poly, distTol);}
        else {return true;}
    }
    else {return false;}
}
bool isRectangle(cnt poly, bool square, double angleTol, double distTol) {return isPoly(poly,4,square,angleTol,distTol);}
bool isSquare(cnt poly, double angleTol, double distTol) {return isPoly(poly,4,true,angleTol,distTol);}
