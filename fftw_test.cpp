/*
 * =====================================================================================
 *
 *       Filename:  fftw_test.cpp
 *
 *    Description:  Learning FFTW 
 *
 *        Version:  1.0
 *        Created:  13-12-05 10:28:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  Texas A&M University 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fftw3.h>
#include <vector>

int real2complex() {
	std::cout << "\nHello from real2complex()" << std::endl;
	
	//Here we run the DFT and return complex numbers.

	int i; 
	int i_inputArraySize = 16;
	int i_outputArraySize = i_inputArraySize;

	fftw_plan plan; //FFTW data structure

	double *pd_inputArray;  //The input is with real numbers
	fftw_complex *pfc_outArray;   //The output is complex numbers

	pd_inputArray = (double*)malloc(sizeof(double) * i_inputArraySize);   //Allocate memory for the input.
	pfc_outArray = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * i_outputArraySize);  //Allocate memory for the output using fftw's malloc  

	std::cout << "\nThis takes in a real number array and returns a complex number array:" << std::endl;
	plan = fftw_plan_dft_r2c_1d(i_inputArraySize, pd_inputArray, pfc_outArray, FFTW_ESTIMATE); //Set up the plan for the real 2 complex DFT.

	//Set up a test array.
	for (i = 0; i < i_inputArraySize; i += 4) {
		pd_inputArray[i] = 0.0;
		pd_inputArray[i+1] = 0.0;
		pd_inputArray[i+2] = 1.0;
		pd_inputArray[i+3] = 1.0;
	}

	//Perform the DFT.
	fftw_execute(plan);

	//Input array remains unchanged.
	std::cout << "Input array: " << std::endl;
	for (i = 0; i < i_inputArraySize; i++) {
		std::cout << " " << pd_inputArray[i];
	}
	std::cout << std::endl;
	
	//Output array now has the DFT of the input array.
	std::cout << "Output array: " << std::endl;
	for (i = 0; i < i_outputArraySize; i++) {
		std::cout << " (" << pfc_outArray[i][0] << "," << pfc_outArray[i][1] << ")";
	}
	std::cout << std::endl;

	//Cleanup
	fftw_destroy_plan(plan);
	fftw_free(pfc_outArray);
	free(pd_inputArray);

	return EXIT_SUCCESS;
}

int real2real() {
	std::cout << "\nHello from real2real()" << std::endl;

	//Here we run the DFT and return the "halfcomplex" format output.
	//It contains the non-redundant half of the complex output (see the FFTW documentation for its explanation)
	
	int i;
	int i_inputArraySize = 16;
	int i_outputArraySize = i_inputArraySize;

	fftw_plan plan;  //FFTW data structure

	double *pd_inputArray;  //The input is in real numbers
	double *pd_outArray;   //The output is also as real numbers.

	pd_inputArray = (double*)malloc(sizeof(double) * i_inputArraySize);   //Allocate memory for the input.
	pd_outArray = (double*)malloc(sizeof(double) * i_outputArraySize);   //Allocate memory for the output.
	
	std::cout << "\nThis takes in a real number array and returns a real number array:" << std::endl;
	plan = fftw_plan_r2r_1d(i_inputArraySize, pd_inputArray, pd_outArray, FFTW_R2HC, FFTW_ESTIMATE);  //Set up the plan for the real 2 real DFT.

	//Set up the test array.
	for (i = 0; i < i_inputArraySize; i += 4) {
		pd_inputArray[i] = 0.0;
		pd_inputArray[i+1] = 0.0;
		pd_inputArray[i+2] = 1.0;
		pd_inputArray[i+3] = 1.0;
	}
	//Since we're going to calculate the power spectrum, we'll use a sample spacing to calculate frequency.
	double d_sampleSpacing = 1; //Inverse of the sampling rate

	//Perform the DFT
	fftw_execute(plan);

	//Input array remains unchanged
	std::cout << "Input array" << std::endl;
	for (i = 0; i < i_inputArraySize; i++) {
		std::cout << " " << pd_inputArray[i];
	}
	std::cout << std::endl;
	
	//Output array now has the DFT in the "halfcomplex" format discussed in the FFTW documentation.
	std::cout << "\"Halfcomplex\" output array: " << std::endl;
	for (i = 0; i < i_outputArraySize; i++) {
		std::cout << " " << pd_outArray[i];
	}
	std::cout << std::endl;

	//Calculating the power spectrum
	std::vector<double> vd_powerSpectrum;
	std::vector<double> vd_frequencies;

	double real;
	double img;
	double real2;
	double img2;
	double pwr;

	//Take the absolute value of the imaginary numbers contained in "halfcomplex" format
	//In the "halfcomplex" format, the real component is element i, and the imaginary component is 
	//the array size - i.
	for (i = 1; i < i_outputArraySize / 2; i++) {
		real = pd_outArray[i];
		img = pd_outArray[i_outputArraySize - i];
		real2 = real * real;
		img2 = img * img;
		pwr = sqrt(real2 + img2);
		vd_powerSpectrum.push_back(pwr);	
	}

	std::cout << "Power spectrum" << std:: endl;
	for (i = 0; i < vd_powerSpectrum.size(); i++) {
		std::cout << " " << vd_powerSpectrum[i];
	}
	std::cout << std::endl;
  
	//Determine the frequencies assuming an even array.
	double frequency;
	for (i = 1; i < i_outputArraySize / 2; i++) {
		frequency = (i / (d_sampleSpacing * i_outputArraySize));
		vd_frequencies.push_back(frequency);
	}

	std::cout << "Frequencies of spectrum " << std::endl;
	for (i = 0; i < vd_frequencies.size(); i++) {
		std::cout << " " << vd_frequencies[i];
	}
	std::cout << std::endl << std::endl;
	
	//Cleanup
	fftw_destroy_plan(plan);
	free(pd_outArray);
	free(pd_inputArray);

	return EXIT_SUCCESS;
}

int main ( int argc, char *argv[] )
{
	std::cout << "Hello from main" << std::endl;
	
	real2complex();  //Runs the DFT and returns complex numbers.
	real2real(); //Runs the DFT and returns the "halfcomplex" format.
	
	return EXIT_SUCCESS;
}
