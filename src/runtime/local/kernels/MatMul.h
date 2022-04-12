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

#pragma once

#include <runtime/local/context/DaphneContext.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/CSRMatrix.h>
#include <runtime/local/datastructures/Matrix.h>
#include <runtime/local/datastructures/DenseMatrix.h>

#include <cblas.h>

#include <cassert>
#include <cstddef>

// ****************************************************************************
// Struct for partial template specialization
// ****************************************************************************

template<class DTRes, class DTLhs, class DTRhs>
struct MatMul {
    static void apply(DTRes *& res, const DTLhs * lhs, const DTRhs * rhs, DCTX(ctx)) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

template<class DTRes, class DTLhs, class DTRhs>
void matMul(DTRes *& res, const DTLhs * lhs, const DTRhs * rhs, DCTX(ctx)) {
    MatMul<DTRes, DTLhs, DTRhs>::apply(res, lhs, rhs, ctx);
}

// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix <- DenseMatrix, DenseMatrix
// ----------------------------------------------------------------------------

template<>
struct MatMul<DenseMatrix<float>, DenseMatrix<float>, DenseMatrix<float>> {
    static void apply(DenseMatrix<float> *& res, const DenseMatrix<float> * lhs, const DenseMatrix<float> * rhs, DCTX(ctx)) {
        const auto nr1 = static_cast<int>(lhs->getNumRows());
        const auto nc1 = static_cast<int>(lhs->getNumCols());
        const auto nc2 = static_cast<int>(rhs->getNumCols());
        assert((nc1 == static_cast<int>(rhs->getNumRows())) && "#cols of lhs and #rows of rhs must be the same");

        if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<float>>(nr1, nc2, false);

        if(nr1 == 1 && nc2 == 1) // Vector-Vector
            res->set(0, 0, cblas_sdot(nc1, lhs->getValues(), 1, rhs->getValues(),
                    static_cast<int>(rhs->getRowSkip())));
        else if(nc2 == 1)        // Matrix-Vector
            cblas_sgemv(CblasRowMajor, CblasNoTrans, nr1, nc1, 1, lhs->getValues(),
                    static_cast<int>(lhs->getRowSkip()), rhs->getValues(),
                    static_cast<int>(rhs->getRowSkip()), 0,res->getValues(),
                    static_cast<int>(res->getRowSkip()));
        else                     // Matrix-Matrix
            cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, nr1, nc2, nc1,
                    1, lhs->getValues(), static_cast<int>(lhs->getRowSkip()), rhs->getValues(),
                static_cast<int>(rhs->getRowSkip()), 0, res->getValues(), static_cast<int>(res->getRowSkip()));
    }
};

template<>
struct MatMul<DenseMatrix<double>, DenseMatrix<double>, DenseMatrix<double>> {
    static void apply(DenseMatrix<double> *& res, const DenseMatrix<double> * lhs, const DenseMatrix<double> * rhs, DCTX(ctx)) {
        const auto nr1 = static_cast<int>(lhs->getNumRows());
        const auto nc1 = static_cast<int>(lhs->getNumCols());
        const auto nc2 = static_cast<int>(rhs->getNumCols());
        assert((nc1 == static_cast<int>(rhs->getNumRows())) && "#cols of lhs and #rows of rhs must be the same");

        if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<double>>(nr1, nc2, false);

        if(nr1 == 1 && nc2 == 1) // Vector-Vector
            res->set(0, 0, cblas_ddot(nc1, lhs->getValues(), 1, rhs->getValues(),
                static_cast<int>(rhs->getRowSkip())));
        else if(nc2 == 1)        // Matrix-Vector
            cblas_dgemv(CblasRowMajor, CblasNoTrans, nr1, nc1, 1, lhs->getValues(),
            static_cast<int>(lhs->getRowSkip()), rhs->getValues(),static_cast<int>(rhs->getRowSkip()), 0,
                res->getValues(), static_cast<int>(res->getRowSkip()));
        else                     // Matrix-Matrix
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, nr1, nc2, nc1,
                    1, lhs->getValues(), static_cast<int>(lhs->getRowSkip()), rhs->getValues(),
                    static_cast<int>(rhs->getRowSkip()), 0, res->getValues(), static_cast<int>(res->getRowSkip()));
    }
};






