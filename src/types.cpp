#include "types.h"

DotMatrixState::DotMatrixState(MD_MAX72XX *const _matrix) : matrix(_matrix) {
  graphics = new GraphicsBuffer();
}