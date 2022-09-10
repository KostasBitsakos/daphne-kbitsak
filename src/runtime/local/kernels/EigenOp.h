
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


#ifndef SRC_RUNTIME_LOCAL_KERNELS_EIGENOP_H
#define SRC_RUNTIME_LOCAL_KERNELS_EIGENOP_H

#include <runtime/local/context/DaphneContext.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/datastructures/Frame.h>
#include <runtime/local/datastructures/ValueTypeCode.h>
#include <runtime/local/datastructures/ValueTypeUtils.h>
//#include <eigen/Eigen/Dense>
// #include <Eigen/Dense>



// ****************************************************************************
// Struct for partial template specialization
// ****************************************************************************


template<class DTRes, class DTArg>
struct EigenOp {
    static void apply(DTRes *& res, const DTArg * arg, DCTX(ctx)) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************


/**
 * @brief Takes as input a DenseMatrix and returns its eigen vectors and eigen values
 * 
 * @param arg The input dense matrix
 * @return The output eigen vectors and eigen values
 */



template<class DTRes, class DTArg>
void eigenOp(DTRes *& res, const DTArg * arg, DCTX(ctx)) {
    EigenOp<DTRes, DTArg>::apply(res, arg, ctx);
}



// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************



template<typename VTRes, typename VTArg>
class EigenOp<DenseMatrix<VTRes>, DenseMatrix<VTArg>> {

public:
    static void apply(DenseMatrix<VTRes> *& res, const DenseMatrix<VTArg> * arg, DCTX(ctx)) {
        const auto nr1 = static_cast<int>(arg->getNumRows());
        const auto nc1 = static_cast<int>(arg->getNumCols());
        assert((nr1 == static_cast<int>(arg->getNumCols())) && "The input matrix must be a square matrix ");

        if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<VTRes>>(nr1, nc1, false);
        float lelement;
        std::cout << nr1 << " " << nc1 << "test \n";
        for(size_t r = 0; r < nr1; r++) {
            for(size_t c=0; c<nc1; c++){
                lelement=arg->get(r,c);
                res->set(r,c,lelement);
            }
        }

    }
};


#endif //SRC_RUNTIME_LOCAL_KERNELS_EIGENOP_H