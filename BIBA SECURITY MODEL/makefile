CXXFLAGS = -std=c++11 -O2

OBJ = Subject.o Object.o ReferenceMonitor.o BIBASecure.o

BLPSecure: $(OBJ)
	g++ -o BIBASecure $(OBJ)

Subject.o: Subject.h
Object.o: Object.h
ReferenceMonitor.o: ReferenceMonitor.h
BIBASecure.o:

.PHONY : clean
clean:
	rm -r *.o BIBASecure
