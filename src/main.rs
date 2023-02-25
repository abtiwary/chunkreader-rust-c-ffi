use chunk_reader_sys::ChunkReader;

fn main() {
    let file_path = String::from("/home/pimeson/Development/RustDev/ChunkReader/gutenberg_alice.txt");
    let creader = ChunkReader::new(
        &file_path,
        500
    ).unwrap();

    let temp_file_1 = String::from("/home/pimeson/temp/temp_out_rust_ffi_1");
    creader.next_chunk_to_file(&temp_file_1).unwrap();

    creader.skip_a_chunk().unwrap();

    let temp_file_2 = String::from("/home/pimeson/temp/temp_out_rust_ffi_2");
    creader.next_chunk_to_file(&temp_file_2).unwrap();

    let mut buf = vec![0u8; 500usize];
    creader.next_chunk_to_buffer(&mut buf).unwrap();
    println!("{}", String::from_utf8(buf).unwrap());

    //creader.skip_a_chunk().unwrap();

    let mut buf2 = vec![0u8; 500usize];
    creader.next_chunk_to_buffer(&mut buf2).unwrap();
    println!("{}", String::from_utf8(buf2).unwrap());
}
