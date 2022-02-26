//README file for release of Wave++ Library. January 2,000.

Welcome to Release 1.0 of Wave++ !!!

1) After having executed the commands for unpacking

gunzip nameOfFile.tar.gz

tar xvf   nameOfFile.tar

The directory wave++ should then have been created. This directory contains
the following subdirectories:

1a) source
1b) demos
1c) documentation

To get a quick idea if Wave++ may be useful for you we suggest
to read the short PostScript file "documentation/wave++.ps".

Each directory contains a README file that you should read
for instructions and information. To compile and run the 
code in "demos" you  need first to create the library. This is 
done following the instructions in the "source" directory.

2) The execution of the Makefile in "source" will create a
library archive in that same directory. Path dependencies 
in the directory "demos" point to the "source" location 
of the library archive. That is to say, our distribution
creates files locally with respect to the wave++ directory.
The user can move the relevant library file and header file to more
standard directories if he/she wishes.


3) The directory "documentation" contains PostScript files
explaining features of the library and some background to understand
the demos. 



All program code and documentation in this directory is
copyright (c) 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic

Permission is granted for anyone to copy, use, or modify these
programs and documents for purposes of research or education,
provided this copyright notice is retained, and note is made of
any changes that have been made.  Use for commercial applications is only
allowed by permission of the copyright holders.

These programs and documents are distributed without any warranty,
express or implied.  As the programs were written for research
purposes only, they have not been tested to the degree that would
be advisable in any important application.  All use of these
programs is entirely at the user's own risk. 
