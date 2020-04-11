//
//  main.cpp
//  rendomGenerator
//
//  Created by Madhurdeep Singh on 13/02/20.
//  Copyright © 2020 mdsStudio. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string.h>
#include <vector>
#include<array>
#include<cmath>
#include <string>
#include <sstream>

# define MIN_WAIT_TIME 5

using namespace std;

class Point {
public:
    int x;
    int y;
    
    Point() {
        x= 0;
        y =0;
    }
    
    Point(int x , int y) {
        this->x = x;
        this->y = y;
    }
};

class Line {
public:
    vector<Point> points;
    void addPoint(Point p) {
        points.push_back(p);
    }
};

class Street {
public:
    vector<Line> lines;
    
    void addLine(Line line) {
        lines.push_back(line);
    }
};

class RandomGenerator {
public:
    int maxNumberOfStreets = 10;
    int maxNumberOfLines = 5;
    int maxWaitTime = 20;
    int maxRandomNumberRange = 20;
    
    int numOfStreets;
    Street routeObject;
    vector<string> streetsArray;
    
    
    void generateRandomStreets();
    void removeRandomStreets();
    
    RandomGenerator(int maxNumberOfStreets ,int maxNumberOfLines , int maxWaitTime, int maxRandomNumberRange) {
        this->maxWaitTime = maxWaitTime;
        this->maxNumberOfLines = maxNumberOfLines;
        this->maxNumberOfStreets = maxNumberOfStreets;
        this->maxRandomNumberRange = maxRandomNumberRange;
    }
};

bool streetIntersectionCheck(Street routeObject);
bool lineIntersectionCheck(Line line);
bool intersect(Point p1,Point p2,Point p3,Point p4);
void formatOutput(Street routeObject, string command, int numOfStreets,vector<string> streetsArray);

void RandomGenerator::removeRandomStreets() {
    
    if (routeObject.lines.size() != 0) {
        formatOutput(routeObject,"r",numOfStreets,streetsArray);
        routeObject.lines.clear();
        streetsArray.clear();
    }
}

void showError(string message) {
    cerr << "Error: "<<message<<endl;
}

