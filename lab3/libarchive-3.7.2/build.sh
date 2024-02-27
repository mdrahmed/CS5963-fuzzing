#export FUZZ_INTROSPECTOR_CONFIG=$SRC/fuzz_introspector_exclusion.config
#cat > $FUZZ_INTROSPECTOR_CONFIG <<EOF
#FILES_TO_AVOID
#libxml2
#EOF


sed -i 's/-Wall//g' ./CMakeLists.txt
sed -i 's/-Werror//g' ./CMakeLists.txt

mkdir OUT
mkdir build2
cd build2
cmake -DCHECK_CRC_ON_SOLID_SKIP=1 -DDONT_FAIL_ON_CRC_ERROR=1 ../
make -j$(nproc)

cp ../libarchive/contrib/oss-fuzz/corpus.zip \
    ../OUT/libarchive_fuzzer_seed_corpus.zip

#$CXX $CXXFLAGS -I../libarchive     ../libarchive_fuzzer.cc -o ../OUT/libarchive_fuzzer     $LIB_FUZZING_ENGINE ./libarchive/libarchive.a     -Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz ${DEPS}/libxml2.a

$CXX $CXXFLAGS -I../libarchive     ../harness.cpp -o ../OUT/harness     $LIB_FUZZING_ENGINE ./libarchive/libarchive.a     -Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz ../deps/libxml2.a

cd .. # moving to src dir

mkdir ./uudecoded
find . -type f -name "test_extract.*.uu" -print0 | xargs -0 -I % cp -f % ./uudecoded/
# find . -type f -name "test_extract.*.uu" -print0 | xargs -0 -I % cp -f % ./uudecoded/

cd ./uudecoded
find ./ -name "*.uu" -exec uudecode {} \;
cd ../
rm -f ./uudecoded/*.uu
zip -jr ./OUT/libarchive_fuzzer_seed_corpus.zip ./uudecoded/*

# add weird archives
git clone --depth=1 https://github.com/corkami/pocs
find ./pocs/ -type f -print0 | xargs -0 -I % zip -jr ./OUT/libarchive_fuzzer_seed_corpus.zip %
