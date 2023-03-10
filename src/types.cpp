#include "types.h"

namespace {

GraphicsBuffer _graphics;

}

DotMatrixState::DotMatrixState(MD_MAX72XX *const _matrix)
    : matrix(_matrix), graphics(&_graphics) {}