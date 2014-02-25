#!/bin/bash
make;
mkdir -p test_file_stage.d;
cp "correct_output/$1.s$2" "test_file_stage.d/$1.s$2"

./bin/vslc -s $2 < vsl_programs/$1.vsl 2> test_file_stage.d/$1.test.s$2

echo ""
echo "-- DIFF --"
diff test_file_stage.d/$1.s$2 test_file_stage.d/$1.test.s$2 | cat

echo ""
echo ""
echo "-- CORRECT OUTPUT --"
cat test_file_stage.d/$1.s$2

echo ""
echo ""
echo "-- YOUR OUTPUT --"
cat test_file_stage.d/$1.test.s$2

