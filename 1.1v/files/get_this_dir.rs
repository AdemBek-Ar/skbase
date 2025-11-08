use std::ffi::CString;
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn ___ucc_rust_get_executable_dir___() -> *mut c_char {
    match std::env::current_exe() {
        Ok(path) => {
            let dir = path.parent().unwrap_or_else(|| std::path::Path::new("."));
            let c_str = CString::new(dir.to_string_lossy().to_string()).unwrap();
            c_str.into_raw()
        }
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn free_rust_string(ptr: *mut c_char) {
    if ptr.is_null() { return; }
    unsafe { drop(CString::from_raw(ptr)); }
}

