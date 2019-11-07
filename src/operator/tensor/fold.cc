/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * Copyright (c) 2019 by Contributors
 * \file fold.cc
 * \brief CPU implementation of unfold operator
 * \author Istvan Fehervari
*/

#include "./fold-inl.h"
namespace mxnet {
namespace op {

DMLC_REGISTER_PARAMETER(UnfoldParam);

NNVM_REGISTER_OP(unfold)
.describe(R"code(Extracts sliding local blocks from a batched input tensor.

Consider an batched input tensor of shape (N, C, *), where N is the batch dimension, C is the channel dimension, and * represent arbitrary spatial dimensions.
This operation flattens each sliding kernel_size-sized block within the spatial dimensions of input into a column (i.e., last dimension) of a 3-D output tensor of shape (N, C \times \prod(\text{kernel\_size}), L)(N,C×∏(kernel_size),L) , where C \times \prod(\text{kernel\_size})C×∏(kernel_size) is the total number of values within each block (a block has \prod(\text{kernel\_size})∏(kernel_size) spatial locations each containing a CC -channeled vector), and LL is the total number of such blocks:

(text copied from https://pytorch.org/docs/stable/nn.html?highlight=unfold#torch.nn.functional.unfold)

)code")
.set_num_outputs(1)
.set_num_inputs(1)
.set_attr_parser(ParamParser<UnfoldParam>)
.set_attr<nnvm::FListInputNames>("FListInputNames",
    [](const NodeAttrs& attrs) {
  return std::vector<std::string>{"data"};
})
.set_attr<mxnet::FInferShape>("FInferShape", UnfoldOpShape)
.set_attr<nnvm::FInferType>("FInferType", UnfoldOpType)
.set_attr<FCompute>("FCompute<cpu>", UnfoldOpForward<cpu>)
.set_attr<nnvm::FGradient>("FGradient", ElemwiseGradUseNone{"_backward_unfold"})
.add_argument("data", "NDArray-or-Symbol", "data")
.add_arguments(UnfoldParam::__FIELDS__());


NNVM_REGISTER_OP(_backward_unfold)
.set_attr_parser(ParamParser<UnfoldParam>)
.set_num_inputs(1)
.set_num_outputs(1)
.set_attr<nnvm::TIsBackward>("TIsBackward", true)
.set_attr<FCompute>("FCompute<cpu>", UnfoldOpBackward<cpu>);

}  // namespace op
}  // namespace mxnet