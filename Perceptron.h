#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "util.h"

#pragma once

typedef struct {
  int layerSize;
  int connectionsCount;
  int* receptors;
  int* associations;
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

void setDoubleArray2d(double* array, int size, int row, int col, double value) {
  array[row * size + col] = value;
}
double getDoubleArray2d(double* array, int size, int row, int col) {
  return array[row * size + col];
}

void setWeight(Perceptron* perceptron, int row, int col, double value) {
  setDoubleArray2d(perceptron->weights, perceptron->layerSize, row, col, value);
}
double getWeight(Perceptron* perceptron, int row, int col) {
  return getDoubleArray2d(perceptron->weights, perceptron->layerSize, row, col);
}

Position* getConnection(Perceptron* perceptron, int row, int col, int number) {
  return perceptron->connections + row * perceptron->layerSize + col * perceptron->connectionsCount + number;
}

Perceptron* createPerceptron(int layerSize, int connectionsCount) {
  Perceptron* result = malloc(sizeof(Perceptron));

  result->layerSize = layerSize;
  result->connectionsCount = connectionsCount;

  result->receptors = malloc(layerSize * layerSize * sizeof(int));
  result->associations = malloc(layerSize * layerSize * sizeof(int));

  result->weights = malloc(layerSize * layerSize * sizeof(double));
  result->connections = malloc(layerSize * layerSize * connectionsCount * sizeof(Position));
  for (int r = 0; r < layerSize; r += 1) {
    for (int c = 0; c < layerSize; c += 1) {
      setWeight(result, r, c, 0);

      for (int k = 0; k < connectionsCount; k += 1) {
        Position* connection = getConnection(result, r, c, k);
        connection->row = rand() % layerSize;
        connection->col = rand() % layerSize;
      }
    }
  }

  return result;
}

int generateImage(Perceptron* perceptron) {
  int kind = rand() % 2;

  int radius = rand() % (perceptron->layerSize / 2);
  if (radius < 5) {
    radius = 5;
  }

  int centerR = (rand() % (int)(perceptron->layerSize - 2 * radius)) + radius;
  int centerC = (rand() % (int)(perceptron->layerSize - 2 * radius)) + radius;

  int top = centerR - radius;
  int left = centerC - radius;
  int right = centerC + radius;
  int bottom = centerR + radius;

  int circleDotCount = 100;

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      setReceptor(perceptron, r, c, 0);
    }
  }

  // О П С Т
  // switch (kind) {
  //   case 0:
  //     for (int i = 0; i < circleDotCount; i += 1) {
  //       int r = centerR + (int)(radius * cos(2 * 3.14 * (i / (double)circleDotCount)));
  //       int c = centerC + (int)(radius * sin(2 * 3.14 * (i / (double)circleDotCount)));
  //       setReceptor(perceptron, r, c, 1);
  //     }
  //     break;
  //   case 1:
  //     for (int r = top; r < bottom; r += 1) {
  //       setReceptor(perceptron, r, left, 1);
  //       setReceptor(perceptron, r, right, 1);
  //     }

  //     for (int c = left; c < right; c += 1) {
  //       setReceptor(perceptron, top, c, 1);
  //     }
  //     break;
  //   case 2:
  //     for (int i = 0; i < circleDotCount; i += 1) {
  //       int r = centerR - (int)(radius * cos(3.14 * (i / (double)circleDotCount)));
  //       int c = centerC - (int)(radius * sin(3.14 * (i / (double)circleDotCount)));
  //       setReceptor(perceptron, r, c, 1);
  //     }
  //     break;
  //   case 3:
  //     for (int r = top; r < bottom; r += 1) {
  //       setReceptor(perceptron, r, centerC, 1);
  //     }

  //     for (int c = left; c < right; c += 1) {
  //       setReceptor(perceptron, top, c, 1);
  //     }
  //     break;
  // }

  // 1 3
  // switch (kind) {
  //   case 0:
  //     for (int r = top; r < bottom; r += 1) {
  //       setReceptor(perceptron, r, centerC, 1);
  //     }
  //     break;
  //   case 1:
  //     for (int i = 0; i < circleDotCount; i += 1) {
  //       int r = centerR + radius / 2 + (int)(radius / 2 * cos(3.14 * (i / (double)circleDotCount)));
  //       int c = centerC + (int)(radius * sin(3.14 * (i / (double)circleDotCount)));
  //       setReceptor(perceptron, r, c, 1);

  //       r = centerR - radius / 2 + (int)(radius / 2 * cos(3.14 * (i / (double)circleDotCount)));
  //       c = centerC + (int)(radius * sin(3.14 * (i / (double)circleDotCount)));
  //       setReceptor(perceptron, r, c, 1);
  //     }
  //     break;
  // }

  // 0 +
  switch (kind) {
    case 0:
      for (int i = 0; i < circleDotCount; i += 1) {
        int r = centerR + (int)(radius * cos(2 * 3.14 * (i / (double)circleDotCount)));
        int c = centerC + (int)(radius * sin(2 * 3.14 * (i / (double)circleDotCount)));
        setReceptor(perceptron, r, c, 1);
      }
      break;
    case 1:
      for (int r = top; r < bottom; r += 1) {
        setReceptor(perceptron, r, centerC, 1);
      }
      for (int c = left; c < right; c += 1) {
        setReceptor(perceptron, centerR, c, 1);
      }
      break;
  }

  // for (int r = 0; r < perceptron->layerSize; r += 1) {
  //   for (int c = 0; c < perceptron->layerSize; c += 1) {
  //     printf("%c", getReceptor(perceptron, r, c) ? '#' : '.');
  //   }
  //   printf("\n");
  // }

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
      if (association > 0) {
        setAssociation(perceptron, r, c, 1);
      }
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

int recognize(Perceptron* perceptron) {
  double out = 0;

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      out += getAssociation(perceptron, r, c) * getWeight(perceptron, r, c);
    }
  }

  return out > 0;
}

void adjust(Perceptron* perceptron, int expectedImage) {
  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      if (getAssociation(perceptron, r, c) == 0) {
        continue;
      }

      double curWeight = getWeight(perceptron, r, c);

      double adjustment = (expectedImage == 1) ? 0.1 : -0.1;

      setWeight(perceptron, r, c, curWeight + adjustment);
    }
  }
}

void trainPerceptron(Perceptron* perceptron, int batches, int batchSize) {
  for (int batch = 0; batch < batches; batch += 1) {
    int correct = 0;
    for (int step = 0; step < batchSize; step += 1) {
      int expectedImage = generateImage(perceptron);

      associate(perceptron);
      int recognizedImage = recognize(perceptron);

      if (expectedImage == recognizedImage) {
        correct += 1;
      } else {
        adjust(perceptron, expectedImage);
      }
    }

    printf("Correct: %d\n", correct);
  }
  printf("Weights:\n");

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      printf("%c", (getWeight(perceptron, r, c) > 0) ? '+' : '-');
    }
    printf("\n");
  }
}

void presentImage(Perceptron* perceptron) {
  int expectedImage = generateImage(perceptron);

  associate(perceptron);
  int recognizedImage = recognize(perceptron);

  for (int r = 0; r < perceptron->layerSize; r += 1) {
    for (int c = 0; c < perceptron->layerSize; c += 1) {
      printf("%c", getReceptor(perceptron, r, c) ? '#' : '.');
    }
    printf("\n");
  }

  if (recognizedImage == 0) {
    printf("1\n");
  } else {
    printf("3\n");
  }
}
