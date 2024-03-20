/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include "imageloader.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 200

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
  FILE *ppm;
  Image *img;
  int pixel;
  int col, row;
  int r, g, b;
  char buf[MAX_LEN + 1];

  // read ppm file
  ppm = fopen(filename, "r");
  if (!ppm) exit(1);

  fgets(buf, MAX_LEN, ppm);  // skip fist line
  // store col and row
  fscanf(ppm, "%d %d", &col, &row);

  img = (Image *)malloc(sizeof(struct Image));
  if (!img) exit(1);
  img->cols = col;
  img->rows = row;
  fscanf(ppm, "%d", &pixel);
  img->image = (Color **)malloc(sizeof(struct Color *) * row);

  for (int i = 0; i < row; i++) {
    img->image[i] = (Color *)malloc(sizeof(struct Color) * col);
    for (int j = 0; j < col; j++) {
      fscanf(ppm, " %d  %d  %d    ", &r, &g, &b);
      img->image[i][j].R = (uint8_t)r;
      img->image[i][j].G = (uint8_t)g;
      img->image[i][j].B = (uint8_t)b;
    }
  }

  fclose(ppm);
  return img;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the
// image's data.
void writeData(Image *image) {
  printf("P3\n");
  printf("%d %d\n", image->cols, image->rows);
  printf("255\n");
  for (int i = 0; i < image->rows; i++) {
    for (int j = 0; j < image->cols; j++) {
      printf("%3hhu %3hhu %3hhu", image->image[i][j].R, image->image[i][j].G,
             image->image[i][j].B);
      if (j != image->cols - 1) printf("   ");
    }
    printf("\n");
  }
}

// Frees an image
void freeImage(Image *image) {
  for (int i = 0; i < image->rows; i++) {
    free(image->image[i]);
  }
  free(image->image);
  free(image);
}