#!/bin/bash

# Test Task 1
echo "Testing Task 1 with test01.txt"
OUTPUT=$(cat tests/test01.txt | ./a.out 1)
EXPECTED=$(cat tests/test01.txt.expected1)

echo "Output:   '$OUTPUT'"
echo "Expected: '$EXPECTED'"

if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo "Test 1 PASSED"
else
    echo "Test 1 FAILED"
fi