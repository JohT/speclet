//file: demoWavPack.cc
//Shows some of the functionality of the Wavelet Library, namely:
//we denoise a noisy signal using the nonlinear thresholding algorithm
//of Donoho and Johnstone (D&J). This is done in the context
//of the Best Wavelet Basis algorithm with the library of Wavelet
//Packets. Actually, we do this best wavelet basis denoising 17
//times, one for each library which in turn are especified by
//a pair of wavelet filters
//math. background for algo. is in /documentation/demoDenoising.ps
//Remark: The function stop() waits for keyboard "enter"  
//to resume execution
#include "demoTools.h"
#include "../source/libw.h"
#include "./ranlib/ranlib.h"
#include "input_maker.h"
#include "rms_error.h"

int MAXLEVEL= 10;
int DIM = (1<<MAXLEVEL);//dimensionality of vector ("signal")
real_number  Factor = (1.0+sqrt(2*log(MAXLEVEL*DIM)));//D&J Best Wavelet (BWB)
//thresholding parameter (modified)

#define  SNR  5.0//Signal to Noise Ratio

int main()
{
  
  
  //set random number generator for noise (uses ranlib) 
  long iseed1 =333;
  long iseed2 = (long) (.987654321 * (double)iseed1);
  setall(iseed1,(long) iseed2);
  
  int i, k;
  real_number Noise_level = sqrt(1.0/(SNR*DIM));
  
  //variance of the Gaussian noise added
  real_number variance = Noise_level*Noise_level;
  
  
  int iterations = 17;//as many as wavelet filters
  

  real_number bwbErrors[iterations]; //array of bwb errors found
  
  Interval arrayOfIntervals[iterations];
  
  
  
  
  //create arrays to hold wavelets' specifications
  PQMF *G = new PQMF [17];
  PQMF *H = new PQMF [17];
  
  G[0].Set(d02doqf, 0, 1);    H[0].Set(d02soqf, 0, 1);
  G[1].Set(d04doqf, 0, 3);    H[1].Set(d04soqf, 0, 3);
  G[2].Set(d06doqf, 0, 5);    H[2].Set(d06soqf, 0, 5);
  G[3].Set(d08doqf, 0, 7);    H[3].Set(d08soqf, 0, 7);
  G[4].Set(d10doqf, 0, 9);    H[4].Set(d10soqf, 0, 9);
  G[5].Set(d12doqf, 0, 11);   H[5].Set(d12soqf, 0, 11);
  G[6].Set(d14doqf, 0, 13);   H[6].Set(d14soqf, 0, 13);
  G[7].Set(d16doqf, 0, 15);   H[7].Set(d16soqf, 0, 15);
  G[8].Set(d18doqf, 0, 17);   H[8].Set(d18soqf, 0, 17);
  G[9].Set(d20doqf, 0, 19);   H[9].Set(d20soqf, 0, 19);
  G[10].Set(c06doqf, 0, 5);   H[10].Set(c06soqf, 0, 5);
  G[11].Set(c12doqf, 0, 11);  H[11].Set(c12soqf, 0, 11);
  G[12].Set(c18doqf, 0, 17);  H[12].Set(c18soqf, 0, 17);
  G[13].Set(c24doqf, 0, 23);  H[13].Set(c24soqf, 0, 23);
  G[14].Set(c30doqf, 0, 29);  H[14].Set(c30soqf, 0, 29);
  G[15].Set(b18doqf, 0, 17);  H[15].Set(b18soqf, 0, 17); 
  G[16].Set(v24doqf, 0, 23);  H[16].Set(v24soqf, 0, 23);
  
  
  
  
  //to hold clean signal
  Interval CleanSignal(0, DIM-1);
  //to hold sampled function plus added noise 
  Interval NoisySignal(0, DIM-1);
  //load sampled function into CleanSignal 
  Input_maker(1, CleanSignal);
  
  
  Interval RecSignal;//present Reconstruction Signal
  
  for(i=0; i<DIM; i++) NoisySignal[i] = CleanSignal[i] + Noise_level*(real)gennor(0,1);
  
  for(k=0; k< iterations; k++)
    {
      
      //analysis
      ArrayTreePer Theta(MAXLEVEL);//Wavelet packet coeffs for CleanSignal 
      ArrayTreePer W(MAXLEVEL);//Wavelet packet coeffs for NoisySignal
      Analysis(NoisySignal, W, H[k], G[k], ConvDecPer);//Wavelet packet analysis for NoisySignal
      Analysis(CleanSignal, Theta, H[k], G[k], ConvDecPer);//Wavelet packet analysis for CleanSignal
    

      ArrayTreePer Temp(MAXLEVEL);//Temporary device to find BB. 
      HedgePer BB; 
      ExtractBestBasis(W, BB, OracCost, Noise_level);//Find the best basis (BB)
      
      
      //at this point the BB for Noisy signal has been found
      //temp hedge for manipulations
      HedgePer BB_temp;
      BB_temp = BB;
      
      //extract coeff. in W relative to BB
      ExtractHedge(BB_temp, W);
      
      Interval Delta(0, DIM-1);//Delta values initialized to 0
      for(i=0; i<DIM; i++)//Threshold values a la Donoho
	{
	  real_number x = BB_temp.origin[i]*BB_temp.origin[i];
	  if (x > variance*Factor*Factor) Delta[i]=1;
	}
      for(i=0; i<DIM; i++)  BB_temp.origin[i] *= Delta[i];
 
      SuperposeHedge(BB_temp,Temp);
      
      //Inverse wavelet transform applied to thresholded coeff.
      Synthesis(Temp, RecSignal, H[k], G[k], AdjConvDecPer);
      arrayOfIntervals[k] = RecSignal;
      
      real_number BB_rms = RMS_error(CleanSignal,RecSignal);
      bwbErrors[k] = BB_rms;
      
      cout <<"The levels for BB are: "<<endl;
      for(i=0; i<BB.num_of_levels; i++) cout <<BB.levels[i]<<"\t";cout<<endl;
      stop();
      cout <<"The Relative Mean Square Error for estimate k= " << k <<endl;
      cout <<bwbErrors[k]<< endl;
      stop();
      
}//end of iterations 
  stop();
  
  delete [] G; delete [] H;
  return 0;
}

//void stop(){
//  while(getchar()==0) { 
//   getchar();}
//}


















