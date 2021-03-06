#ifndef CAFFE2_OPERATORS_AVERAGEPOOL_OP_H_
#define CAFFE2_OPERATORS_AVERAGEPOOL_OP_H_

#include "caffe2/core/context.h"
#include "caffe2/core/operator.h"
#include "caffe2/operators/conv_pool_op_base.h"
#include "caffe2/utils/math.h"
#include "glog/logging.h"

namespace caffe2 {

template <typename dtype, class DeviceContext>
class AveragePoolOp final : public ConvPoolOpBase<dtype, DeviceContext> {
 public:
  USE_CONV_POOL_BASE_FUNCTIONS;
  AveragePoolOp(const OperatorDef& operator_def, Workspace* ws)
      : ConvPoolOpBase<dtype, DeviceContext>(operator_def, ws) {}
  ~AveragePoolOp() {}

  bool RunOnDeviceWithOrderNCHW() override;
  bool RunOnDeviceWithOrderNHWC() override;

  // Input: X
  // Output: Y
  INPUT_OUTPUT_STATS(1, 1, 1, 1);
  DISABLE_COPY_AND_ASSIGN(AveragePoolOp);
};

template <typename dtype, class DeviceContext>
class AveragePoolGradientOp final :
    public ConvPoolOpBase<dtype, DeviceContext> {
 public:
  USE_CONV_POOL_BASE_FUNCTIONS;
  AveragePoolGradientOp(const OperatorDef& operator_def, Workspace* ws)
      : ConvPoolOpBase<dtype, DeviceContext>(operator_def, ws) {}
  ~AveragePoolGradientOp() {}

  bool RunOnDeviceWithOrderNCHW() override;
  bool RunOnDeviceWithOrderNHWC() override;

  // Input: X, Y_grad
  // Output: X_grad
  INPUT_OUTPUT_STATS(2, 2, 1, 1);
  DISABLE_COPY_AND_ASSIGN(AveragePoolGradientOp);
};


}  // namespace caffe2

#endif  // CAFFE2_OPERATORS_AVERAGEPOOL_OP_H_
