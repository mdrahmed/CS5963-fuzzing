# Functions to fuzz
`pathcmp` is used by `bsdtar` to check the path.


## Install libarchive
### Install it from zip file
Download the file from the [libarchive site](https://www.libarchive.org/). I downloaded this [tar.gz](https://github.com/libarchive/libarchive/tarball/master)

Now, just execute following,
```
tar xzf libarchive-XX.YY.ZZ.tar.gz
cd libarchive-XX.YY.ZZ
./configure
make
make check
make install
```
### Install it from Source
Install the dependencies,
```
sudo apt update
sudo apt install libarchive-dev tar cpio unzip
sudo apt install libtool
```

You will need Autoconf version 2.71 or higher. So, check the version with this `autoconf --version`.
Clone the repo and follow along,
```
git clone git@github.com:libarchive/libarchive.git
build/autogen.sh
./configure
make
make check
make install
```

## Instrument with afl++
Compile again to instrument the codes,
```
export CC='afl-cc' 
export CXX='afl-cc++'

# or,
export CC='afl-clang-fast'
export CXX='afl-clang-fast++'

./configure
make
make check
# do not make install, these binaries are instrumented
```

After running make,
![instrumented-afl-cc](pics/instrumented-afl-cc.png)

It is creating the zip & tar files for testing,
![instrumented-check](pics/instrumented-check.png)

### Fuzz
Now, to fuzz it,
```
# download a tar file and put it to ./in folder
wget https://github.com/mdrahmed/dataVisProject/archive/refs/tags/v1.0.tar.gz
wget https://www.libarchive.org/downloads/libarchive-3.7.2.tar.gz
# create tar files
tar -cvf pdfs.tar.gz pdf_examples/

# Now, check if bsdtar is working or not
./bsdtar -tf ./in/<tar file>
# ./bsdtar -tf in/libarchive-3.7.2.tar.gz 

afl-fuzz -i in/ -D -o out -- ./bsdtar -tf @@
```

This is not the actual input file, need to generate the actual input file,
![bsdtar-fuzz1](pics/bsdtar-fuzz1.png)


