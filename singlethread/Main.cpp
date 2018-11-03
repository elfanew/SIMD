
//sudo apt install libx11-dev:i386 libx11-dev
//sudo apt install cimg-dev cimg-doc cimg-examples
//g++ Main.cpp -O0 -g3 -Wall -lm -lpthread -lX11

#include <CImg.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
using namespace cimg_library;


/**********************************
 * TODO
 * 	- Change the data type returned by CImg.srcImage to adjust the
 * 	requirements of your workgroup
 * 	- Change the data type of the components pointers to adjust the
 * 	requirements of your workgroup
 */

int main() {

	CImg<float> srcImageX("bailarina.bmp"); // Open file and object initialization
	CImg<float> srcImageY("figura.bmp");

	float *pRcompX, *pGcompX, *pBcompX, *pRcompY, *pGcompY, *pBcompY; // Pointers to the R, G and B components
	float *pRnew, *pGnew, *pBnew;
	float *pdstImage; // Pointer to the new image pixels
	int widthX, heightX, widthY, heightY; // Width and height of the image
	int nCompX, nCompY; // Number of image components


	/***************************************************
	 *
	 * Variables initialization.
	 * Preparation of the necessary elements for the algorithm
	 * Out of the benchmark time
	 *
	 */

	srcImageX.display(); // If needed, show the source image
	widthX = srcImageX.width(); // Getting information from the source image
	heightX  = srcImageX.height();

	// source image number of components
	// Common values for spectrum (number of image components):
		//  B&W images = 1
		//	Normal color images = 3 (RGB)
		//  Special color images = 4 (RGB and alpha/transparency channel)
	nCompX = srcImageX.spectrum(); 
	
	srcImageY.display();
	widthY = srcImageY.width();
	heightY = srcImageY.height();
	nCompY = srcImageY.spectrum();


	// Allocate memory space for the pixels of the destination (processed) image 
	pdstImage = (float *) malloc (widthX * heightX * nCompX * sizeof(float));
	
	if (pdstImage == NULL) {
		printf("\nMemory allocating error\n");
		exit(-2);
	}

	// Pointers to the RGB arrays of the sourceX image
	pRcompX = srcImageX.data(); 		  // pRcomp points to the R component
	pGcompX = pRcompX + heightX * widthX; // pGcomp points to the G component
	pBcompX = pGcompX + heightX * widthX; // pBcomp points to B component

	// Pointers to the RGB arrays of the sourceY image
	pRcompY = srcImageY.data(); 		  // pRcomp points to the R component
	pGcompY = pRcompY + heightY * widthY; // pGcomp points to the G component
	pBcompY = pGcompY + heightY * widthY; // pBcomp points to B component

	// Pointers to the RGB arrays of the destination image
	pRnew = pdstImage;
	pGnew = pRnew + heightX * widthX;
	pBnew = pGnew + heightX * widthX;


	/*********************************************
	 * Algorithm start
	 *
	 * Measure initial time
	 *
	 */

    struct timespec tStart, tEnd;
    double dElapsedTimeS;

    printf("Running task: \n");
	if ( clock_gettime(CLOCK_REALTIME, &tStart) == -1 )
	{
		printf("ERROR: clock_gettime: %d.\n", errno);
		exit(EXIT_FAILURE);
	}


	/************************************************
	 * Algorithm.
	 * In this example, the algorithm is a components exchange
	 *
	 */

	for (int i = 0; i < heightX ; i++) {
		for (int j=0; j < widthX; j++)  {

			pRnew[i * widthX + j] = 255 - ( (256 * (255 - pRcompY[i * widthY + j])) / (pRcompX[i * widthX + j] + 1) );
			pGnew[i * widthX + j] = 255 - ( (256 * (255 - pGcompY[i * widthY + j])) / (pGcompX[i * widthX + j] + 1) );
			pBnew[i * widthX + j] = 255 - ( (256 * (255 - pBcompY[i * widthY + j])) / (pBcompX[i * widthX + j] + 1) );

			if(pRnew[i * widthX + j] > 255 || pGnew[i * widthX + j] > 255 || pBnew[i * widthX + j] > 255) printf("error");
		}
		//printf("%f\n", pRnew[i*widthX]);
	}


	/***********************************************
	 * End of the algorithm
	 *
	 * Measure the final time and calculate the time spent
	 *
	 */

	if ( clock_gettime(CLOCK_REALTIME, &tEnd) == -1 )
	{
		printf("ERROR: clock_gettime: %d.\n", errno);
		exit(EXIT_FAILURE);
	}
	printf("Finished\n");

    dElapsedTimeS = (tEnd.tv_sec - tStart.tv_sec);
    dElapsedTimeS += (tEnd.tv_nsec - tStart.tv_nsec) / 1e+9;
    printf("TIME: %f\n", dElapsedTimeS);
		

	// Create a new image object with the calculated pixels
	// In case of normal color image use nComp=3,
	// In case of B&W image use nComp=1.
	CImg<float> dstImage(pdstImage, widthX, heightX, 1, nCompX);

	// Store the destination image in disk
	dstImage.save("exit.bmp"); 

	// Display the destination image
	dstImage.display(); // If needed, show the result image
	return 0;

}


