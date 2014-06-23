# About

Originally from: http://huonw.github.io/2014/06/10/knn-rust.html

# GOALLLL

Until I lure someone to implement node.js version and put it on npm for profit.

# Explanation

There are two csv files:

* trainingsample.csv
* validationsample.csv

Format is:

* First row is header.
* Given a data row (not header),
    * First column is number that the image is representing.
    * The rest of columns are pixels of the image.

For each image (row) in `validationsample.csv`, find the best matching image in `trainingsample.csv`.
And compare the number (first column).

For example, 5th image of `validationsample.csv` is 3. And if you found the best matching image from `trainingsample.csv`, 
its label should be 3.

Correct answer should be `94.4%`. There are 500 images in `validationsample.csv`. 472 of them are _correctly_ matched against 5000 images in `trainingsample.csv`.

# Summary

    $ ./run.bash 
    C single
    Compiling for 1 thread(s).
    gcc knn.c -DNUM_THREADS=1 -pthread -O3 -std=c99 -Wall -pedantic -o knn
    matched: 472/500 : 94.4%
    time:    0.910000s
    ----
    C multi
    Compiling for 6 thread(s).
    gcc knn.c -DNUM_THREADS=6 -pthread -O3 -std=c99 -Wall -pedantic -o knn
    matched: 472/500 : 94.4%
    time:    0.163000s
    ----
    Java single
    94.4% Took: 1.5390 secs
    ----
    Rust single
    Percentage correct: 94.4% Took: 3.707468
    ----
    Rust multi
    Percentage correct: 94.4% Took: 0.734235
    ----
    Go multi
    named files must be .go files
    Percentage correct: 94.4% Took: 0.732868
    ----
    Python single numpy (lapack)
    94.4
    elapsed time: 5.43683695793
    ----
    Python multi
    count: 500 match: 472
    94.4% Took: 0:00:40.917040


- C fast.
- Python slow.
- Go and Rust similar.
- But it's all meaningless because node.js will beat them all once written utilizing mongodb webscale power.

# pseudo code

    validationsample.csv = parsed from file
    trainingsample.csv = parsed from file

    t = now()
    good_match_count = 0
    for unknown in validationsample.csv:
        match = null
        min_distance = MAX_DISTANCE
        for known in trainingsample.csv:
            d = distance(unknown, known)
            if d < min_distance:
                min_distance = d
                match = known
        if match.label == unknown.label:
            good_match_count++

    print(good_match_count/validationsample.csv.length)
    print(now() - t)


    function distance(pixels1, pixels2):
        result = 0
        for i in 0 to pixels1.length:
            d = pixels1[i]-pixels2[i]
            result += d*d
        return result

# vpenis

Host 'sam-HP-Z400', running Linux 3.13.0-29-generic - Cpu0: Intel 3326 MHz Cpu1: Intel 1596 MHz Cpu2: Intel 2793 MHz Cpu3: Intel 1596 MHz Cpu4: Intel 1596 MHz Cpu5: Intel 1596 MHz; Up: 4d+3:30; Users: 20; Load: 0.47; Free: [Mem: 1990/11999 Mio] [Swap: 7340/7467 Mio] [/: 48537/172755 Mio] [/media/data: 51715/99297 Mio]; Vpenis: 486.6 cm;


# python

using all cores

    $ python knn.py
    count: 500 match: 472
    94.4% Took: 0:00:48.740370
    python knn.py  271.71s user 0.90s system 540% cpu 50.430 total
    
numpy single core

    $ python knn_numpy.py 
    94.4
    elapsed time: 16.4862749577

# rust

all cores

    $ rustc -O knn.rs
    $ ./knn
    Percentage correct: 94.4% Took: 0.754412
    ./knn  4.48s user 0.06s system 486% cpu 0.934 total
    
single thread

    $ rustc -O knn_single.rs    
    $ ./knn_single 
    Percentage correct: 94.4% Took: 3.718948
    ./knn_single  3.85s user 0.02s system 99% cpu 3.877 total

# c

single thread

    $ cd c
    $ ./knn -t ../trainingsample.csv -v ../validationsample.csv -n 6
    Training Samples ../trainingsample.csv
    Validation Samples ../validationsample.csv
    Number of CPUs 6
    846.00% Took 6.71 s
    ./knn -t ../trainingsample.csv -v ../validationsample.csv -n 6  6.90s user 0.01s system 99% cpu 6.924 total

multi thread

    $ cd c_rcr
    $ make threads=6
    $ ./knn
    matched: 472/500 : 94.4%
    time:    0.164000s
    
single thread
    
    $ cd c_rcr
    $ make threads=1
    $ ./knn
    matched: 472/500 : 94.4%
    time:    0.913000s


# go

single thread

    $ go build -o knn knn.go
    $ ./knn
    Match: 94.4% 
    Duration: 3.247531573s 
    ./knn  3.99s user 0.04s system 100% cpu 4.029 total
    
multi thread

    $ go build knn.go
    $ ./knn
    Match: 472 94.40% 
    Duration: 748.936676ms 
    ./knn  4.78s user 0.05s system 314% cpu 1.533 total
        
# java

single thread

    $ javac Knn.java
    $ java Knn
    94.4% Took: 1.5320 secs
    
