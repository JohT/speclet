// input_maker.h

#include "../source/libw.h"

#ifndef INPUTMAKER_H
#define INPUTMAKER_H

void Input_maker(int type, Interval &In);        // function for making 6 different
                                                                   // clean signals
                                              // type is an integer between 1 and 6
                                // on input In is predefined all 0 interval so that 
                                         // In.length = desired length of the signal
                                          // on output In is filled up with a signal

int signum(real_number x);    // utility

const real_number Pi = 3.141592654;

real_number funct(real_number x);   // utility

#endif



















