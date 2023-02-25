use std::ffi::CString;
use anyhow::{anyhow, Result};

#[allow(non_upper_case_globals)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[allow(dead_code)]
pub mod chunk_reader_ffi {
    include!("bindings.rs");
}

#[non_exhaustive]
#[derive(Clone, Debug)]
pub struct ChunkReader {
    input_file: String,
    chunk_size: u32,
    chunk_reader_handle: *mut chunk_reader_ffi::ChunkReader,
}


impl ChunkReader {

    pub fn new(input_file_path: &str, chunk_size: u32) -> Result<Self> {
        unsafe {
            let c_path = CString::new(input_file_path.as_bytes())?;
            let handle = chunk_reader_ffi::ChunkReader_init(
                c_path.into_raw(), 
                input_file_path.len() as i32, 
                chunk_size as i32
            );

            Ok(Self { 
                input_file: String::from(input_file_path), 
                chunk_size, 
                chunk_reader_handle: handle
            })
        }
    }

    pub fn next_chunk_to_file(&self, out_file_path: &str) -> Result<i32> {
        unsafe {
            let c_path = CString::new(out_file_path.as_bytes())?;
            let handle = self.chunk_reader_handle as *mut chunk_reader_ffi::ChunkReader;
            let res = chunk_reader_ffi::next_chunk_into_file(
                handle, 
                c_path.into_raw(), 
                out_file_path.len() as i32
            );

            if res != chunk_reader_ffi::ChunkReaderError_CHUNK_READER_ERROR_NONE {
                return Err(anyhow!("an error occurred"));
            } else {
                return Ok((*(self.chunk_reader_handle)).last_bytes_written);
            }
        }
    }

    pub fn next_chunk_to_buffer(&self, buf: &mut [u8]) -> Result<i32> {
        unsafe {
            let handle = self.chunk_reader_handle as *mut chunk_reader_ffi::ChunkReader;
            let res = chunk_reader_ffi::next_chunk_into_buffer(
                handle,
                buf.as_mut_ptr() as *mut ::std::os::raw::c_char,
                buf.len() as i32
            );
            
            if res != chunk_reader_ffi::ChunkReaderError_CHUNK_READER_ERROR_NONE {
                return Err(anyhow!("an error occurred"));
            } else {
                return Ok((*(self.chunk_reader_handle)).last_bytes_written);
            }
        }
    }

    pub fn skip_a_chunk(&self) -> Result<()> {
        unsafe {
            let handle = self.chunk_reader_handle as *mut chunk_reader_ffi::ChunkReader;
            let res = chunk_reader_ffi::skip_a_chunk(handle);
            
            if res != chunk_reader_ffi::ChunkReaderError_CHUNK_READER_ERROR_NONE {
                return Err(anyhow!("an error occurred"));
            } else {
                return Ok(());
            }
        }
    }

}

impl Drop for ChunkReader {
    fn drop(&mut self) {
        unsafe {
            let handle = self.chunk_reader_handle as *mut chunk_reader_ffi::ChunkReader;
            chunk_reader_ffi::ChunkReader_free(handle);
        }
    }
}


/* 
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(4, 4);
    }
}
*/