// ----------------------------------------------------------------------------
// DENSEMatrix <- CSRMatrix, CSRMatrix
// ----------------------------------------------------------------------------

template<>
struct MatMul<DenseMatrix<double>, CSRMatrix<double>, CSRMatrix<double>> {
    static void apply(DenseMatrix<double> *& res, const CSRMatrix<double> * lhs, const CSRMatrix<double> * rhs, DCTX(ctx)) {

        const auto nr1 = static_cast<int>(lhs->getNumRows());
        const auto nc1 = static_cast<int>(lhs->getNumCols());
        const auto nc2 = static_cast<int>(rhs->getNumCols());
        assert((nc1 == static_cast<int>(rhs->getNumRows())) && "#cols of lhs and #rows of rhs must be the same");



        if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<double>>(nr1, nc2, false);




         for(size_t r = 0; r < nr1; r++) {   // iterate rows of left array
             size_t NonZeros=lhs->getNumNonZeros(r);
             if (NonZeros){
                 const size_t * colIdxsRowLhs = lhs->getColIdxs(r);   // array with all non Zeros columns on Left Matrix
                 



                 for(size_t c=0; c<nc2; c++){     // iterate columns of second array
                     double result=0;     
                     for(size_t clhs=0; clhs<NonZeros; clhs++){
                         size_t nonZeroColId=colIdxsRowLhs[clhs];   // iterate non zero elemens
                         double lelement=lhs->get(r,nonZeroColId);
                         double relement=rhs->get(nonZeroColId,c);
                         result+=lelement*relement;

                     }

                     
                     res->set(r,c,result);
                 }
             }
             else {
                 for(size_t c=0; c<nc2; c++){ 
                     res->set(r,c,0);   //automaticaly set to 0 if no NonZero elements on this row

                    }

                }
             }

             

            }

        
};




template<>
struct MatMul<DenseMatrix<float>, CSRMatrix<float>, CSRMatrix<float>> {
    static void apply(DenseMatrix<float> *& res, const CSRMatrix<float> * lhs, const CSRMatrix<float> * rhs, DCTX(ctx)) {

        const auto nr1 = static_cast<int>(lhs->getNumRows());
        const auto nc1 = static_cast<int>(lhs->getNumCols());
        const auto nc2 = static_cast<int>(rhs->getNumCols());
        assert((nc1 == static_cast<int>(rhs->getNumRows())) && "#cols of lhs and #rows of rhs must be the same");



        if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<float>>(nr1, nc2, false);




         for(size_t r = 0; r < nr1; r++) {   // iterate rows of left array
             size_t NonZeros=lhs->getNumNonZeros(r);
             if (NonZeros){
                 const size_t * colIdxsRowLhs = lhs->getColIdxs(r);   // array with all non Zeros columns on Left Matrix
                 



                 for(size_t c=0; c<nc2; c++){     // iterate columns of second array
                     float result=0;     
                     for(size_t clhs=0; clhs<NonZeros; clhs++){
                         size_t nonZeroColId=colIdxsRowLhs[clhs];   // iterate non zero elemens
                         float lelement=lhs->get(r,nonZeroColId);
                         float relement=rhs->get(nonZeroColId,c);
                         result+=lelement*relement;

                     }

                     
                     res->set(r,c,result);
                 }
             }
             else {
                 for(size_t c=0; c<nc2; c++){ 
                     res->set(r,c,0);   //automaticaly set to 0 if no NonZero elements on this row

                    }

                }
             }

             

            }

        
};