void RandomGenerator::generateRandomStreets(){
    
    
    string streetFirstName[10] = {"Weber ","King ","Conestoga ","Columbia ","Albert ","Devenport ","Philip ","Erb ","Victoria ","Lester "};
    
    string streetLastName[4] = {"Road ","Blvd ","Street ","Ave "};
    
    string streetDirection[4] = {"E","W","S","N"};
    
    int firstNameCount = 10 , lastNameCount = 4 , directionCount = 4;
    std::ifstream urandom("/dev/urandom");
    
    if (urandom.fail()) {
        std::cerr <<"Error: cannot open /dev/urandom"<<endl;
    }
    
    unsigned int num = 40;
    urandom.read((char*)&num, sizeof(int));
    numOfStreets = num % (maxNumberOfStreets-1) + 2;
    //    cout<<numOfStreets<<" Streets"<<endl<<endl;
    
    vector<string> pointsArray;
    vector<string> commandArray;
    
    
    for(int i= 0;i<numOfStreets;i++) {
        unsigned int num = 20;
        
        bool isDuplicate = false;
        urandom.read((char*)&num, sizeof(int));
        int index1 = num % (firstNameCount-1);
        urandom.read((char*)&num, sizeof(int));
        int index2 = num % (lastNameCount-1);
        urandom.read((char*)&num, sizeof(int));
        int index3 = num % (directionCount-1);
        
        string street = streetFirstName[index1] + streetLastName[index2] + streetDirection[index3];
        for (int index = 0; index < streetsArray.size(); index ++) {
            if (street == streetsArray[index]) {
                isDuplicate = true;
            }
        }
        if(!isDuplicate) {
            streetsArray.push_back(street);
        }else {
            i--;
        }
    }
    int streetCount = 0;
    do {
        
        if (streetCount >= 25 ) {
            showError("random generation overflow : retry with different switch values");
            exit(0);
        }
        int lineCount = 0;
        for(int i= 0;i<numOfStreets;i++) {
            
            if (lineCount >= 25 ) {
                showError("random generation overflow : retry with different switch values");
                exit(0);
            }
            
            Line lineObject;
            bool isDuplicate = false;
            
            unsigned int num = 20;
            urandom.read((char*)&num, sizeof(int));
            int numOflines = num % (maxNumberOfLines) + 1;
            int pointCount = 0;
            for(int j=0; j<numOflines+1; j++) {
                int num_neg = 50;
                urandom.read((char*)&num_neg, sizeof(int));
                int xcoord = num_neg % (maxRandomNumberRange);
                urandom.read((char*)&num_neg, sizeof(int));
                int ycoord = num_neg % (maxRandomNumberRange);
                
                for(int index = 0; index < lineObject.points.size();index ++) {
                    if (xcoord == lineObject.points[index].x && ycoord == lineObject.points[index].y) {
                        isDuplicate = true;
                    }
                }
                if(!isDuplicate) {
                    Point p(xcoord,ycoord);
                    lineObject.addPoint(p);
                }else {
                    pointCount++;
                    if(pointCount >= 10) {
                        showError("random generation overflow : retry with different switch values");
                        exit(0);
                    }
                    j--;
                }
            }
            //            cout<<"line gernated: "<<i<<endl;
            //
            //            for(int j =0;j<lineObject.points.size();j++) {
            //
            //                cout<<"("<<lineObject.points[j].x<<","<<lineObject.points[j].y<<")"<<" ";
            //            }
            //            cout<<"------------------"<<endl;
            
            if (!lineIntersectionCheck(lineObject)) {
                routeObject.addLine(lineObject);
            } else {
                i--;
                lineCount ++;
            }
        }
        if (!streetIntersectionCheck(routeObject)) {
            routeObject.lines.clear();
            streetCount ++;
        } else {
            break;
        }
    } while(!streetIntersectionCheck(routeObject) || (streetCount <= 25));
    
    //    for(int i= 0;i<numOfStreets;i++) {
    //        cout<<"a \""<<streetsArray[i]<<"\" ";
    //        for(int j =0;j<routeObject.lines[i].points.size();j++) {
    //            cout<<"("<<routeObject.lines[i].points[j].x<<","<<routeObject.lines[i].points[j].y<<")"<<" ";
    //        }
    //        cout<<endl;
    //    }
    
    formatOutput(routeObject,"a",numOfStreets,streetsArray);
}

bool isInBetween(Point p1,Point p2,float xcoor,float ycoor) {
    float distance1 = sqrt(pow((p1.x - xcoor),2) + pow((p1.y - ycoor),2));
    float distance2 = sqrt(pow((xcoor - p2.x), 2) + pow((ycoor - p2.y),2));
    float distance3 = sqrt(pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2));
    float a = distance1 + distance2;
    float b = distance3;
    double rel_tol=1e-09;
    double abs_tol=0.0;
    //    bool temp = abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol);
    //    cout<<temp<<endl;
    
    return abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol);
}

bool streetIntersectionCheck(Street routeObject) {
    
    bool isIntersecting = false;
    
    for (int i = 0 ; i<routeObject.lines.size();i++) {
        for(int j = i+1;j<routeObject.lines.size();j++) {
            for(int k = 0;k<routeObject.lines[i].points.size()-1;k++) {
                for(int l = 0;l<routeObject.lines[j].points.size()-1;l++) {
                    if(intersect(routeObject.lines[i].points[k], routeObject.lines[i].points[k+1], routeObject.lines[j].points[l], routeObject.lines[j].points[l+1])) {
                        isIntersecting = true;
                        //                        break;
                        return true;
                    }
                }
            }
        }
    }
    
    //    if (isIntersecting) {
    //        return true;
    //    } else {
    return false;
    //    }
}

