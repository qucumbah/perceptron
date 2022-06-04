#pragma once

typedef struct {
  int row;
  int col;
} Position;

int log2int(int value) {
  int buffer = value;
  int result = 0;
  while (buffer > 1) {
    buffer >>= 1;
    result += 1;
  }

  return result;
}
