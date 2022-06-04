#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned char* data;
  int width;
  int height;
} BinImage;

BinImage* readImage(char* inFilePath) {
  FILE* inFile = fopen(inFilePath, "rb");

  BinImage* result = malloc(sizeof(BinImage));

  int width;
  int height;
  fread(&width, sizeof(int), 1, inFile);
  fread(&height, sizeof(int), 1, inFile);

  result->width = width;
  result->height = height;

  unsigned char* data = malloc(width * height);
  result->data = data;

  fread(data, 1, width * height, inFile);

  fclose(inFile);

  return result;
}

int getPixel(BinImage* image, int r, int c) {
  return *(image->data + r * 100 + c) != 0;
}
