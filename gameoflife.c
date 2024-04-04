/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "imageloader.h"

// helper function: stupid but works
static int aliveNR(Image *image, int rowIndex, int colIndex) {
  int neighbors = 0;
  int row = image->rows;
  int col = image->cols;
  // west
  if (image->image[rowIndex][(colIndex - 1) % col].R == 255) neighbors++;
  // east
  if (image->image[rowIndex][(colIndex + 1) % col].R == 255) neighbors++;
  // north
  if (255 == image->image[(rowIndex + row - 1) % row][colIndex].R) neighbors++;
  // south
  if (255 == image->image[(rowIndex + 1) % row][colIndex].R) neighbors++;
  // northwest
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex - 1) % col].R)
    neighbors++;
  // northeast
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex + 1) % col].R)
    neighbors++;
  // southwest
  if (255 == image->image[(rowIndex + 1) % row][(colIndex - 1) % col].R)
    neighbors++;
  // southeast
  if (255 == image->image[(rowIndex + 1) % row][(colIndex + 1) % col].R)
    neighbors++;

  return neighbors;
}

static int aliveNG(Image *image, int rowIndex, int colIndex) {
  int neighbors = 0;
  int row = image->rows;
  int col = image->cols;
  // west
  if (image->image[rowIndex][(colIndex - 1) % col].G == 255) neighbors++;
  // east
  if (image->image[rowIndex][(colIndex + 1) % col].G == 255) neighbors++;
  // north
  if (255 == image->image[(rowIndex + row - 1) % row][colIndex].G) neighbors++;
  // south
  if (255 == image->image[(rowIndex + 1) % row][colIndex].G) neighbors++;
  // northwest
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex - 1) % col].G)
    neighbors++;
  // northeast
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex + 1) % col].G)
    neighbors++;
  // southwest
  if (255 == image->image[(rowIndex + 1) % row][(colIndex - 1) % col].G)
    neighbors++;
  // southeast
  if (255 == image->image[(rowIndex + 1) % row][(colIndex + 1) % col].G)
    neighbors++;

  return neighbors;
}

static int aliveNB(Image *image, int rowIndex, int colIndex) {
  int neighbors = 0;
  int row = image->rows;
  int col = image->cols;
  // west
  if (image->image[rowIndex][(colIndex - 1) % col].B == 255) neighbors++;
  // east
  if (image->image[rowIndex][(colIndex + 1) % col].B == 255) neighbors++;
  // north
  if (255 == image->image[(rowIndex + row - 1) % row][colIndex].B) neighbors++;
  // south
  if (255 == image->image[(rowIndex + 1) % row][colIndex].B) neighbors++;
  // northwest
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex - 1) % col].B)
    neighbors++;
  // northeast
  if (255 == image->image[(rowIndex + row - 1) % row][(colIndex + 1) % col].B)
    neighbors++;
  // southwest
  if (255 == image->image[(rowIndex + 1) % row][(colIndex - 1) % col].B)
    neighbors++;
  // southeast
  if (255 == image->image[(rowIndex + 1) % row][(colIndex + 1) % col].B)
    neighbors++;

  return neighbors;
}

// Determines what color the cell at the given row/col should be. This function
// allocates space for a new Color. Note that you will need to read the eight
// neighbors of the cell in question. The grid "wraps", so we treat the top row
// as adjacent to the bottom row and the left column as adjacent to the right
// column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
  Color *newColor = (Color *)malloc(sizeof(Color));
  if (!newColor) exit(-1);

  int ruleArr[18];  // 0x00000 - 0x3FFFF
  int isAliveR = 0, isAliveG = 0, isAliveB = 0;
  int aliveNeighborsR = 0, aliveNeighborsG = 0, aliveNeighborsB = 0;

  isAliveR = image->image[row][col].R == 255;
  isAliveG = image->image[row][col].G == 255;
  isAliveB = image->image[row][col].B == 255;

  aliveNeighborsR = aliveNR(image, row, col);
  aliveNeighborsG = aliveNG(image, row, col);
  aliveNeighborsB = aliveNB(image, row, col);

  for (int i = 0; i < 18; ++i) {
    ruleArr[i] = (rule >> i) & 1;
  }

  // 0 -8 for alive, 9 - 17 for dead
  if (isAliveR) {
    if (ruleArr[9 + aliveNeighborsR])
      newColor->R = 255;
    else
      newColor->R = 0;
  } else {
    if (ruleArr[aliveNeighborsR])
      newColor->R = 255;
    else
      newColor->R = 0;
  }

  if (isAliveG) {
    if (ruleArr[9 + aliveNeighborsG])
      newColor->G = 255;
    else
      newColor->G = 0;
  } else {
    if (ruleArr[aliveNeighborsG])
      newColor->G = 255;
    else
      newColor->G = 0;
  }

  if (isAliveB) {
    if (ruleArr[9 + aliveNeighborsB])
      newColor->B = 255;
    else
      newColor->B = 0;
  } else {
    if (ruleArr[aliveNeighborsB])
      newColor->B = 255;
    else
      newColor->B = 0;
  }

  return newColor;
}

// The main body of Life; given an image and a rule, computes one iteration of
// the Game of Life. You should be able to copy most of this from
// steganography.c
Image *life(Image *image, uint32_t rule) {
  // you can not manipulate original image!
  Image *retImage = (Image *)malloc(sizeof(struct Image));
  if (!retImage) exit(-1);
  retImage->cols = image->cols;
  retImage->rows = image->rows;

  Color *color = (Color *)malloc(sizeof(struct Color));
  if (!color) exit(-1);

  retImage->image = (Color **)malloc(sizeof(struct Color *) * image->rows);

  for (int i = 0; i < image->rows; i++) {
    retImage->image[i] = (Color *)malloc(sizeof(struct Color) * image->cols);
    for (int j = 0; j < image->cols; j++) {
      color = evaluateOneCell(image, i, j, rule);
      retImage->image[i][j].R = color->R;
      retImage->image[i][j].G = color->G;
      retImage->image[i][j].B = color->B;
    }
  }

  return retImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then
prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this
will be a string. You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you
should exit with code -1. Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv) {
  if (argc != 3) {
    printf("usage: %s filename rule\n", argv[0]);
    printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
    printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
    exit(-1);
  }

  Image *image;
  char *hex = argv[2];
  char *filename = argv[1];
  image = readData(filename);

  uint32_t rule = strtol(hex, NULL, 0);
  image = life(image, rule);
  writeData(image);
  freeImage(image);

  return 0;
}
