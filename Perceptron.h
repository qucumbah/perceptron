#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#include "util.h"
#include "BinImage.h"

#pragma once

typedef struct {
  int layerSize;
  int connectionsCount;
  int outputsCount;
  int* receptors;
  int* associations;
  int* outputs;
  Position* connections;
  double* weights;
} Perceptron;

void setIntArray2d(int* array, int size, int row, int col, int value) {
  array[row * size + col] = value;
}
int getIntArray2d(int* array, int size, int row, int col) {
  return array[row * size + col];
}

void setReceptor(Perceptron* perceptron, int row, int col, int value) {
  setIntArray2d(perceptron->receptors, perceptron->layerSize, row, col, value);
}
int getReceptor(Perceptron* perceptron, int row, int col) {
  return getIntArray2d(perceptron->receptors, perceptron->layerSize, row, col);
}

void setAssociation(Perceptron* perceptron, int row, int col, int value) {
  setIntArray2d(perceptron->associations, perceptron->layerSize, row, col, value);
}
int getAssociation(Perceptron* perceptron, int row, int col) {
  return getIntArray2d(perceptron->associations, perceptron->layerSize, row, col);
}
void incAssociation(Perceptron* perceptron, int row, int col) {
  int newValue = getAssociation(perceptron, row, col) + 1;
  setIntArray2d(perceptron->associations, perceptron->layerSize, row, col, newValue);
}

void setWeight(Perceptron* perceptron, int row, int col, int out, double value) {
  perceptron->weights[row * perceptron->layerSize * perceptron->outputsCount + col * perceptron->outputsCount + out] = value;
}
double getWeight(Perceptron* perceptron, int row, int col, int out) {
  return perceptron->weights[row * perceptron->layerSize * perceptron->outputsCount + col * perceptron->outputsCount + out];
}

void setOutput(Perceptron* perceptron, int outputNumber, int value) {
  perceptron->outputs[outputNumber] = value;
}
int getOutput(Perceptron* perceptron, int outputNumber) {
  return perceptron->outputs[outputNumber];
}

Position* getConnection(Perceptron* perceptron, int row, int col, int number) {
  return perceptron->connections + row * perceptron->layerSize + col * perceptron->connectionsCount + number;
}

Perceptron* createPerceptron(int layerSize, int connectionsCount, int outputsCount) {
  Perceptron* result = malloc(sizeof(Perceptron));

  result->layerSize = layerSize;
  result->connectionsCount = connectionsCount;
  result->outputsCount = outputsCount;

  result->receptors = malloc(layerSize * layerSize * sizeof(int));
  result->associations = malloc(layerSize * layerSize * sizeof(int));
  result->outputs = malloc(outputsCount * sizeof(int));

  result->weights = malloc(layerSize * layerSize * outputsCount * sizeof(double));
  result->connections = malloc(layerSize * layerSize * connectionsCount * sizeof(Position));
  for (int r = 0; r < layerSize; r += 1) {
    for (int c = 0; c < layerSize; c += 1) {
      for (int k = 0; k < connectionsCount; k += 1) {
        Position* connection = getConnection(result, r, c, k);
        connection->row = rand() % layerSize;
        connection->col = rand() % layerSize;
      }

      for (int o = 0; o < outputsCount; o += 1) {
        setWeight(result, r, c, o, 0);
      }
    }
  }

  return result;
}

BinImage* getBinImages(int count) {
  BinImage* result = malloc(count * sizeof(BinImage));

  char inFilePath[256];
  for (int i = 0; i < count; i += 1) {
    sprintf(inFilePath, "./img/%d.bin", i + 1);
    readImageTo(inFilePath, &result[i]);
  }

  return result;
}

int generateAndReceiveImage(Perceptron* perceptron, BinImage* images) {
  int radius = rand() % (perceptron->layerSize / 2);
  int treshold = 5;
  if (radius < treshold) {
    radius = treshold;
  }

  int centerR = (rand() % (int)(perceptron->layerSize - 2 * radius)) + radius;
  int centerC = (rand() % (int)(perceptron->layerSize - 2 * radius)) + radius;

  int top = centerR - radius;
  int left = centerC - radius;

  int kind = rand() % perceptron->outputsCount;
  BinImage* image = &images[kind];

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      setReceptor(perceptron, r, c, 0);
    }
  }

  for (int r = 0; r < 2 * radius; r += 1) {
    for (int c = 0; c < 2 * radius; c += 1) {
      int threeR = (int)(r / (2.0 * radius) * image->width);
      int threeC = (int)(c / (2.0 * radius) * image->height);
      int pixel = getPixel(image, threeR, threeC);
      setReceptor(perceptron, left + r, top + c, pixel);
    }
  }

  return kind;
}

