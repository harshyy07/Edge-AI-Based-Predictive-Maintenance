#pragma once
#include <math.h>

// Feature order must match servo_vibration_ml/features.py:
// ax(mean,std,rms,abs_peak), then ay, then az.
static inline void servo_extract_features(const float samples[][3], int count, float out[12]) {
  for (int axis = 0; axis < 3; ++axis) {
    float mean = 0.0f;
    float square_sum = 0.0f;
    float peak = 0.0f;
    for (int i = 0; i < count; ++i) {
      const float value = samples[i][axis];
      mean += value;
      square_sum += value * value;
      const float magnitude = fabsf(value);
      if (magnitude > peak) peak = magnitude;
    }
    mean /= (float)count;
    float variance = 0.0f;
    for (int i = 0; i < count; ++i) {
      const float delta = samples[i][axis] - mean;
      variance += delta * delta;
    }
    variance /= (float)count;
    const int base = axis * 4;
    out[base] = mean;
    out[base + 1] = sqrtf(variance);
    out[base + 2] = sqrtf(square_sum / (float)count);
    out[base + 3] = peak;
  }
}
