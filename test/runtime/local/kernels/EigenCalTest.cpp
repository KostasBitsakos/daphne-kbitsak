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
#include <runtime/local/kernels/EigenCal.h>

#include <tags.h>

#include <catch.hpp>

#include <vector>

#include <cstdint>


template<class DT>
void checkEigenCal(const DT * inMat, const DT * exp1, const DT * exp2) {
    DT * res1 = nullptr;
    DT * res2 = nullptr;
    eigenCal<DT,DT, DT>(res1, res2, inMat, nullptr);
    std::cout<<"results \n\n\n" << std::endl;
    std::cout << typeid(*res1).name();
    std::cout<<"\n" << std::endl;
    std::cout << typeid(*exp1).name();
    std::cout<<"\n\n" << std::endl;

    // res1->print(std::cout);
    // res2->print(std::cout);
    //CHECK(*res1 == *exp1);
    CHECK(*res2 == *exp2);
    DataObjectFactory::destroy(res1);
    DataObjectFactory::destroy(res2);
}


TEMPLATE_PRODUCT_TEST_CASE("EigenCal", TAG_KERNELS, (DenseMatrix), (double)) {
    //using DT = DenseMatrix<float>;
    using DT=TestType;

      auto m0 = genGivenVals<DT>(4, {
        -0.522371, 0.721017, 0.372318, 0.261996,
0.263355, -0.242033, 0.925557, -0.124135,
-0.581254, -0.140892, 0.0210948, -0.801154,
-0.565611, -0.633802, 0.0654158, 0.523546
        
    });
   

        auto m1 = genGivenVals<DT>(4, {
     1.00671141, -0.11010327,  0.87760486,  0.82344326,
    -0.11010327,  1.00671141, -0.42333835, -0.358937,
    0.87760486, -0.42333835,  1.00671141,  0.96921855,
    0.82344326, -0.358937,    0.96921855,  1.00671141
 });

    
   
   
    auto v0 = genGivenVals<DT>(4, {
        2.93035,
        0.148342,
        0.927404,
        0.020746
    });



//     auto m0 = genGivenVals<DenseMatrix<float>>(4, {
//         -0.522371, 0.721017, 0.372318, 0.261996,
// 0.263355, -0.242033, 0.925557, -0.124135,
// -0.581254, -0.140892, 0.0210948, -0.801154,
// -0.565611, -0.633802, 0.0654158, 0.523546
        
//     });
   

//         auto m1 = genGivenVals<DenseMatrix<float>>(4, {
//      1.00671141, -0.11010327,  0.87760486,  0.82344326,
//     -0.11010327,  1.00671141, -0.42333835, -0.358937,
//     0.87760486, -0.42333835,  1.00671141,  0.96921855,
//     0.82344326, -0.358937,    0.96921855,  1.00671141
//  });

    
   
   
//     auto v0 = genGivenVals<DenseMatrix<float>>(4, {
//         2.93035,
//         0.148342,
//         0.927404,
//         0.020746
//     });
    
    
    checkEigenCal(m0, v0, m1);

    DataObjectFactory::destroy(m0, m1,  v0);
}

