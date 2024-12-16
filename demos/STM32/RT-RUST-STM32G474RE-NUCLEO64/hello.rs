#![no_std]
#![no_main]
#![allow(unused)]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {

    loop {}
}

#[unsafe(no_mangle)]
extern "C" fn hello() {

//    println!("Hello, world!");
//    loop {}
}