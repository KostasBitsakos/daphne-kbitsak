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

#ifndef SRC_RUNTIME_LOCAL_IO_WRITEHDFSCSV_H
#define SRC_RUNTIME_LOCAL_IO_WRITEHDFSCSV_H

#include <hdfs.h>
#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>

#include <runtime/local/io/File.h>
#include <runtime/local/io/utils.h>

#include <type_traits>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <sstream>

// ****************************************************************************
// Struct for partial template specialization
// ****************************************************************************

template <class DTArg>
struct WriteHDFSCsv {
    static void apply(const DTArg *arg, const char *hdfsFilename) = delete;
};

// ****************************************************************************
// Convenience function
// ****************************************************************************

template <class DTArg>
void writeHDFSCsv(const DTArg *arg, const char *hdfsFilename) {
    WriteHDFSCsv<DTArg>::apply(arg, hdfsFilename);
}

// ****************************************************************************
// (Partial) template specializations for different data/value types
// ****************************************************************************

// ----------------------------------------------------------------------------
// DenseMatrix
// ----------------------------------------------------------------------------

template <typename VT>
struct WriteHDFSCsv<DenseMatrix<VT>> {
    static void apply(const DenseMatrix<VT> *arg, const char *hdfsFilename) {
        assert(hdfsFilename != nullptr && "File path required");

        const char* hdfsFilePath = hdfsFilename;
        const char* host = "hdfs://localhost";
        tPort port = 9000;  // Default port
        const char* user = "hdoop";

        hdfsFS fs = hdfsConnectAsUser(host, port, user);
            if (fs == NULL) {
        std::cout << "Error connecting to HDFS" << std::endl;
        }


        // Check if the file already exists
        // if (hdfsExists(fs, hdfsFilePath)==-1) {
        //     std::cout << "whaaat \n";
        //      // The file does not exist, so create the directory structure and the file
        // hdfsCreateDirectory(fs, hdfsFilePath); // Create the directory if needed
        // }

//         hdfsFileInfo* fileInfo = hdfsGetPathInfo(fs, hdfsFilePath);
// if (fileInfo == nullptr) {
//     // File or directory does not exist, create it
//     hdfsCreateDirectory(fs, hdfsFilePath);
//     hdfsFreeFileInfo(fileInfo, 1); // Clean up fileInfo
// }

//         if (shdfsExists(fs,  "/user/hdoop/new.csv")) {
//     std::cout << "File /user/hdoop/new.csv does not exist. Creating it...\n";
//     // Create the file or take further action
// } else {
//     std::cout << "File /user/hdoop/new.csv already exists.\n";
// }

        //Open the HDFS file for reading
        hdfsFile hdfsFile = hdfsOpenFile(fs, hdfsFilePath, O_WRONLY, 0, 0, 0);
        if (hdfsFile == NULL) {
            std::cout << "Error opening HDFS file" << std::endl;
            hdfsDisconnect(fs);
        }
        else {
             ; 
        }

    // const VT* valuesArg = arg->getValues();
    // size_t cell = 0;
    // for (size_t i = 0; i < arg->getNumRows(); ++i) {
    //     for (size_t j = 0; j < arg->getNumCols(); ++j) {
    //         std::ostringstream oss;
    //         oss << (std::is_floating_point<VT>::value ? "%f" : (std::is_same<VT, long int>::value ? "%ld" : "%d"))
    //             << (j < (arg->getNumCols() - 1) ? "," : "\n");
    //         const std::string& valueStr = oss.str();
    //         if (hdfsWrite(fs, hdfsFile, static_cast<const void*>(valueStr.c_str()), valueStr.size()) == -1) {
    //             hdfsCloseFile(fs, hdfsFile);
    //             hdfsDisconnect(fs);
    //             throw std::runtime_error("Failed to write to HDFS file");
    //         }
    //         cell++;
    //     }
    // }

  const VT* valuesArg = arg->getValues();
size_t cell = 0;
for (size_t i = 0; i < arg->getNumRows(); ++i) {
    for (size_t j = 0; j < arg->getNumCols(); ++j) {
        // Convert the numeric value to a string and add a comma
        std::ostringstream oss;
        oss << valuesArg[cell];
        oss << (j < (arg->getNumCols() - 1) ? "," : ""); // Add a comma unless it's the last column
        const std::string& valueStr = oss.str();

        // Write the value string to the HDFS file
        if (hdfsWrite(fs, hdfsFile, static_cast<const void*>(valueStr.c_str()), valueStr.size()) == -1) {
            hdfsCloseFile(fs, hdfsFile);
            hdfsDisconnect(fs);
            throw std::runtime_error("Failed to write to HDFS file");
        }
        cell++;
    }
    // Add a newline character at the end of each row
    const char newline = '\n';
    if (hdfsWrite(fs, hdfsFile, static_cast<const void*>(&newline), 1) == -1) {
        hdfsCloseFile(fs, hdfsFile);
        hdfsDisconnect(fs);
        throw std::runtime_error("Failed to write to HDFS file");
    }
}


// tOffset start = 0; // Start offset
//     tOffset length = 1024; // Length of data to fetch
//     int numOfBlock = 0;
    
//     // Call the hdfsGetFileBlockLocations function to get block locations
//     BlockLocation* blockLocations = hdfsGetFileBlockLocations(fs, hdfsFilePath, start, length, &numOfBlock);

//     // Check if block locations were successfully retrieved
//     if (blockLocations != NULL) {
//         // Iterate through the block locations and print them
//         for (int i = 0; i < numOfBlock; ++i) {
//             std::cout << "Block " << i << ":\n";
//             std::cout << "  Hosts: ";
//             for (int j = 0; j < blockLocations[i].numOfNodes; ++j) {
//                 std::cout << blockLocations[i].hosts[j] << " ";
//             }
//             std::cout << "\n";
//             std::cout << "  Offset: " << blockLocations[i].offset << "\n";
//             std::cout << "  Length: " << blockLocations[i].length << "\n";
//         }

//         // Free the block locations when you're done with them
//         hdfsFreeFileBlockLocations(blockLocations, numOfBlock);
//     } else {
//         // Handle the case where block locations retrieval failed
//         std::cerr << "Failed to retrieve block locations.\n";
//         // You can check errno for the specific error code
//     }




    // Close the HDFS file and disconnect
    if (hdfsCloseFile(fs, hdfsFile) == -1) {
        throw std::runtime_error("Failed to close HDFS file");
    }

    


    hdfsDisconnect(fs);
    }
};

    #endif  // SRC_RUNTIME_LOCAL_IO_WRITEHDFSCSV_H



