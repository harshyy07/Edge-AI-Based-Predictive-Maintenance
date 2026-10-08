// Feature extraction identical to ../features.py (pure C++, host-testable).
#pragma once
#include <math.h>
#define FS 2000
#define WIN_N 1024

static float g_re[WIN_N], g_im[WIN_N];

static void fft_inplace() {                       // radix-2, in place
  for (int i = 1, j = 0; i < WIN_N; i++) {
    int bit = WIN_N >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) { float t = g_re[i]; g_re[i] = g_re[j]; g_re[j] = t; t = g_im[i]; g_im[i] = g_im[j]; g_im[j] = t; }
  }
  for (int len = 2; len <= WIN_N; len <<= 1) {
    float ang = -2.0f * (float)M_PI / len, wr = cosf(ang), wi = sinf(ang);
    for (int i = 0; i < WIN_N; i += len) {
      float cr = 1, ci = 0;
      for (int k = 0; k < len / 2; k++) {
        int a = i + k, b = i + k + len / 2;
        float vr = g_re[b] * cr - g_im[b] * ci, vi = g_re[b] * ci + g_im[b] * cr;
        g_re[b] = g_re[a] - vr; g_im[b] = g_im[a] - vi;
        g_re[a] += vr;          g_im[a] += vi;
        float ncr = cr * wr - ci * wi; ci = cr * wi + ci * wr; cr = ncr;
      }
    }
  }
}

// f[12]: rms, peak, crest, kurtosis, skew, std, dom_freq, 5 band energies
static void compute_features(const float *x, float *f) {
  double s = 0, s2 = 0, pk = 0;
  for (int i = 0; i < WIN_N; i++) { s += x[i]; s2 += (double)x[i] * x[i]; if (fabs(x[i]) > pk) pk = fabs(x[i]); }
  double mean = s / WIN_N, rms = sqrt(s2 / WIN_N), m2 = 0, m3 = 0, m4 = 0;
  for (int i = 0; i < WIN_N; i++) { double d = x[i] - mean, d2 = d * d; m2 += d2; m3 += d2 * d; m4 += d2 * d2; }
  m2 /= WIN_N; m3 /= WIN_N; m4 /= WIN_N;
  f[0] = rms; f[1] = pk; f[2] = pk / rms;
  f[3] = m4 / (m2 * m2) - 3.0; f[4] = m3 / pow(m2, 1.5); f[5] = sqrt(m2);

  for (int i = 0; i < WIN_N; i++) { g_re[i] = x[i] * (0.5f - 0.5f * cosf(2.0f * (float)M_PI * i / (WIN_N - 1))); g_im[i] = 0; }
  fft_inplace();
  const float lo[5] = {0, 40, 80, 150, 500}, hi[5] = {40, 80, 150, 500, 1000};
  float b[5] = {0, 0, 0, 0, 0}, best = -1; int bi = 0;
  for (int k = 0; k <= WIN_N / 2; k++) {
    float m = sqrtf(g_re[k] * g_re[k] + g_im[k] * g_im[k]) / WIN_N, fr = (float)k * FS / WIN_N;
    if (m > best) { best = m; bi = k; }
    for (int j = 0; j < 5; j++) if (fr >= lo[j] && fr < hi[j]) b[j] += m;
  }
  f[6] = (float)bi * FS / WIN_N;
  for (int j = 0; j < 5; j++) f[7 + j] = b[j];
}
