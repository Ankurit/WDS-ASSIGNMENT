CXXFLAGS = -std=c++11 -O2

OBJ = Subject.o Object.o ReferenceMonitor.o BLPSecure.o

BLPSecure: $(OBJ)
	g++ -o BLPSecure $(OBJ)

Subject.o: Subject.h
Object.o: Object.h
ReferenceMonitor.o: ReferenceMonitor.h
BLPSecure.o:

.PHONY : clean
clean:
	rm -r *.o BLPSecure
