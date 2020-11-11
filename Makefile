CXXC = g++
CFLAGS = -w -O3 

all: collText collReg svm.o documentCollection.o uciType.o uciReg.o helperRoutines.o Sparse.o parseCommandLine.o MarianaPredict


parseCommandLine.o: parseCommandLine.cpp parseCommandLine.h
	$(CXXC) $(CFLAGS) -c parseCommandLine.cpp
svm.o: svm.cpp svm.h
	$(CXXC) $(CFLAGS) -c svm.cpp
Sparse.o: Sparse.cpp Sparse.h
	$(CXXC) $(CFLAGS) -c Sparse.cpp
uciType.o: uciType.cpp uciType.h
	$(CXXC) $(CFLAGS) -c uciType.cpp
uciReg.o: uciReg.cpp uciReg.h
	$(CXXC) $(CFLAGS) -c uciReg.cpp
helperRoutines.o:helperRoutines.cpp helperRoutines.h
	$(CXXC) $(CFLAGS) -c helperRoutines.cpp
documentCollection.o: documentCollection.cpp documentCollection.h 
	$(CXXC) $(CFLAGS) -c documentCollection.cpp 
MarianaPredict: MarianaPredict.cpp
	$(CXXC)  $(CFLAGS) MarianaPredict.cpp -o marianaPredict
collText: collText.cpp Sparse.o parseCommandLine.o documentCollection.o \
		helperRoutines.o svm.o uciType.o 
	$(CXXC) $(CFLAGS) collText.cpp -o mariana parseCommandLine.o Sparse.o \
		uciType.o svm.o helperRoutines.o documentCollection.o -lm 
collReg: collReg.cpp Sparse.o parseCommandLine.o documentCollection.o \
		helperRoutines.o svm.o uciReg.o 
	$(CXXC) $(CFLAGS) collReg.cpp -o marianaR parseCommandLine.o Sparse.o \
		uciReg.o svm.o helperRoutines.o documentCollection.o -lm 
clean:
	rm -f *~ mariana marianaR marianaPredict svm.o parseCommandLine.o \
		documentCollection.o mariana Sparse.o helperRoutines.o uciType.o uciReg.o 
