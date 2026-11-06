#pragma once
/*
  PLACEHOLDER self-test vectors for the servo prototype.

  IMPORTANT:
  - This file only satisfies the symbols referenced by the original sketch.
  - The zero-filled vectors are NOT valid validation data.
  - The feature self-test result is NOT meaningful until these arrays are
    regenerated from the exact Python feature extractor and model.
  - Replace this file when the matching edge_features.h and model.h are ready.

  Required definitions from edge_features.h / model.h:
    WIN_N, N_FEAT, RF_N_CLASSES
*/

#ifndef WIN_N
  #error "WIN_N must be defined by edge_features.h before selftest_vectors.h"
#endif

#ifndef N_FEAT
  #error "N_FEAT must be defined by edge_features.h before selftest_vectors.h"
#endif

#ifndef RF_N_CLASSES
  #error "RF_N_CLASSES must be defined by model.h before selftest_vectors.h"
#endif

// One zero-valued raw window. This is a compile-time placeholder only.
static const float ST_RAW[WIN_N] = {0.0f};

// Placeholder expected feature vector. Not a real golden reference.
static const float ST_RAW_FEAT[N_FEAT] = {0.0f};

// One placeholder feature vector and class index for the RF self-test.
// This does not validate model correctness.
static const int ST_N_VEC = 1;
static const float ST_X[1][N_FEAT] = {{0.0f}};
static const int ST_Y[1] = {0};
