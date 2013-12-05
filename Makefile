LDLIBS := -lfftw3 -lm
EXECUTABLE := fftw_test

all: $(EXECUTABLE)

fftw_test: fftw_test.o
	g++ fftw_test.o -o $(EXECUTABLE) $(LDLIBS)

fftw_test.o: fftw_test.cpp
	g++ -c fftw_test.cpp $(LDLIBS)

clean:
	rm -rf *o $(EXECUTABLE)
