cppcheck --enable=all --inconclusive --xml-version=2 -v ./src/evm/src/*.cpp -I ./src/evm/include --output-file=out/cppcheck_results.xml