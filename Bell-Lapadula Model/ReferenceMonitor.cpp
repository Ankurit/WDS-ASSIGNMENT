#include "ReferenceMonitor.h"

// Add subject name and associated security level to a map and also store subject information
void ReferenceMonitor::addSubject(std::string name, int level) {
    counter++;
    subLevels.insert(std::pair <std::string, int> (name, level));
    Subject newSub(name, 0);
    subjects.push_back(newSub);
}

// Add object name and associated security level to a map and also store object information
void ReferenceMonitor::addObject(std::string name, int level) {
    counter++;
    objLevels.insert(std::pair <std::string, int> (name, level));
    Object newObj(name, 0);
    objects.push_back(newObj);
}

// Perform a read if subject is allowed to access it.
bool ReferenceMonitor::executeRead(std::string name, std::string file) {
    counter++;
    int subjLoc = -1;
    int objLoc = -1;
    if(getLevelSubj(name) >= getLevelObj(file)) {
        int i, j;
        // Find subject in our database
        for(i = 0; i < subjects.size(); i++) {
            if(subjects[i].getName() == name) {
                subjLoc = i;
                break;
            }
        }
        // Find object in our database
        for(i = 0; i < objects.size(); i++) {
            if(objects[i].getName() == file) {
                objLoc = i;
                break;
            }
        }

        // Return success if subject and object exist and set the subject's temp value to the object's value
        if(subjLoc != -1 && objLoc != -1) {
            subjects[subjLoc].setTemp(objects[objLoc].getValue());
            return true;
        }

        // Return false if either subject or object were not found
        return false;
    }

    return false;
}

// Perform a write if subject is allowed to access it.
bool ReferenceMonitor::executeWrite(std::string name, std::string file, int v) {
    counter++;
    int objLoc = -1;
    int i;
    if(getLevelSubj(name) <= getLevelObj(file)) {
        // Find object in our database
        for(i = 0; i < objects.size(); i++) {
            if(objects[i].getName() == file) {
                objLoc = i;
                break;
            }
        }

        // Return success if subject and object exist and set the object's value to the requested write value
        if(objLoc != -1) {
            objects[objLoc].setValue(v);
            return true;
        }

        // Return false if either subject or object were not found
        return false;
    }
    return false;
}

// Returns the security level of a subject
int ReferenceMonitor::getLevelSubj(std::string key) {
    return subLevels.find(key)->second;
}

// Returns the security level of an object
int ReferenceMonitor::getLevelObj(std::string key) {
    return objLevels.find(key)->second;
}

// Returns how many actions we've performed, every 10 we will call printState() from BLPSecure.cpp
int ReferenceMonitor::getCounter() {
    return counter;
}

// Prints out a table of all information about every subject and object
void ReferenceMonitor::printState() {
    int i;
    std::cout << "+--------------------------------+\n[Current States]\n";
    std::cout << "\t<Subjects>\n";
    for(i = 0; i < subjects.size(); i++) {
        std::cout << "\t\t" << subjects[i].getName() << " <> temp: " << subjects[i].getTemp() << std::endl;
    }
    std::cout << "\t<Objects>\n";
    for(i = 0; i < objects.size(); i++) {
        std::cout << "\t\t" << objects[i].getName() << " <> value: " << objects[i].getValue() << std::endl;
    }
    std::cout << "+--------------------------------+\n";
    counter = 0;
}
