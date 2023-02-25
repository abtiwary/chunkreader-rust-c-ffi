#ifndef _CHUNK_READER_H_
#define _CHUNK_READER_H_

#include <stdio.h>

typedef enum {
    CHUNK_READER_ERROR_NONE,
    CHUNK_READER_ERROR_INVALID_INSTANCE,
    CHUNK_READER_ERROR_INVALID_INPUTS,
    CHUNK_READER_ERROR_INVALID_OUTPUT_FILE,
    CHUNK_READER_ERROR_EOF,
} ChunkReaderError;

typedef struct {
    char* input_file_path;
    int chunk_size;
    int chunk_number;
    FILE*  input_file_handle;
    char* last_output_file;
    char* last_chunk;
    int last_bytes_read;
    int last_bytes_written;
    int eof_reached;
} ChunkReader;

ChunkReader* ChunkReader_init(
        char* input_file_path,
        int input_file_path_size,
        int chunk_size
);

void ChunkReader_free(ChunkReader* creader);

ChunkReaderError next_chunk_into_file(
        ChunkReader* creader,
        char* output_file_path,
        int output_file_path_size
);

ChunkReaderError next_chunk_into_buffer(ChunkReader* creader, char* output_buffer, int size);

ChunkReaderError skip_a_chunk(ChunkReader* creader);

#endif // _CHUNK_READER_H_
