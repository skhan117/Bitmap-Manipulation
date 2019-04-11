/*
  Name: Shaan Khan

  COP4338 Assignment #2: BMP Image Processing

  This program manipulates bitmap images; images can be flipped vertically or
  horizontally, rotated in 90 degree increments, and/or enlarged. The user's
  commands are entered at the command line via getopt() as in previous programs.
  Memory is used efficiently and memory leaks are prevented.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmplib.h"
#include <unistd.h>
#include <string.h>


/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 *
 * Newrows and newcols were changed from pointers to regular integers.
 * The program still works as intended.
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
      PIXEL** new, int newrows, int newcols)
{
  // Try to enlarge the image by scale.
  int row, col;

  // Return -1 if image is invalid.
  if ((rows <= 0) || (cols <= 0)) return -1;

  int i = 0;
  int j = 0;

  newrows = scale*scale*rows;
  newcols = scale*scale*cols;

  // Allocate memory for new, enlarged bmp
  *new = (PIXEL*)malloc((newrows)*(newcols)*sizeof(PIXEL));

  // Iterate over each row
  for (row = 0; row < rows; row++) {
    // Multiply pixel by scale in one dimension
    for (i = 0; i < scale; i++) {
      // Iterate over each col
      for (col = 0; col < cols; col++) {
        // Multiply pixel by scale in second dimension
        for (j = 0; j < scale; j++) {
          // Find the pixel to enlarge
          PIXEL* o = original + row*cols + col;
          // Stretch out pixel in two dimensions
          // E.g. If scale is 2x, then 1 pixel becomes 4
          PIXEL* n = (*new) + scale*scale*row*cols + scale*i*cols + scale*col + j;
          *n = *o;
        }
      }
    }
  }
  // Return 0 if method was successful to enable later piping.
  return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 *
 * Note: I changed newrows and newcols from pointers to regular ints.
 * The program still works as intended.
 */

int rotate(PIXEL* original, int rows, int cols, int rotation,
     PIXEL** new, int newrows, int newcols)
{

  int row, col;

  // If the rotation exceeds +360, then rotation = rotation % 360.
  // e.g. Rotate +450 degrees is equivalent of rotate +90 degrees.
  if (rotation > 360) {
    rotation = rotation % 360;
  }

  // If the rotation is less than -360, then rotation = rotation % 360.
  // E.g. Rotate -540 degrees is equivalent of rotate -180degrees
  if (rotation < -360) {
    rotation = rotation % 360;
  }

  if ((rows <= 0) || (cols <= 0)) return -1;

  // Allocate memory for the new bmp.
  *new = (PIXEL*)malloc(newcols*newrows*sizeof(PIXEL));

  // We must deal with four rotations: 90, 180, 270, and 0

  // Case 1: Rotation of +90 or -270 are equivalent
  if (rotation == 90 || rotation == -270) {
    for (row=0; row < rows; row++) { 
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        // Rotate 90 degrees clockwise        
        PIXEL* n = (*new) + newrows * (newcols - col - 1) + (row);
        *n = *o;
      }
    }
  }
  // Case 2: Rotation of +180 or -180 are equivalent
  else if (rotation == 180 || rotation == -180) {
    for (row=0; row < rows; row++) { 
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        // Rotate 180 degrees clockwise        
        PIXEL* n = (*new) + newcols * (newrows - row - 1) + (newcols - col - 1);
        *n = *o;
      }
    }
  }
  // Case 3: Rotation of -90 or +270 are equivalent
  else if (rotation == -90 || rotation == 270) {
    for (row=0; row < rows; row++) { 
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        // Rotate 90 degrees counterclockwise        
        PIXEL* n = (*new) + newrows * col + (newcols - 1 - row);
        *n = *o;
      }
    }
  }
  // Case 4: Rotation of 0
  else if (rotation == 0) {
    for (row=0; row < rows; row++) { 
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        // Just return the image as-is, no processing is needed.
        PIXEL* n = (*new) + row*newcols + col;
        *n = *o;
      }
    }
  }
  return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  // The verticalflip method is based on the given flip method.
  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (col=0; col < cols; col++)
    for (row=0; row < rows; row++) {
      // Pixel will have its row changed, but column unchanged.
      // This means that if pixel is originally in first row, it will 
      // go to terminal row, but in the same column as it was originally.
      // If pixel is originally in second row, it will go to terminal - 1 
      // row, but again will stay in the same column.
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + (rows-1-row)*cols + col;
      *n = *o;
    }

  return 0;
}


