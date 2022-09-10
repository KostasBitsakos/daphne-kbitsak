/*
 * Copyright 2021 The DAPHNE Consortium
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <runtime/local/datagen/GenGivenVals.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/datastructures/Frame.h>
#include <runtime/local/datastructures/Structure.h>
#include <runtime/local/datastructures/ValueTypeCode.h>
#include <runtime/local/kernels/EigenOp.h>
#include <runtime/local/kernels/CheckEq.h>

#include <tags.h>

#include <catch.hpp>

#include <vector>

#include <cstdint>



TEMPLATE_PRODUCT_TEST_CASE("Eigen Op Test", TAG_KERNELS, (DenseMatrix), (double, float)) {
    using VT = TestType;
    using DT = TestType;
    using DenseDT= DenseMatrix<VT>;
    std::cout << "main function Eigen \n";
    DT * res = nullptr;

    auto m0 = genGivenVals<DT>(4, {
        0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
    });

    auto m1 = genGivenVals<DT>(4, {
        0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
    });

    eigenOp<DT, DT>(res, m0, nullptr);
    CHECK(*res == *m1);
    DataObjectFactory::destroy(res);
    DataObjectFactory::destroy(m1);
    DataObjectFactory::destroy(m0);

}