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

#include <hdfs.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/datastructures/CSRMatrix.h>
#include <runtime/local/datastructures/Frame.h>

#include <runtime/local/io/File.h>
#include <runtime/local/io/utils.h>
#include <runtime/local/io/ReadCsvFile.h>

#include <util/preprocessor_defs.h>

#include <type_traits>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <fstream>
#include <limits>
#include <sstream>
#include <iostream>

#include <fstream>







// ****************************************************************************
// Struct for partial template specialization
// ****************************************************************************

template <class DTRes> struct HDFSblocklocations {
  static void apply(DTRes *&res, const char *hdfsFilename) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

template <class DTRes>
void hDFSblocklocations(DTRes *&res, const char *hdfsFilename) {
  HDFSblocklocations<DTRes>::apply(res, hdfsFilename);
}


// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix
// ----------------------------------------------------------------------------

template <typename VT> struct HDFSblocklocations<DenseMatrix<VT>> {
  static void apply(DenseMatrix<VT> *&res, const char *hdfsFilename) {
    
    if(res == nullptr)
            res = DataObjectFactory::create<DenseMatrix<VT>>(4, 4, false);
    const char* host = "hdfs://localhost";
    tPort port = 9000;  // Default port
    const char* user = "hdoop";
    

    hdfsFS fs = hdfsConnectAsUser(host, port, user);
    if (fs == NULL) {
        std::cout << "Error connecting to HDFS" << std::endl;
    }


tOffset start = 0; // Start offset
    tOffset length = 1024; // Length of data to fetch
    int numOfBlock = 0;
    
    // Call the hdfsGetFileBlockLocations function to get block locations
    BlockLocation* blockLocations = hdfsGetFileBlockLocations(fs, hdfsFilename, start, length, &numOfBlock);

    // Check if block locations were successfully retrieved
    if (blockLocations != NULL) {
        // Iterate through the block locations and print them
        for (int i = 0; i < numOfBlock; ++i) {
            std::cout << "Block " << i << ":\n";
            std::cout << "  Hosts: ";
            for (int j = 0; j < blockLocations[i].numOfNodes; ++j) {
                std::cout << blockLocations[i].hosts[j] << " ";
            }
            std::cout << "\n";
            std::cout << "  Offset: " << blockLocations[i].offset << "\n";
            std::cout << "  Length: " << blockLocations[i].length << "\n";
        }

        // Free the block locations when you're done with them
        hdfsFreeFileBlockLocations(blockLocations, numOfBlock);
    } else {
        // Handle the case where block locations retrieval failed
        std::cerr << "Failed to retrieve block locations.\n";
        // You can check errno for the specific error code
    }
 

    hdfsDisconnect(fs);

    }


  
};