void associate(Perceptron* perceptron) {
  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      setAssociation(perceptron, r, c, 0);
    }
  }

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      if (getReceptor(perceptron, r, c) == 0) {
        continue;
      }

      for (int k = 0; k < perceptron->connectionsCount; k += 1) {
        Position* connectedAssociation = getConnection(perceptron, r, c, k);
        incAssociation(perceptron, connectedAssociation->row, connectedAssociation->col);
      }
    }
  }

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      int association = getAssociation(perceptron, r, c);
      setAssociation(perceptron, r, c, association > 2);
    }
  }

  // for (int r = 0; r < perceptron->layerSize; r += 1) {
  //   for (int c = 0; c < perceptron->layerSize; c += 1) {
  //     printf("%c", getAssociation(perceptron, r, c) ? '#' : '.');
  //   }
  //   printf("\n");
  // }

  // printf("");
}

void recognize(Perceptron* perceptron) {
  double* outputsDouble = calloc(perceptron->outputsCount, sizeof(double));

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      for (int o = 0; o < perceptron->outputsCount; o += 1) {
        int association = getAssociation(perceptron, r, c);
        double weight = getWeight(perceptron, r, c, o);
        outputsDouble[o] += association * weight;
      }
    }
  }

  for (int o = 0; o < perceptron->outputsCount; o += 1) {
    setOutput(perceptron, o, outputsDouble[o] > 0);
  }
}

bool isOutputCorrect(Perceptron* perceptron, int expectedImage) {
  for (int o = 0; o < perceptron->outputsCount; o += 1) {
    int outputValue = getOutput(perceptron, o);
    int expectedValue = o == expectedImage;
    if (outputValue != expectedValue) {
      return false;
    }
  }

  return true;
}

void adjust(Perceptron* perceptron, int expectedImage) {
  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      if (getAssociation(perceptron, r, c) == 0) {
        continue;
      }

      for (int o = 0; o < perceptron->outputsCount; o += 1) {
        int outputValue = perceptron->outputs[o];
        int expectedValue = o == expectedImage;
        if (outputValue == expectedValue) {
          continue;
        }

        double curWeight = getWeight(perceptron, r, c, o);

        double adjustment = (expectedValue == 1) ? 0.1 : -0.1;

        setWeight(perceptron, r, c, o, curWeight + adjustment);
      }
    }
  }

  // printf("Expecting %d\n", expectedImage);

  // for (int r = 0; r < perceptron->layerSize; r += 1) {
  //   for (int c = 0; c < perceptron->layerSize; c += 1) {
  //     printf("%c", getAssociation(perceptron, r, c) ? '#' : '.');
  //   }
  //   printf("\n");
  // }

  // for (int o = 0; o < perceptron->outputsCount; o += 1) {
  //   for (int r = 0; r < perceptron->layerSize; r += 1) {
  //     for (int c = 0; c < perceptron->layerSize; c += 1) {
  //       printf("%c", getWeight(perceptron, r, c, o) ? '#' : '.');
  //     }
  //     printf("\n");
  //   }
  //   printf("--------\n");
  // }
  // printf("--------\n");
}

void trainPerceptron(Perceptron* perceptron, int batches, int batchSize) {
  BinImage* images = getBinImages(perceptron->outputsCount);
  for (int batch = 0; batch < batches; batch += 1) {
    int correct = 0;
    for (int step = 0; step < batchSize; step += 1) {
      int expectedImage = generateAndReceiveImage(perceptron, images);
      associate(perceptron);
      recognize(perceptron);

      if (isOutputCorrect(perceptron, expectedImage)) {
        correct += 1;
      } else {
        adjust(perceptron, expectedImage);
      }
    }

    printf("Correct: %d\n", correct);
  }
}

void presentImage(Perceptron* perceptron) {
  BinImage* images = getBinImages(perceptron->outputsCount);

  int expectedImage = generateAndReceiveImage(perceptron, images);

  associate(perceptron);
  recognize(perceptron);

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      printf("%c", getReceptor(perceptron, r, c) ? '#' : '.');
    }
    printf("\n");
  }

  if (isOutputCorrect(perceptron, expectedImage)) {
    printf("Guessed correctly:\n");
  } else {
    printf("Guessed incorrectly:\n");
  }

  printf(
    "Guessed:  %d %d %d %d\n", 
    perceptron->outputs[0],
    perceptron->outputs[1],
    perceptron->outputs[2],
    perceptron->outputs[3]
  );

  printf(
    "Expected: %d %d %d %d\n", 
    expectedImage == 0,
    expectedImage == 1,
    expectedImage == 2,
    expectedImage == 3
  );
}
