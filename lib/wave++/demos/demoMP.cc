//file: demoMP.cc
//it will run both implementations of MP up
// a max_iter number of components or quiting
//by epsilon. The functionality of the code
//below should be clear once the MP is understood. 
#include "../source/libw.h"

#include <fstream>
#include "input_maker.h"
#include <vector>

int main( int argc, char* argv[])
{
  if( argc != 2 )
    { 
      std::cout << "Must give parameter m to main. E.g. type: main 7\n" 
	   << "Alternatively, if you want results to go to a file with \n "
	   << "a different suffix, say .temp, then type : main 7.temp \n"
	   << "In this case results will go to file results_for_m=7.temp"
	   << std::endl;
      exit(1);
    }
  
  int m = atoi( argv[1] );
  std::cout << "m = " << m << std::endl;
  int dim = 1<<m;
  real_number epsilon = 0.000000000000000000001;
  int max_iter = 4;  //set to different value if desired
  real_number error, error1;
  char filename[60];
  strcpy( filename, "results");
  strcat(filename, argv[1]);
  std::ofstream outfile( filename, std::ios::out);
  assert(outfile);
  
  Interval I(0, dim-1);
  for(int i=0; i<dim; i++) I[i] = i;
  
  Interval CleanSignal(0, dim-1);
 real_number coef, coef1;
 for( int inp=4; inp < 5; inp++)//may be used to loop over many input
   {                            //functions if desired
     Input_maker(inp, CleanSignal);  
     Interval Rf, RecSignal, RecSignal1, Rf1;
     std::vector <RealGabor> G, G1;
     std::vector <real_number> Gcoef, Gcoef1;
     Interval f_approx(0, dim-1), f_approx1(0, dim-1);
     RealGabor Gtemp, Gtemp1;
     Partition Part(dim, 2.0);
     
     error = RunShiftGaborMP( max_iter, epsilon, CleanSignal, Part,
			      RecSignal, Rf, G, Gcoef);
     std::cout << "error for Shift, a = 2 : " << error << std::endl;
     
     error1 = RunFFTGaborMP(max_iter, epsilon, CleanSignal,
			    RecSignal1,Rf1, G1, Gcoef1);
     std::cout << "error for FFT" << " : " << error1 << std::endl;      
     
     outfile << "********* INPUT_MAKER " << inp << " *********" << std::endl;
     outfile << "number of iterations performed " << G.size() 
	     << "  error = " << error << std::endl;
     for(int z=0; z < G.size(); z++)
       {
	 Gtemp = G[z];
	 coef = Gcoef[z];
	 Gtemp1 = G1[z];
	 coef1 = Gcoef1[z];
	 outfile << "iteration " << z << std::endl;
	 outfile << "\t\tcoeficient: "<< coef <<std::endl;
	 outfile << "\t(" << Gtemp.s << ", " << Gtemp.u << ", "
		 << Gtemp.v << ", " << Gtemp.w << ")" << std::endl;
	 outfile << "\t\tcoeficient: "<< coef1 <<std::endl;
	 outfile << "\t(" << Gtemp1.s << ", " << Gtemp1.u << ", "
		 << Gtemp1.v << ", " << Gtemp1.w << ")" << std::endl;
	 Gtemp.createSample(I);
	 Gtemp1.createSample(I);
	 f_approx += Gtemp.Sample * coef;
	 f_approx1 += Gtemp1.Sample * coef1;
	 
       }  // end of z-loop
     
   }
 outfile << "\n\n\n";   
 return 0;
}




