#include <stdlib.h>
#include <string.h>
#include "chunk-reader.h"

ChunkReader* ChunkReader_init(
        char* input_file_path,
        int input_file_path_size,
        int chunk_size
) {
    ChunkReader* creader = (ChunkReader*)malloc(sizeof(ChunkReader));
    if (NULL == creader) return NULL;

    creader->input_file_path = (char*) malloc(input_file_path_size * sizeof(char));
    strcpy(creader->input_file_path, input_file_path);

    creader->input_file_handle  = fopen(creader->input_file_path, "rb");

    creader->chunk_size = chunk_size;
    creader->chunk_number = 0;
    creader->last_output_file = NULL;
    creader->last_bytes_read = 0;
    creader->eof_reached = 0;
    creader->last_bytes_written = 0;

    creader->last_chunk = (char*) malloc(chunk_size * sizeof(char));
}

void ChunkReader_free(ChunkReader* creader) {
    if (NULL != creader) {
        if (NULL != creader->last_chunk) free(creader->last_chunk);
        if(NULL != creader->input_file_handle) fclose(creader->input_file_handle);
        if(NULL != creader->input_file_path) free(creader->input_file_path);

        free(creader);
    }
}

ChunkReaderError next_chunk_into_file(
    ChunkReader* creader,
    char* output_file_path,
    int output_file_path_size
 ) {
    if (NULL == creader) return CHUNK_READER_ERROR_INVALID_INSTANCE;
    if (1 == creader->eof_reached) return CHUNK_READER_ERROR_EOF;
    if ((NULL == output_file_path) || (0 == output_file_path_size)) return CHUNK_READER_ERROR_INVALID_INPUTS;
    char* output_path = (char*)malloc(output_file_path_size * sizeof(char));
    strcpy(output_path, output_file_path);

    FILE* output_file = fopen(output_path, "w+");
    if (NULL == output_file) return CHUNK_READER_ERROR_INVALID_OUTPUT_FILE;

    creader->last_bytes_read = fread(
            creader->last_chunk,
            1 * sizeof(char),
            creader->chunk_size,
            creader->input_file_handle
    );

    creader->last_bytes_written = fwrite(creader->last_chunk, 1, creader->last_bytes_read, output_file);
    creader->chunk_number++;
    if (feof(creader->input_file_handle)) creader->eof_reached = 1;
    return CHUNK_READER_ERROR_NONE;
}

ChunkReaderError next_chunk_into_buffer(ChunkReader* creader, char* output_buffer, int size) {
    if (NULL == creader) return CHUNK_READER_ERROR_INVALID_INSTANCE;
    if (1 == creader->eof_reached) return CHUNK_READER_ERROR_EOF;
    if ((NULL == output_buffer) || (0 == size)) return CHUNK_READER_ERROR_INVALID_INPUTS;

    creader->last_bytes_read = fread(
            creader->last_chunk,
            1 * sizeof(char),
            creader->chunk_size,
            creader->input_file_handle
    );

    memcpy(output_buffer, creader->last_chunk, size);\
    creader->last_bytes_written = size;
    creader->chunk_number++;
    if (feof(creader->input_file_handle)) creader->eof_reached = 1;
    return CHUNK_READER_ERROR_NONE;
}

ChunkReaderError skip_a_chunk(ChunkReader* creader) {
    if (NULL == creader) return CHUNK_READER_ERROR_INVALID_INSTANCE;
    if (1 == creader->eof_reached) return CHUNK_READER_ERROR_EOF;

    long int curr_pos = ftell(creader->input_file_handle);
    fseek(creader->input_file_handle, creader->chunk_size, SEEK_CUR);
    long int new_pos = ftell(creader->input_file_handle);

    return CHUNK_READER_ERROR_NONE;
}
