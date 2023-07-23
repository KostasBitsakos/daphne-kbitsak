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

#ifndef SRC_RUNTIME_LOCAL_KERNELS_READHDFS_H
#define SRC_RUNTIME_LOCAL_KERNELS_READHDFS_H

#include <runtime/local/context/DaphneContext.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/datastructures/Frame.h>
#include <runtime/local/io/File.h>
#include <runtime/local/io/ReadHDFSCsv.h>
// #include <runtime/local/io/ReadMM.h>
// #include <runtime/local/io/ReadParquet.h>
// #include <runtime/local/io/ReadDaphne.h>
// #include <parser/metadata/MetaDataParser.h>

#include <string>
#include <regex>
#include <map>


// ****************************************************************************
// Struct for partial template specialization
// ****************************************************************************

template<class DTRes>
struct ReadHDFS {
    static void apply(DTRes *& res, const char * filename,const char *hdfsFilename,size_t numRows,
                    size_t numCols, DCTX(ctx)) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

template<class DTRes>
void readHDFS(DTRes *& res, const char * filename,const char *hdfsFilename,size_t numRows,
                    size_t numCols, DCTX(ctx)) {
    ReadHDFS<DTRes>::apply(res, filename, hdfsFilename, numRows, numCols, ctx);
}

// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix
// ----------------------------------------------------------------------------

template<typename VT>
struct ReadHDFS<DenseMatrix<VT>> {
    static void apply(DenseMatrix<VT> *& res, const char * filename,const char *hdfsFilename,size_t numRows,
                    size_t numCols, DCTX(ctx)) {

	
		if(res == nullptr)
			res = DataObjectFactory::create<DenseMatrix<VT>>(
				numRows, numCols, false
			);
		readHDFSCsv(res, filename, hdfsFilename, numRows, numCols, ',');
	
    }
};


#endif //SRC_RUNTIME_LOCAL_KERNELS_READHDFS_H
