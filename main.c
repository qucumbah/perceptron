#include "util.h"
#include "Perceptron.h"

int main() {
  Perceptron* perceptron = createPerceptron(20, 20);

  trainPerceptron(perceptron, 1000, 1000);

  for (int i = 0; i < 10; i += 1) {
    presentImage(perceptron);
  }

  return 0;
}
