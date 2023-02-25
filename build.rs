extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:include=/home/pimeson/Development/RustDev/ChunkReader/chunk-reader-sys/C/chunk-reader/include");
    println!("cargo:rustc-link-search=/home/pimeson/Development/RustDev/ChunkReader/chunk-reader-sys/C/chunk-reader/build/lib");
    println!("cargo:rustc-link-lib=static=chunk-reader");

    println!("cargo:rerun-if-changed=wrapper.h");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("libchunk-reader.a")
        .clang_arg("-L/home/pimeson/Development/RustDev/ChunkReader/chunk-reader-sys/C/chunk-reader/build/lib")
        .clang_arg("-I/home/pimeson/Development/RustDev/ChunkReader/chunk-reader-sys/C/chunk-reader/include")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings");
}
