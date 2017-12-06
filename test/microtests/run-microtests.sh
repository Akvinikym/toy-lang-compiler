cd ../../build
cmake ../src
make
./voc -o ../test/microtests/test_out.o ../test/microtests/microtests.vo
cd ../test/microtests
cc -o main.out main.c test_out.o
./main.out