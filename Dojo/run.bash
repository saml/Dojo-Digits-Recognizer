#!/bin/bash

echo "C single"
cd c_rcr/
rm -rf knn
make threads=1
./knn
echo "----"

echo "C multi"
rm -rf knn
make threads=6
./knn
echo "----"

echo "Java single"
cd ..
javac Knn.java
java Knn
echo "----"

echo "Rust single"
rustc -O knn_single.rs -o knn
./knn
echo "----"

echo "Rust multi"
rustc -O knn.rs -o knn
./knn
echo "----"

echo "Go multi"
go build knn.go
./knn
echo "----"

echo "Python single numpy (lapack)"
python knn_numpy.py
echo "----"

echo "Python multi"
python knn.py
echo "----"

