#!/bin/bash
make;
mkdir -p test_file_stage.d;

./bin/vslc -s 12 < vsl_programs/$1.vsl 2> test_file_stage.d/$1.s

echo "-- ASSEMBLY --"
cat -n test_file_stage.d/$1.s

arm-linux-gnueabi-gcc -static test_file_stage.d/$1.s -o test_file_stage.d/$1.exe 2> test_file_stage.d/$1.asmError;

echo ""
echo "-- ASSEMBLER ERROR --"
cat test_file_stage.d/$1.asmError;

echo ""
echo "-- PROGRAM OUTPUT --"
qemu-arm test_file_stage.d/$1.exe > test_file_stage.d/$1.output
cat test_file_stage.d/$1.output
