#include "caffe2/operators/order_switch_ops.h"
#include "caffe2/core/context_gpu.h"

namespace caffe2 {

__global__ void NHWC2NCHWKernel(const int N, const int HW, const int C,
                                const float* X, float* Y) {
  CUDA_1D_KERNEL_LOOP(i, N * HW * C) {
    const int c = i % C;
    const int hw = i / C % HW;
    const int n = i / C / HW;
    Y[(n * C + c) * HW + hw] = X[i];
  }
}

__global__ void NCHW2NHWCKernel(const int N, const int C, const int HW,
                                const float* X, float* Y) {
  CUDA_1D_KERNEL_LOOP(i, N * C * HW) {
    const int hw = i % HW;
    const int c = i / HW % C;
    const int n = i / C / HW;
    Y[(n * HW + hw) * C + c] = X[i];
  }
}

template <>
bool NHWC2NCHWOp<float, CUDAContext>::RunOnDevice() {
  auto& X = Input(0);
  auto* Y = Output(0);
  DCHECK_EQ(X.ndim(), 4);
  const int N = X.dim(0), H = X.dim(1), W = X.dim(2), C = X.dim(3);
  Y->Reshape(std::vector<int>{N, C, H, W});
  NHWC2NCHWKernel<<<CAFFE_GET_BLOCKS(X.size()), CAFFE_CUDA_NUM_THREADS,
                    0, device_context_.cuda_stream()>>>(
      N, H * W, C, X.data(), Y->mutable_data());
  return true;
}

template <>
bool NCHW2NHWCOp<float, CUDAContext>::RunOnDevice() {
  auto& X = Input(0);
  auto* Y = Output(0);
  DCHECK_EQ(X.ndim(), 4);
  const int N = X.dim(0), C = X.dim(1), H = X.dim(2), W = X.dim(3);
  Y->Reshape(std::vector<int>{N, H, W, C});
  NCHW2NHWCKernel<<<CAFFE_GET_BLOCKS(X.size()), CAFFE_CUDA_NUM_THREADS,
                    0, device_context_.cuda_stream()>>>(
      N, C, H * W, X.data(), Y->mutable_data());
  return true;
}


namespace {
REGISTER_CUDA_OPERATOR(NHWC2NCHW, NHWC2NCHWOp<float, CUDAContext>)
REGISTER_CUDA_OPERATOR(NCHW2NHWC, NCHW2NHWCOp<float, CUDAContext>)
}  // namespace
}  // namespace caffe2