//         const VT * valuesArg = arg->getValues();
//         size_t cell = 0;
//         for (size_t i = 0; i < arg->getNumRows(); ++i)
//         {
//             for(size_t j = 0; j < arg->getNumCols(); ++j)
//             {
//                 fprintf(
//                         file->identifier,
//                         std::is_floating_point<VT>::value ? "%f" : (std::is_same<VT, long int>::value ? "%ld" : "%d"),
//                         valuesArg[cell++]
//                 );
//                 if(j < (arg->getNumCols() - 1))
//                     fprintf(file->identifier, ",");
//                 else
//                     fprintf(file->identifier, "\n");
//             }
//         }
//    }
// };
  
// #endif // SRC_RUNTIME_LOCAL_IO_WRITEHDFSCSV_H

// #include <hdfs.h>
// #include <runtime/local/datastructures/DenseMatrix.h>
// #include <cassert>
// #include <cstddef>
// #include <cstdint>
// #include <sstream>
// #include <stdexcept>

// // HDFS configuration
// const char* hdfs_server = "hdfs://localhost:9000"; // Change this to your HDFS server address
// const char* hdfs_user = "your_hdfs_user"; // Change this to your HDFS username
// const char* hdfs_path = "/path/to/hdfs/directory/output.csv"; // Change this to your desired HDFS path

// // Function to write a DenseMatrix to an HDFS file
// template <typename VT>
// void writeCsvToHDFS(const DenseMatrix<VT>* arg) {
//     assert(arg != nullptr);

//     // Initialize HDFS
//     hdfsFS fs = hdfsConnect(hdfs_server, 0);
//     if (!fs) {
//         throw std::runtime_error("Failed to connect to HDFS");
//     }

//     // Create or open the HDFS file for writing
//     hdfsFile hdfsFile = hdfsOpenFile(fs, hdfs_path, O_WRONLY | O_CREAT, 0, 0, 0);
//     if (!hdfsFile) {
//         hdfsDisconnect(fs);
//         throw std::runtime_error("Failed to open HDFS file for writing");
//     }

//     // Write the matrix data to the HDFS file
//     const VT* valuesArg = arg->getValues();
//     size_t cell = 0;
//     for (size_t i = 0; i < arg->getNumRows(); ++i) {
//         for (size_t j = 0; j < arg->getNumCols(); ++j) {
//             std::ostringstream oss;
//             oss << (std::is_floating_point<VT>::value ? "%f" : (std::is_same<VT, long int>::value ? "%ld" : "%d"))
//                 << (j < (arg->getNumCols() - 1) ? "," : "\n");
//             const std::string& valueStr = oss.str();
//             if (hdfsWrite(fs, hdfsFile, static_cast<const void*>(valueStr.c_str()), valueStr.size()) == -1) {
//                 hdfsCloseFile(fs, hdfsFile);
//                 hdfsDisconnect(fs);
//                 throw std::runtime_error("Failed to write to HDFS file");
//             }
//             cell++;
//         }
//     }

//     // Close the HDFS file and disconnect
//     if (hdfsCloseFile(fs, hdfsFile) == -1) {
//         throw std::runtime_error("Failed to close HDFS file");
//     }
//     hdfsDisconnect(fs);
// }

// int main() {
//     // Create a DenseMatrix<YourValueType> object
//     DenseMatrix<YourValueType> matrix;
//     // Populate the matrix with data

//     try {
//         writeCsvToHDFS(&matrix);
//         std::cout << "Matrix written to HDFS successfully." << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
