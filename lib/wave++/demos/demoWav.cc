//file: demoWav.cc
//Shows some of the functionality of the Wavelet Library, namely:
//we denoise a noisy signal using the nonlinear thresholding algorithm
//of Donoho and Johnstone (D&J). This is done in the context
//of a single wavelet basis (contrast this with  what is 
//done in demoWavPack.cc)
//math. background for algo. is in /documentation/demoDenoising.ps
//Remark: The function stop() waits for keyboard "enter"  
//to resume execution
#include "demoTools.h"
#include "./ranlib/ranlib.h"
#include "../source/libw.h"
#include "input_maker.h"
#include "rms_error.h"


int MAXLEVEL= 10;
int DIM = (1<<MAXLEVEL);//dimensionality of vector ("signal")
real  Factor = sqrt(2*log(DIM));//D&J thresholding parameter

#define  SNR  5.0//Signal to Noise Ratio

int main()
{
  
  
  //set random number generator for noise (uses ranlib) 
  long iseed1 =333;
  long iseed2 = (long) (.987654321 * (double)iseed1);
  setall(iseed1,(long) iseed2);
  
  int i, k;
  real Noise_level = sqrt(1.0/(SNR*DIM));
  
  //variance of the Gaussian noise added
  real variance = Noise_level*Noise_level;
  
  
  
  //to hold clean signal
  Interval CleanSignal(0, DIM-1);
  //to hold sampled function plus added noise 
  Interval NoisySignal(0, DIM-1);
  //load sampled function into CleanSignal 
  Input_maker(1, CleanSignal);
  
  
  Interval RecSignal(0, DIM-1);//present Reconstruction Signal
  
  for(i=0; i<DIM; i++) NoisySignal[i] = CleanSignal[i] + Noise_level*(real)gennor(0,1);
  
  // Initialize wavelets
  PQMF H(d10soqf, 0, 9); // Daubechies 10
  PQMF G(d10doqf, 0, 9); 
  Interval Out(0, DIM-1);
 

  WaveTrans(NoisySignal, Out, H, G, ConvDecPer);

  
  Interval Delta(0, DIM-1);//Delta values initialized to 0
  for(i=0; i<DIM; i++)//Threshold values a la Donoho
    {
      real x = Out[i]*Out[i];
      if (x > variance*Factor*Factor) Delta[i]=1;
    }
  for(i=0; i<DIM; i++)  Out[i] *= Delta[i];
  
  

      //Inverse wavelet transform applied to thresholded coeff.
  InvWaveTrans(Out, RecSignal, H, G, AdjConvDecPer); 

  real rms = RMS_error(CleanSignal,RecSignal);
      
  
  
  cout <<"The Relative Mean Square Error for estimate " <<endl;
  cout << rms << endl;
  stop();
  
  return 0;
}

//void stop(){
//  while(getchar()==0) { 
//    getchar();}
//}






