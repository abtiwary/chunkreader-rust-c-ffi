#include <stdlib.h>
#include <string.h>

#include "chunk-reader.h"

int main(int argc, char** argv) {
    char* input_file_path = "/home/pimeson/Development/RustDev/ChunkReader/gutenberg_alice.txt";

    ChunkReader* rdr = ChunkReader_init(input_file_path, strlen(input_file_path), 500);

    if (NULL == rdr) exit(-1);

    char* temp_out_1 = "/home/pimeson/temp/temp_out_1.txt";
    next_chunk_into_file(rdr, temp_out_1, strlen(temp_out_1));

    skip_a_chunk(rdr);

    char* temp_out_2 = "/home/pimeson/temp/temp_out_2.txt";
    next_chunk_into_file(rdr, temp_out_2, strlen(temp_out_2));

    skip_a_chunk(rdr);

    char* temp_chunk = (char*)malloc(rdr->chunk_size * sizeof(char ));
    next_chunk_into_buffer(rdr, temp_chunk, rdr->chunk_size * sizeof(char ));

    ChunkReader_free(rdr);

    return 0;
}