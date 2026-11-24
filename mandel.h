/**
 * Filename: mandel.h
 * Description: Header file for mandel.c, which no longer is
 * the main method
 * Author: C. Shannon Terry
 * Section: 131
 * Assignment: Lab 11 - Multiprocessing
 * Date: 11/17/2026
 */

#include "jpegrw.h"

//int mandel(int argc, char* argv[]);
int mandel(double x_center, double y_center, double x_scale, double y_scale, int currentIteration);
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
static void show_help();