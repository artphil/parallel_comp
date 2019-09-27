#!/bin/bash


echo sequential 1M
echo sequential 1M 2T >> result_tests.txt
./sequential_select 1000000 100 t 2 >> result_tests.txt
./sequential_select 1000000 100 t 2 >> result_tests.txt
./sequential_select 1000000 100 t 2 >> result_tests.txt
echo sequential 10M
echo sequential 10M 2T >> result_tests.txt
./sequential_select 10000000 100 t 2 >> result_tests.txt
./sequential_select 10000000 100 t 2 >> result_tests.txt
./sequential_select 10000000 100 t 2 >> result_tests.txt
echo sequential 100M
echo sequential 100M 2T >> result_tests.txt
./sequential_select 100000000 100 t 2 >> result_tests.txt
./sequential_select 100000000 100 t 2 >> result_tests.txt
./sequential_select 100000000 100 t 2 >> result_tests.txt
echo sequential 1B
echo sequential 1B 2T >> result_tests.txt
./sequential_select 1000000000 100 t 2 >> result_tests.txt
./sequential_select 1000000000 100 t 2 >> result_tests.txt
./sequential_select 1000000000 100 t 2 >> result_tests.txt


echo Parallel 1M
echo Parallel 1M 2T >> result_tests.txt
./parallel_select 1000000 100 t 2 >> result_tests.txt
./parallel_select 1000000 100 t 2 >> result_tests.txt
./parallel_select 1000000 100 t 2 >> result_tests.txt
echo parallel 1M 4T >> result_tests.txt
./parallel_select 1000000 100 t 4 >> result_tests.txt
./parallel_select 1000000 100 t 4 >> result_tests.txt
./parallel_select 1000000 100 t 4 >> result_tests.txt
echo parallel 1M 8T >> result_tests.txt
./parallel_select 1000000 100 t 8 >> result_tests.txt
./parallel_select 1000000 100 t 8 >> result_tests.txt
./parallel_select 1000000 100 t 8 >> result_tests.txt
echo parallel 10M
echo parallel 10M 2T >> result_tests.txt
./parallel_select 10000000 100 t 2 >> result_tests.txt
./parallel_select 10000000 100 t 2 >> result_tests.txt
./parallel_select 10000000 100 t 2 >> result_tests.txt
echo parallel 10M 4T >> result_tests.txt
./parallel_select 10000000 100 t 4 >> result_tests.txt
./parallel_select 10000000 100 t 4 >> result_tests.txt
./parallel_select 10000000 100 t 4 >> result_tests.txt
echo sequencial 10M 8T >> result_tests.txt
./parallel_select 10000000 100 t 8 >> result_tests.txt
./parallel_select 10000000 100 t 8 >> result_tests.txt
./parallel_select 10000000 100 t 8 >> result_tests.txt
echo parallel 100M
echo parallel 100M 2T >> result_tests.txt
./parallel_select 100000000 100 t 2 >> result_tests.txt
./parallel_select 100000000 100 t 2 >> result_tests.txt
./parallel_select 100000000 100 t 2 >> result_tests.txt
echo parallel 100M 4T >> result_tests.txt
./parallel_select 100000000 100 t 4 >> result_tests.txt
./parallel_select 100000000 100 t 4 >> result_tests.txt
./parallel_select 100000000 100 t 4 >> result_tests.txt
echo parallel 100M 8T >> result_tests.txt
./parallel_select 100000000 100 t 8 >> result_tests.txt
./parallel_select 100000000 100 t 8 >> result_tests.txt
./parallel_select 100000000 100 t 8 >> result_tests.txt
echo parallel 1B
echo parallel 1B 2T >> result_tests.txt
./parallel_select 1000000000 100 t 2 >> result_tests.txt
./parallel_select 1000000000 100 t 2 >> result_tests.txt
./parallel_select 1000000000 100 t 2 >> result_tests.txt
echo parallel 1B 4T >> result_tests.txt
./parallel_select 1000000000 100 t 4 >> result_tests.txt
./parallel_select 1000000000 100 t 4 >> result_tests.txt
./parallel_select 1000000000 100 t 4 >> result_tests.txt
echo parallel 1B 8T >> result_tests.txt
./parallel_select 1000000000 100 t 8 >> result_tests.txt
./parallel_select 1000000000 100 t 8 >> result_tests.txt
./parallel_select 1000000000 100 t 8 >> result_tests.txt
