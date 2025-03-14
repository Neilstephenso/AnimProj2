// ASCII Animation Project
// 
// Project written by: Neil Stephenson
// Date: 3/14/25
// 
// 
// This file simply draws a cube in a console window space and rotates that cube based upon a preinputted set of matrix equations.
//	Heavy inspiration was taken from Servet Gulnaroglu on youtube, and I plan to add more funtionality to this later such as 
//	being able to animate triangles for more complex shapes.
// 
// TODO: Add functionality to animate triangular planes.
//

#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

double A, B, C;
double x, y, z;
float triangleWidth = 10;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCII = ' ';
double incrementSpeed = 0.6;
int distanceFromCam = 120;
double ooz;
//x position / y position
int xp, yp;
double K1 = 40;
int idx;

static double calculateX(int i, int j, int k) {
	return j * sin(A) * sin(B) * cos(C) 
		- k * cos(A) * sin(B) * cos(C) 
		+ j * cos(A) * sin(C) 
		+ k * sin(A) * sin(C) 
		+ i * cos(B) * cos(C);
}
static double calculateY(int i, int j, int k) {
	return j * cos(A) * cos(C) 
		+ k * sin(A) * cos(C) 
		- j * sin(A) * sin(B) * sin(C) 
		+ k * cos(A) * sin(B) * sin(C) 
		- i * cos(B) * sin(C);
}
static double calculateZ(int i, int j, int k) {
	return k * cos(A) * cos(B) 
		- j * sin(A) * cos(B) 
		+ i * sin(B);
}
static void calculateSurface(double triX, double triY, double triZ, char c) {
	x = calculateX(triX, triY, triZ);
	y = calculateY(triX, triY, triZ);
	z = calculateZ(triX, triY, triZ) + distanceFromCam;

	ooz = 1 / z;

	xp = (int)(width / 2 + K1 * ooz * x * 2);
	yp = (int)(height / 2 + K1 * ooz * y * 2);

	idx = xp + yp * width;
	if (idx >= 0 && idx < width * height) {
		if (ooz > zBuffer[idx]) {
			zBuffer[idx] = ooz;
			buffer[idx] = c;
		}
	}
}

// Calculations for a triangular plane instead of a square one.
static void calculateTriangleSurface(double triX, double triY, double triZ, char c) {
	x = calculateX(triX, triY, triZ);
	y = calculateY(triX, triY, triZ);
	z = calculateZ(triX, triY, triZ) + distanceFromCam;
}

int main()
{
	printf("\x1b[2j");
	while (1) {

		memset(buffer, backgroundASCII, width * height);
		memset(zBuffer, 0, width * height * 4);

		for (float triX = -triangleWidth; triX < triangleWidth; triX += incrementSpeed) {

			for (float triY = -triangleWidth; triY < triangleWidth; triY += incrementSpeed) {

				calculateSurface(triX, triY, -triangleWidth, '@');
				calculateSurface(-triX, triY, triangleWidth, '~');
				calculateSurface(triX, -triangleWidth, -triY, '0');
				calculateSurface(triX, triangleWidth, triY, 'X');
				calculateSurface(triangleWidth, triY, triX, '+');
				calculateSurface(-triangleWidth, triY, -triX, '*');
			}
		}
		printf("\x1b[H");
		for (int k = 0; k < width * height; k++) {
			putchar(k % 160 ? buffer[k] : 10);
		}

		A += 0.045;
		B += 0.045;
		C += 0.01;
	}
}

