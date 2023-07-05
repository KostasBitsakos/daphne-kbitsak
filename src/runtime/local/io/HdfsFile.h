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

#ifndef SRC_RUNTIME_LOCAL_IO_FILE_H
#define SRC_RUNTIME_LOCAL_IO_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <hdfs.h>

struct File {
  hdfsFS fs;
  hdfsFile identifier;
  unsigned long pos;
  unsigned long read;
};

inline struct File *openMemFileHDFS(hdfsFile ident) {
  struct File *f = (struct File *)malloc(sizeof(struct File));

  f->identifier = ident;
  f->pos = 0;

  return f;
}

inline struct File *openFileHDFS(const char *filename, hdfsFS fs) {
  struct File *f = (struct File *)malloc(sizeof(struct File));

  hdfsFile file = hdfsOpenFile(fs, filename, O_RDONLY, 0, 0, 0);
  f->identifier = file;
  f->pos = 0;

  if (file == NULL)
    return NULL;
  return f;
}


inline struct File *openFileForWriteHDFS(const char *filename) {
  struct File *f = (struct File *)malloc(sizeof(struct File));

  f->fs = hdfsConnect(NULL, 0);
  f->identifier = hdfsOpenFile(f->fs, filename, O_WRONLY | O_CREAT, 0, 0, 0);
  f->pos = 0;

  if (f->identifier == NULL)
    return NULL;
  return f;
}

inline void closeFileHDFS(File *f) {
  hdfsCloseFile(f->fs, f->identifier);
  hdfsDisconnect(f->fs);
}

inline char *getLineHDFS(File *f) {
  char *line = NULL;
  int bufferSize = 1024;  // Adjust buffer size as per your requirements

  char buffer[bufferSize];
  int bytesRead = hdfsRead(f->fs, f->identifier, buffer, bufferSize);
  if (bytesRead > 0) {
    line = (char *)malloc(bytesRead + 1);
    memcpy(line, buffer, bytesRead);
    line[bytesRead] = '\0';
    f->read = bytesRead;
    f->pos += bytesRead;
  }

  return line;
}
#endif