bool intersect(Point p1,Point p2,Point p3,Point p4) {
    
    int x1 = p1.x , y1 = p1.y;
    int x2 = p2.x , y2 = p2.y;
    int x3 = p3.x , y3 = p3.y;
    int x4 = p4.x , y4 = p4.y;
    
    float xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    float xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    float xcoor = xnum / xden;
    
    float ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    float yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float ycoor = ynum / yden;
    
    if (xcoor == x2 && ycoor == y2) {
        return false;
    }
    //    cout<<"intesectionPoint :"<<xcoor<<","<<ycoor<<endl;
    
    if (isInBetween(p1,p2,xcoor,ycoor) && isInBetween(p3,p4,xcoor,ycoor)) {
        return true;
    } else {
        return false;
    }
}

bool lineIntersectionCheck(Line line) {
    
    for (int i = 0 ;i < line.points.size()-1;i++) {
        //        cout<<"index i: "<<i<<endl;
        for (int j = i+2; j<line.points.size()-1;j++) {
            //            cout<<"index j: "<<j<<endl;
            if(intersect(line.points[i],line.points[i+1],line.points[j],line.points[j+1])) {
                return true;
            }
        }
    }
    return false;
}

void formatOutput(Street routeObject, string command, int numOfStreets,vector<string> streetsArray) {
    
    string outputString = "";
    
    for(int i= 0;i<numOfStreets;i++) {
        outputString += command + " \"" + streetsArray[i] +"\" ";
        //        cout<<"a \""<<streetsArray[i]<<"\" ";
        if (command == "a") {
            for(int j =0;j<routeObject.lines[i].points.size();j++) {
                outputString += "(" + to_string(routeObject.lines[i].points[j].x) + "," + to_string(routeObject.lines[i].points[j].y) + ")" + " ";
                //            cout<<"("<<routeObject.lines[i].points[j].x<<","<<routeObject.lines[i].points[j].y<<")"<<" ";
            }
        }
        outputString += "\n";
        //        cout<<endl;
    }
    if (command == "a") {
        outputString += "g";
    }
    cout<<outputString<<endl;
}

int main(int argv, char ** argc) {
    
    Street routeObject;
    unsigned int maxNumberOfStreets = 10, maxNumberOfLines = 5,maxWaitTime = 5 ,maxRandomNumberRange = 20;
    std::string sting_s, sting_n, sting_l, sting_c;
    int argument;
    
    opterr = 0;
    
    while ((argument = getopt(argv,argc, "s:n:l:c:")) != -1) {
        switch (argument) {
                
            case 's':
                sting_s = optarg;
                maxNumberOfStreets = std::atoi(sting_s.c_str());
                if (maxNumberOfStreets < 2){
                    showError("Number of streets should be greater than 1");
                    exit(1);
                }
                break;
                
            case 'n':
                sting_n = optarg;
                maxNumberOfLines = std::atoi(sting_n.c_str());
                if (maxNumberOfLines < 1){
                    showError("Number of lines in each street should be atleast 1");
                    exit(1);
                }
                break;
                
            case 'l':
                sting_l = optarg;
                maxWaitTime = std::atoi(sting_l.c_str());
                if (maxWaitTime < 5){
                    showError("Wait time should be atleast 5 seconds");
                    exit(1);
                }
                break;
                
            case 'c':
                sting_c = optarg;
                maxRandomNumberRange = std::atoi(sting_c.c_str());
                if (maxRandomNumberRange < 1){
                    showError("Range of coordi=nates should be grater than or equal to |1|");
                    exit(1);
                }
                break;
                
            case '?':
                showError("invalid switch :" + to_string(optopt));
                exit(1);
                break;
        }
    }
    while (true) {
        RandomGenerator randomGeneratorObject(maxNumberOfStreets,maxNumberOfLines,maxWaitTime,maxRandomNumberRange);
        randomGeneratorObject.generateRandomStreets();
        usleep(maxWaitTime * 1000000);
        randomGeneratorObject.removeRandomStreets();
        //        if (counter > 25) {
        //            showError("random generation overflow : retry with different switch values");
        //            exit(1);
        //        }
    }
    return 0;
}


