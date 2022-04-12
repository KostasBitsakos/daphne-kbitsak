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
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/datastructures/CSRMatrix.h>
#include <runtime/local/kernels/CheckEq.h>
#include <runtime/local/kernels/MatMul.h>

#include <tags.h>

#include <catch.hpp>

#include <vector>

template<class DT>
void checkMatMul(const DT * lhs, const DT * rhs, const DT * exp) {
    DT * res = nullptr;
    matMul<DT, DT, DT>(res, lhs, rhs, nullptr);
    CHECK(*res == *exp);
    DataObjectFactory::destroy(res);
}

template<class SparseDT, class DenseDT>
void checkMatMulSparse(const SparseDT * lhs, const SparseDT * rhs, const DenseDT * exp) {
    DenseDT * res = nullptr;
    matMul<DenseDT, SparseDT, SparseDT>(res, lhs, rhs, nullptr);
    CHECK(*res == *exp);
    DataObjectFactory::destroy(res);
}



TEMPLATE_PRODUCT_TEST_CASE("MatMul", TAG_KERNELS, (DenseMatrix), (float, double)) {
    using DT = TestType;
    
    auto m0 = genGivenVals<DT>(3, {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    });
    auto m1 = genGivenVals<DT>(3, {
        1, 2, 3,
        3, 1, 2,
        2, 3, 1,
    });
    auto m2 = genGivenVals<DT>(3, {
        13, 13, 10,
        10, 13, 13,
        13, 10, 13,
    });
    auto m3 = genGivenVals<DT>(2, {
        1, 0, 3, 0,
        0, 0, 2, 0,
    });
    auto m4 = genGivenVals<DT>(4, {
        0, 1,
        2, 0,
        1, 1,
        0, 0,
    });
    auto m5 = genGivenVals<DT>(2, {
        3, 4,
        2, 2,
    });
    auto v0 = genGivenVals<DT>(3, {
        0,
        0,
        0
    });
    auto v1 = genGivenVals<DT>(3, {
        1,
        1,
        1
    });
    auto v2 = genGivenVals<DT>(3, {
        1,
        2,
        3
    });
    auto v3 = genGivenVals<DT>(3, {
        6,
        6,
        6
    });
    auto v4 = genGivenVals<DT>(3, {
        14,
        11,
        11
    });
    auto v5 = genGivenVals<DT>(1, {
        1,
        2,
        3
    });
    auto v6 = genGivenVals<DT>(1, {14});

    checkMatMul(m0, m0, m0);
    checkMatMul(m1, m1, m2);
    checkMatMul(m3, m4, m5);
    checkMatMul(m0, v0, v0);
    checkMatMul(m1, v0, v0);
    checkMatMul(m2, v0, v0);
    checkMatMul(m0, v1, v0);
    checkMatMul(m1, v1, v3);
    checkMatMul(m1, v2, v4);
    checkMatMul(v5, v2, v6);

    DataObjectFactory::destroy(m0, m1, m2, m3, m4, m5, v0, v1, v2, v3, v4, v5, v6);
}



TEMPLATE_TEST_CASE("MatMulSparse", TAG_KERNELS, (float)) {
    using VT = TestType;
    using DT = CSRMatrix<VT>;
    using DenseDT= DenseMatrix<VT>;
    
    auto m0 = genGivenVals<DenseDT>(3, {
            2, 4, 3,
            1, 2, 0,
            0, 0, 0 
    });
    auto m1 = genGivenVals<DT>(3, {
            1, 2, 0, 
            0, 1, 0,
            0, 0, 0 
          
    });
    auto m2 = genGivenVals<DT>(3, {
            0, 0, 3, 
            1, 2, 0, 
            0, 0, 4
    });
    auto m3 = genGivenVals<DenseDT>(3, {
            0, 0, 0, 
            1, 4, 0, 
            0, 0, 0
    });
    


    checkMatMulSparse(m1, m2, m0);
    checkMatMulSparse(m2, m1, m3);
    
   
    DataObjectFactory::destroy(m0);
    DataObjectFactory::destroy(m1);
    DataObjectFactory::destroy(m2);
    DataObjectFactory::destroy(m3);
}


TEMPLATE_TEST_CASE("MatMulSparse", TAG_KERNELS, (double)) {
    using VT = TestType;
    using DT = CSRMatrix<VT>;
    using DenseDT= DenseMatrix<VT>;
    
    auto m0 = genGivenVals<DenseDT>(3, {
            2, 4, 3,
            1, 2, 0,
            0, 0, 0 
    });
    auto m1 = genGivenVals<DT>(3, {
            1, 2, 0, 
            0, 1, 0,
            0, 0, 0 
          
    });
    auto m2 = genGivenVals<DT>(3, {
            0, 0, 3, 
            1, 2, 0, 
            0, 0, 4
    });
    auto m3 = genGivenVals<DenseDT>(3, {
            0, 0, 0, 
            1, 4, 0, 
            0, 0, 0
    });
    


    checkMatMulSparse(m1, m2, m0);
    checkMatMulSparse(m2, m1, m3);
    
   
    DataObjectFactory::destroy(m0);
    DataObjectFactory::destroy(m1);
    DataObjectFactory::destroy(m2);
    DataObjectFactory::destroy(m3);
}