/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

// instructions() method will print out instructions for the user if needed.
void instructions() {
  printf("\nInstructions: ./bmptool [-s scale | -r degree | -f | -v ] [-o output_file] [input_file]\n");
  printf("For piping, follow this example: ./bmptool -f example.bmp | ./bmptool -f -o 9.bmp\n");
}

// Main function will get user's commands via getopt() and call functions

int main (int argc, char *argv[]) {

  // Set flags for our functions
  int scaleFlag = 0;
  int rotateFlag = 0;
  int horFlip = 0;
  int verFlip = 0;
  int outFile = 0;

  // Keep track of number of commands for each operation to prevent multiples
  int sOps = 0;
  int rOps = 0;
  int fOps = 0;
  int vOps = 0;

  // Variables to hold scale and rotation degrees
  int whatScale = 0;
  int whatDegree = 0;

  // Flags for stdin or stdout

  int r, c;       // r for row, c for col
  PIXEL *b; 
  PIXEL *nb;      // b for old bmp, nb for new bmp

  char bmpFileName[100];
  char bmpOutputFileName[100];

  int argNum;
  argNum = 0;

  int isThereFileOutput = 0; // flags for file output and input
  int isThereFileInput = 0;

  // Getopt will parse the command line, and set proper flags.
  char ch;
  while ((ch = getopt(argc, argv, "s:r:fvo:")) != -1)
  	switch (ch) {
     	// If user selects s, then get scale for enlarge function argument
     	// and set scale flag on
      		case 's':
        		if (sOps > 0) {
          			printf("Can only enlarge once at a time.\n");
          			break;
        		}
        		whatScale = atoi(optarg);
        		scaleFlag = 1;
        		sOps++;
        		break;
      		// If user selects s, get degrees for rotation argument and set
      		// rotate flag on
      		case 'r': 
        		if (rOps > 0) {
          			printf("Can only rotate once at a time.\n");
          			break;
        		}
        		whatDegree = atoi(optarg);
        		rotateFlag = 1;
        		rOps++;
        		break;
      		// If user selects f, set horflip flag on
      		case 'f':
        		if (fOps > 0) {
          			printf("Can only flip horizontally once at a time.\n");
          			break;
        		}
        		horFlip = 1;
        		fOps++;
        		break;
      		// If user selects v, set verFlip flag on
      		case 'v':
        		if (vOps > 0) {
          			printf("Can only flip vertically once at a time.\n");
          			break;
        		}
        		verFlip = 1;
       			vOps++;
        		break;
      		// If user selects o, get output file name and set flag on
      		case 'o':
        		strcpy(bmpOutputFileName, optarg);
        		outFile = 1;
        		isThereFileOutput = 1;
        		break;
    	} // end getopt

 	//printf("Got to end of switch\n");

 	// Get bmp filename from last value in argv[] array
  	strcpy(bmpFileName, argv[argc - 1]);


  	// Try to read the file.
  	// readFile returns 0 if the operation is successful.
  	// readFile returns -1 if the operation isn't successful.
  	if ((readFile(bmpFileName, &r, &c, &b)) != -1) {
    		//printf("Successful read\n");
    		isThereFileInput = 1;
    		readFile(bmpFileName, &r, &c, &b);
 	}
  	else {
    		//printf("No input file, reading from standard input");
    		readFile(NULL, &r, &c, &b);
  	}


  	nb = b;

  	// Now process the image in this order: scale, rotate, verflip, horflip 
  	if (scaleFlag == 1) {
    		enlarge(b, r, c, whatScale, &nb, r, c);
    		b = nb;
    		r = whatScale * r;
    		c = whatScale * c;
  	}

  	if (rotateFlag == 1) {
    		rotate(b, r, c, whatDegree, &nb, r, c);
    		b = nb;
  	}

  	if (verFlip == 1) {
    		verticalflip(b, &nb, r, c);
    		b = nb;
  	}

  	if (horFlip == 1) {
    		flip(b, &nb, r, c);
  	}

  	if (outFile==1) {
    		//printf("Writing out a file\n");
    		writeFile(bmpOutputFileName, r, c, nb);
  	}
  	else {
    		//printf("Not writing out a file\n");
    		writeFile(NULL, r, c, nb);
  	}

  	// Free up memory
  	// free(b);
  	// free(nb);


  	return 0;

} // end main method

