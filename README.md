# File-IO-Buffering
A microbenchmark to evaluate disk write I/O performance.

Writing data to the disk goes throw the follwing I/O data path:
1. User Buffer (UB):
   
   ```char *buf = malloc(256);```
   
2. STDIO Buffer (SB):
   
   This is inside the libc layer, i.e, printf(), fwrite(), fputc() ..etc.
   The data is copied from the user buffer to the stdio buffer and stays here until it is flushed later to the kernel buffer or the kernel page cache.  the movement from the SB to the KPC is triggered by new line, the buffer is full, program exits or by using `fflush()`
   
3. Kernel Page Cache (KPC), the kernel memory:
   
   used by I/O system calls, such as `read()` and `write()`. The data that stays on these pages is not yet on disk and these pages are marked as `Dirty Pages`
   
7. Disk (D):  storage device:
  
   When using `fsync(fd)`, `sync()` or `fdatasync(fd)`, the data will be forced to be directly moved from the KPC to the disk.

| Operation        | Data Movement   |
| ---------------- | --------------- |
| `printf()`       | UB → SB         |
| `fflush()`       | SB → KPC        |
|`fwrite()+fflush()`|UB → SB → KPC   |
|`fwrite()+fflush()+fsync()`|UB → SB → KPC → Disk|
| `write()`        | UB → KPC        |
| `fsync()`        | KPC → Disk      |
| `write()+fsync(fd)`|UB → KPC → Disk|
| `O_DIRECT flag in write()` | UB → Disk       |
| `O_SYNC flag in write()`   | UB → KPC → Disk |


O_DIRECT vs O_SYNC in write():
| Feature            | O_SYNC          | O_DIRECT  |
| ------------------ | --------------- | --------- |
| Uses page cache    | ✔               | ✖         |
| Requires alignment | ✖               | ✔         |
| Write path         | UB → KPC → Disk | UB → Disk |


Create a build directory and run CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
After compilation the executable will be located at: `bin/fileio`. Examples: 
```
./fileio --mode=write --size-bytes=1048576

./fileio --mode=write_fsync --size-bytes=1048576

./fileio --mode=stdio --size-bytes=1048576

./fileio --mode=stdio_fsync --size-bytes=1048576

./fileio --mode=osync --size-bytes=1048576
./fileio --mode=odirect --size-bytes=1048576

```


