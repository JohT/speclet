//README file for directory "wave++/demos"

To create the demos type 

make

this will create the executables for the following demos


1) demoWavPack

The mathematical background for the algorithm implemented in demoWavPack.cc
is in the file "documentation/demoDenoising.ps". Essentially,
a thresholding is applied to the noisy wavelet coefficients
with respect to the best wavelet basis. This is done for many
dictionaries of wavelet packets. See the comments in demoWav.cc
for more details. The program will pause (stop();) execution
and will wait for the user to type "enter" 
every time after a new best basis is found and displayed on the screen
(reconstruction errors are also displayed).

2) demoWav

Same as above except we use only the Wavelet transform instead of the
full Wavelet Packet Transform.  This demo is a good example of how to
use the Wavelet Transform and its inverse.

3) demoMP

The mathematical background and the basics of our implementation of
the matching pursuit (MP) algorithm can be found in 
"documentation/mpTechReport.ps". Running demoMP will run both versions
of our implementation of MP and display errors for the reconstruction.
It is easy to see that different errors are to be expected from the two
implementations. The excutable demoMP takes (command-line) arguments 
and outputs a  file, this features should be self explanatory after 
inspecting the file demoMP.cc.





NOTE: The inclusion of the library "ranlib" is only needed
to generate random numbers with Gaussian distribution.



