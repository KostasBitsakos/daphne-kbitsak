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

#ifndef SRC_RUNTIME_LOCAL_KERNELS_EIGEN_H
#define SRC_RUNTIME_LOCAL_KERNELS_EIGEN_H

#pragma once

#include <runtime/local/context/DaphneContext.h>
#include <runtime/local/datastructures/CSRMatrix.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
// #include <runtime/local/datastructures/Frame.h>
// #include <runtime/local/datastructures/ValueTypeCode.h>


// #include <algorithm>
// #include <random>
// #include <set>
// #include <type_traits>



//kostas test

#include <Eigen/Eigenvalues> // header file
#include <Eigen/Dense>

#include <iostream>


//end kostas test

// #include <cassert>
// #include <cmath>
// #include <cstddef>
// #include <cstdint>
// #include <chrono>

#include <cassert>
#include <cstddef>


// ****************************************************************************
// Struct for partial template specialization
// res1 matrix for eigenvalues, res2 matrix for eigenvectors
// Column k of the returned matrix res2 is an eigenvector corresponding to eigenvalue number k as returned by eigenvalues(). 
// The eigenvectors are normalized to have (Euclidean) norm equal to one. 
// ****************************************************************************

// SOS template<class DTRes, typename VTArg>
// struct EigenCal {
//     static void apply(DTRes *& res1, DTRes *& res2, const VTArg * inMat, DCTX(ctx)) = delete;
// };

template<class DTRes1, class DTRes2, class VTArg>
struct EigenCal {
    static void apply(DTRes1 *& res1, DTRes2 *& res2, const VTArg * inMat, DCTX(ctx)) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

// SOS template<class DTRes, typename VTArg>
// void eigenCal(DTRes *& res1,DTRes *& res2, const VTArg * inMat, DCTX(ctx)) {
//     EigenCal<DTRes, VTArg>::apply(res1,res2, inMat, ctx);
// }

template<class DTRes1, class DTRes2, class VTArg>
void eigenCal(DTRes1 *& res1, DTRes2 *& res2, const VTArg * inMat, DCTX(ctx)) {
    EigenCal<DTRes1, DTRes2, VTArg>::apply(res1,res2, inMat, ctx);
}

// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix
// ----------------------------------------------------------------------------

template<>
struct EigenCal<DenseMatrix<double>,DenseMatrix<double>,DenseMatrix<double>> {
    static void apply(DenseMatrix<double> *& res1,DenseMatrix<double> *& res2, const DenseMatrix<double> * inMat, DCTX(ctx)) {
        const auto nr = static_cast<size_t>(inMat->getNumRows());
        const auto nc = static_cast<size_t>(inMat->getNumCols());
        // const auto nc2 = static_cast<int>(rhs->getNumCols());
        // assert((nc1 == static_cast<int>(rhs->getNumRows())) && "#cols of lhs and #rows of rhs must be the same");
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> inputMatrix(nr, nc);
       // Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> inputMatrix; // declare a real (double) 2x2 matrix
       
       std::tuple<int, int>{1, -1};
        for(size_t r=0; r<nr; r++){
            for(size_t c=0; c<nc; c++){
                std::cout << inMat->get(r,c) << "what \n";
                inputMatrix(r,c)=inMat->get(r,c);
            }
        }
    inMat->print(std::cout);
//Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigensolver(A);
  Eigen::EigenSolver<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> > s(inputMatrix);  // the instance s(A) includes the eigensystem
  std::cout << inputMatrix << std::endl;
  std::cout << "eigenvalues:" << std::endl;
  std::cout << s.eigenvalues() << std::endl;
  std::cout << "eigenvectors=" << std::endl;
  std::cout << s.eigenvectors() << std::endl;
  std::cout << s.eigenvalues().cols() << std::endl;
  std::cout << s.eigenvalues().rows() << std::endl;
  std::cout << s.eigenvectors().cols() << std::endl;
  std::cout << s.eigenvectors().rows() << std::endl;
  
  size_t eigenValuesrows=s.eigenvalues().rows();
  size_t eigenValuescols=s.eigenvalues().cols();

  size_t eigenVectorsrows=s.eigenvectors().rows();
  size_t eigenVectorscols=s.eigenvectors().cols();
  Eigen::MatrixXd eigenVectors = s.eigenvectors().real().cast <double> (); 

    if(res1 == nullptr)
             res1= DataObjectFactory::create<DenseMatrix<double>>(eigenValuesrows, eigenValuescols, false);


    if(res2 == nullptr)
             res2= DataObjectFactory::create<DenseMatrix<double>>(eigenVectorsrows, eigenVectorscols, false);

    for(size_t r = 0; r < eigenValuesrows; r++) {
                for(size_t c = 0; c < eigenValuescols; c++) {
                    res1->set(r,c,s.eigenvalues()[r].real());
    }
    }
    float element=0.0;
    for(size_t r = 0; r < eigenVectorsrows; r++) {
                for(size_t c = 0; c < eigenVectorscols; c++) {
            //res2->set(r,c,s.eigenvectors().row(r).col(c));
        //     std::cout<<eigenVectors.row(r).col(c) <<"\n check " << std::endl;
        //     std::cout<<eigenVectors.coeff(r, c) <<"\n check " << std::endl;
        //    std::cout<< typeid(eigenVectors.row(r).col(c)).name()<<"\n check2 " << std::endl;

            //element=eigenVectors.row(r).col(c);
            res2->set(r,c,eigenVectors.coeff(r, c));
    }
    }
   
    res1->print(std::cout);
    res2->print(std::cout);
    }
};
#endif //SRC_RUNTIME_LOCAL_KERNELS_EIGEN_H