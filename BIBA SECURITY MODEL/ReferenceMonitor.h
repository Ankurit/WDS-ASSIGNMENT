#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Subject.h"
#include "Object.h"
using namespace std;
#define low 0
#define medium 1
#define high 2


class ReferenceMonitor {
    private:
        // Store all subjects, objects, and their associated security levels
        std::map<std::string, int> subLevels;
        std::map<std::string, int> objLevels;
        std::vector<Subject> subjects;
        std::vector<Object> objects;
        int counter;

    public:
        ReferenceMonitor() {
            counter = 0;
        }
        // Functions for adding subjects and objects of some security level
        void addSubject(string ss, int level);
        void addObject(string s, int level);
        // Executing read/write instructions with command information from BLPSecurity.cpp
        bool executeRead(string, string);
        bool executeWrite(string,string, int);
        // Returns security level of an object or subject
        int getLevelSubj(string);
        int getLevelObj(string);
        // Returns the number of actions performed
        int getCounter();
        // Prints out a table of all information
        void printState();

};
