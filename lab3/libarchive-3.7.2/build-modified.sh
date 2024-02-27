sed -i 's/-Wall//g' ./CMakeLists.txt
sed -i 's/-Werror//g' ./CMakeLists.txt

mkdir -p build2
cd build2
cmake -DCHECK_CRC_ON_SOLID_SKIP=1 -DDONT_FAIL_ON_CRC_ERROR=1 ../
make -j$(nproc)


cp $SRC/contrib/oss-fuzz/corpus.zip\
        $OUT/libarchive_fuzzer_seed_corpus.zip

$CXX -I../libarchive \
	$SRC/harness.cpp -o $OUT/harness \
	$SRC/.libs/libarchive.a \
	-Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz /usr/lib/x86_64-linux-gnu/libxml2.a 

# find the libxml2.a path and replace it with the existing path
# find / -name 'libxml2.a'

cd .. # moving to src dir

mkdir -p input
find . -type f -name "test_extract.*.uu" -print0 | xargs -0 -I % cp -f % ./input/

cd ./input
find ./ -name "*.uu" -exec uudecode {} \;
cd ../
rm -f ./input/*.uu
zip -jr $OUT/libarchive_fuzzer_seed_corpus.zip ./input/*

git clone --depth=1 https://github.com/corkami/pocs
find ./pocs/ -type f -print0 | xargs -0 -I % zip -jr ./OUT/libarchive_fuzzer_seed_corpus.zip %


# harness file will be present inside OUT dir. 
# Inputs are created with pocs repo and present inside the input dir
# all the newly built executables are present inside build2/bin 
