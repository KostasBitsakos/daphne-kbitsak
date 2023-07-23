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

template <class DTRes> struct ReadHDFSCsv {
  static void apply(DTRes *&res, const char *filename,const char *hdfsFilename, size_t numRows, size_t numCols,
                    char delim) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

template <class DTRes>
void readHDFSCsv(DTRes *&res, const char *filename,const char *hdfsFilename, size_t numRows, size_t numCols,
             char delim) {
  ReadHDFSCsv<DTRes>::apply(res, filename,hdfsFilename, numRows, numCols, delim);
}


// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix
// ----------------------------------------------------------------------------

template <typename VT> struct ReadHDFSCsv<DenseMatrix<VT>> {
  static void apply(DenseMatrix<VT> *&res, const char *filename, const char *hdfsFilename, size_t numRows,
                    size_t numCols, char delim) {
    //struct File *file = openFile(filename);
    const char* hdfsFilePath = hdfsFilename;
    const char* localFilePath = filename;
    std::ifstream file(localFilePath);
    bool fileExists = file.good();
    file.close();

    if (!fileExists) {

    const char* host = "hdfs://localhost";
    tPort port = 9000;  // Default port
    const char* user = "hdoop";
    

    hdfsFS fs = hdfsConnectAsUser(host, port, user);
    if (fs == NULL) {
        std::cout << "Error connecting to HDFS" << std::endl;
    }

    char buffer1[256];
    hdfsGetWorkingDirectory(fs, buffer1, sizeof(buffer1));
    const char* workingDir = buffer1;

   

    //Open the HDFS file for reading
    hdfsFile hdfsFile = hdfsOpenFile(fs, hdfsFilePath, O_RDONLY, 0, 0, 0);
    if (hdfsFile == NULL) {
        std::cout << "Error opening HDFS file" << std::endl;
        hdfsDisconnect(fs);
    }
    else {
             ; 
    }

    struct File *localFile = openFileForWrite(localFilePath);
  if (localFile == NULL) {
    std::cout << "Error opening local file" << std::endl;
    
  }
    const int bufferSize = 4096;

   char buffer[bufferSize];
   int bytesRead = 0;

while ((bytesRead = hdfsRead(fs, hdfsFile, buffer, bufferSize)) > 0) {
    fwrite(buffer, sizeof(char), bytesRead, localFile->identifier);
}
    struct File *file = openFile(localFilePath);
    readCsvFile(res, file, 3, 3, ',');
    closeFile(file);
    closeFile(localFile);

    hdfsCloseFile(fs, hdfsFile);
    hdfsDisconnect(fs);

    }
    else{
      ;

    }

  }
};

