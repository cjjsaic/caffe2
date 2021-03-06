#include "caffe2/operators/order_switch_ops.h"

namespace caffe2 {

template <>
bool NHWC2NCHWOp<float, CPUContext>::RunOnDevice() {
  auto& X = Input(0);
  auto* Y = Output(0);
  DCHECK_EQ(X.ndim(), 4);
  const int N = X.dim(0), H = X.dim(1), W = X.dim(2), C = X.dim(3);
  Y->Reshape(std::vector<int>{N, C, H, W});
  const float* Xdata = X.data();
  float* Ydata = Y->mutable_data();
  for (int n = 0; n < N; ++n) {
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
        for (int c = 0; c < C; ++c) {
          Ydata[((n * C + c) * H + h) * W + w] = *(Xdata++);
        }
      }
    }
  }
  return true;
}

template <>
bool NCHW2NHWCOp<float, CPUContext>::RunOnDevice() {
  auto& X = Input(0);
  auto* Y = Output(0);
  DCHECK_EQ(X.ndim(), 4);
  const int N = X.dim(0), C = X.dim(1), H = X.dim(2), W = X.dim(3);
  Y->Reshape(std::vector<int>{N, H, W, C});
  const float* Xdata = X.data();
  float* Ydata = Y->mutable_data();
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
          Ydata[((n * H + h) * W + w) * C + c] = *(Xdata++);
        }
      }
    }
  }
  return true;
}


namespace {
REGISTER_CPU_OPERATOR(NHWC2NCHW, NHWC2NCHWOp<float, CPUContext>)
REGISTER_CPU_OPERATOR(NCHW2NHWC, NCHW2NHWCOp<float, CPUContext>)
}  // namespace
}  // namespace caffe2
