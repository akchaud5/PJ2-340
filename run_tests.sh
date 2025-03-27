#!/bin/bash

# For use in a Linux environment or after fixing the macOS compilation issues

# Compile the program
g++ -std=c++11 -o a.out project2.cc inputbuf.cc lexer.cc -I /Library/Developer/CommandLineTools/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/
if [ $? -ne 0 ]; then
  echo "Compilation failed. Please use a Linux environment or fix the macOS compilation issues."
  exit 1
fi

# Create output directory
mkdir -p output

# Run tests for Task 1
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected1"
  output_file="output/${test_name}_task1.out"
  
  echo "Testing Task 1 with ${test_name}..."
  ./a.out 1 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done

# Run tests for Task 2
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected2"
  output_file="output/${test_name}_task2.out"
  
  echo "Testing Task 2 with ${test_name}..."
  ./a.out 2 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done

# Run tests for Task 3
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected3"
  output_file="output/${test_name}_task3.out"
  
  echo "Testing Task 3 with ${test_name}..."
  ./a.out 3 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done

# Run tests for Task 4
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected4"
  output_file="output/${test_name}_task4.out"
  
  echo "Testing Task 4 with ${test_name}..."
  ./a.out 4 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done

# Run tests for Task 5
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected5"
  output_file="output/${test_name}_task5.out"
  
  echo "Testing Task 5 with ${test_name}..."
  ./a.out 5 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done

# Run tests for Task 6
for test_file in tests/test*.txt; do
  test_name=$(basename $test_file .txt)
  expected_file="${test_file}.expected6"
  output_file="output/${test_name}_task6.out"
  
  echo "Testing Task 6 with ${test_name}..."
  ./a.out 6 < $test_file > $output_file
  
  if [ -f "$expected_file" ]; then
    if diff -w "$expected_file" "$output_file" > /dev/null; then
      echo "  PASSED"
    else
      echo "  FAILED"
      echo "  Expected:"
      cat "$expected_file"
      echo "  Got:"
      cat "$output_file"
    fi
  else
    echo "  No expected output file found"
  fi
done