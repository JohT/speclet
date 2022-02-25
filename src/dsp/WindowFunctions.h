/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
	------------------------------------------------------------------------------
  Most of the window function implementations in this class had been taken out of the fft c program 
  from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
  ==============================================================================
*/
#pragma once
#include <map>
using namespace std;

class WindowFunction;

//------------------------------------------------------------------------------------//
// Factory                                                                            //
//------------------------------------------------------------------------------------//
class WindowFunctionsFactory {
public:
    static WindowFunctionsFactory *getSingletonInstance();
    WindowFunction *createWindowFunction(int windowFunctionNr, long resolution);
    void destruct();

private:
    static WindowFunctionsFactory *pSingletonInstance;

    WindowFunctionsFactory(void) {}
    ~WindowFunctionsFactory() {}
    WindowFunctionsFactory(const WindowFunctionsFactory &);

    typedef map<long, WindowFunction *> TResolutionsMap;
    typedef map<long, WindowFunction *>::const_iterator TResolutionsMapIterator;
    typedef pair<long, WindowFunction *> TResolutionsKeyValue;
    typedef map<int, TResolutionsMap> TWindowFunctionsMap;
    typedef map<int, TResolutionsMap>::const_iterator TWindowFunctionsMapIterator;
    typedef pair<int, TResolutionsMap> TWindowFunctionsKeyValue;

    TWindowFunctionsMap windowingFunctions;

    WindowFunction *readWindowFunctionFromMap(int windowFunctionNr, long resolution);
    void writeWindowFunctionIntoMap(int windowFunctionNr, long resolution, WindowFunction *pWindowFunction);
    void deleteWindowFunctionsMap();
};


//------------------------------------------------------------------------------------//
// Interface as abstract class                                                        //
//------------------------------------------------------------------------------------//
class WindowFunction {
public:
    explicit WindowFunction(long resolution);
    ~WindowFunction();

    auto getFactor(int index) -> double;
    auto getWindow() -> double *;
    virtual auto getName() -> char * = 0;

protected:
    virtual auto calculateFactor(int index) -> double = 0;//will be redefined by the derived windowing function

    long mResolution{};//also known as N or FFT-size or length of signal piece array
    double *windowFunctionFactors{};

private:
    WindowFunction() = default;                      //explicit no default contructor;
    WindowFunction(const WindowFunction &) = default;//explicit no copy contructor;
};


//------------------------------------------------------------------------------------//
// Interface implementations                                                          //
//------------------------------------------------------------------------------------//
class WindowBartlett : WindowFunction {
public:
    WindowBartlett(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//bartlett windowing function
    char *getName(void) { return "Barlett"; };
};

class WindowBlackman : WindowFunction {
public:
    WindowBlackman(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//blackman windowing function
    char *getName(void) { return "Blackman"; };
};

class WindowBlackmanHarris : WindowFunction {
public:
    WindowBlackmanHarris(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//blackman_harris windowing function
    char *getName(void) { return "Blackman Harris"; };
};

class WindowHamming : WindowFunction {
public:
    WindowHamming(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//hamming windowing function
    char *getName() { return "Hamming"; };
};

class WindowHanning : WindowFunction {
public:
    WindowHanning(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//hanning windowing function
    char *getName(void) { return "Hanning"; };
};

class WindowParzen : WindowFunction {
public:
    WindowParzen(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//parzen windowing function
    char *getName(void) { return "Parzen"; };
};

class WindowSquare : WindowFunction {
public:
    WindowSquare(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//square windowing function
    char *getName(void) { return "Square"; };
};

class WindowWelch : WindowFunction {
public:
    WindowWelch(long resolution) : WindowFunction(resolution){};
    double calculateFactor(int index);//welch windowing function
    char *getName(void) { return "Welch"; };
};