/**
 * Filename: mandelmovie.c
 * Description: Runs mandel on multiple child
 * processes to generate many images.
 * Author: C. Shannon Terry
 * Section: 131
 * Assignment: Lab 11 - Multiprocessing
 * Date: 11/17/2026
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include "mandel.h"

#define IMAGECOUNT 50
#define COMMAND_LINE_BUFFER_SIZE 300
//The string for the command line options for mandel won't
//ever be more than 300 characters long. This is a safe bet.
//#define XCENTER -0.745 //Original
#define XCENTER -0.7425 //test
#define YCENTER 0.13141
//#define DEFAULTSCALE 0.01
#define DEFAULTSCALE 1 //test
#define TRANSFORM_LIMIT_IMAGE 4


int main(int argc, char* argv[]) {
    int childCount = 1; //Default is 1.
    double currentX = XCENTER;
    double currentY = YCENTER;
    double scale = DEFAULTSCALE;
    int currentImage = 0; //Used to track generated image count.
	char c;
    while((c = getopt(argc,argv,"c:h")) != -1) {
		switch(c) 
		{
			case 'c': //Child count
				childCount = atof(optarg);
				break;
			case 'h': //Help command
				show_help();
				exit(1);
				break;
		}
	}

	//Start loop for child processes
	while (currentImage < IMAGECOUNT) {
		//Ensure that exactly 50 images are created, regardless of child count.
		if ((currentImage + childCount) > IMAGECOUNT) {
			childCount = (IMAGECOUNT - currentImage);
		}
		for (int i = 0; i < childCount; i++) {
			pid_t pid = fork();
			if (pid < 0) {
				perror("Fork failure.\n");
				exit(1);
			} else if (pid == 0) { //Child process
				//currentImage += i; //Should work. Increment what image
				//we are using by using which child process it is.
				if (currentImage < TRANSFORM_LIMIT_IMAGE) {
					//This extra block allows for a "fly up" effect in the first few frames.
					//Helps a lot with cinematics.
					currentY += (currentImage*0.001);
				} else {
					currentY = YCENTER + (3*0.001);
					//Ensures we zoom in on the right spot.
				}
				int returnInt = \
				mandel(currentX, \
					   currentY, \
					   scale/((currentImage+1)*pow(1.1, currentImage)), \
					    0, currentImage);
					   //1.1 provides a nice multiple that allows for a smooth zoom
				i = childCount;
				if (returnInt != 0) {
					printf("Failed\n");
					exit(1);
				}
				exit(0); 
			} else { //Parent
				currentImage++; 
				//Increment what image we are on by 1. This will run for each child.
			}
		}
		while (wait(NULL) > 0); 
		//Wait for all children to finish. Has to be outside the loop
		//or the parent gets stuck each time and doesn't create more processes.
	}
	printf("Done.\n");
	return 0;    
}

void show_help(void) {
	printf("This program has two command line options:\n");
	printf("-h - Show the help menu and exit.\n");
	printf("-c - Include an integer after this option. That integer will be ");
	printf("the number of child processes used for the creation of the images.\n");
	printf("The default, if not provided, is 1.\n");
}