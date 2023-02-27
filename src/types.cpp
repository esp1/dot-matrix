#include "types.h"

MatrixState::MatrixState(MD_MAX72XX *const _matrix) : matrix(_matrix) {
  graphics = new GraphicsBuffer();
}