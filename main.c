#include "util.h"
#include "Perceptron.h"

int main() {
  Perceptron* perceptron = createPerceptron(20, 20);

  trainPerceptron(perceptron, 500, 100);

  for (int i = 0; i < 30; i += 1) {
    presentImage(perceptron);
  }

  return 0;
}
