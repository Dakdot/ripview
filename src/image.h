#pragma once

#include "stdint.h"

typedef struct {
  const char *filename;
  const uint8_t *data;
} rvImage;

rvImage *load_image(const char *filename);
