# endre_test
This is an implemenation in C++ of one of the techniques of ENDRE, the redundancy elimination scheme presentation at NSDI 2010. Although ENDRE was used primarily with traffic reduction on enterprise networks, we move ENDRE into the context of utilizing it to increase efficiency for live migration of virtual machines. We seek specifically to reduce bandwidth consumption/latency, with as little CPU and memory overhead as possible.
This project was completed over the summer as a student research project.

## Install Before Running
* Install mininet here http://mininet.org/download/
* Install ZSTD here https://github.com/facebook/zstd
* Install ZLIB here https://github.com/madler/zlib
* Get fmem here https://github.com/NateBrune/fmem

## How to Run
* Compile with __make__
* Generate input using either script
* Run mininet's basic topology with __sudo mn -x__
* Run the server program on h1
* Run the client program on h2
* Enter 0, 1, 2, or 3 on both client and server (Must Match) to select No Compression, RLE, ZSTD, or ZLIB
* Wait for result

## Future Work
1. Port all code to C, since this project was eventually meant to be applied to QEMU.
This would mean that many of the data structures and functions in the C++ standard library would no longer be usuable, and substitutions would need to be found.
2. Make the running process less of a headache.
3. Work with the more advanced API of ZSTD and ZLIB to allow for even more finer grained optimization of the program.
4. Create a more accurate representation of memory pages being transferred by the migration process
5. Test performance on mininet's wireless simulator
