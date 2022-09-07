# Big Data Benchmarks

This is a set of three "big data" benchmarks. The benchmarks utilize three very common statistics/machine learning techniques, namely principal components analysis (PCA), k-means clustering, and support vector machine (SVM). This gives one dimension reduction technique, one unsupervised technique, and one supervised technique.

For each benchmark, the data must be distributed across ranks by row, and is assumed to be taller than it is wide. Each uses MPI for communication.

Of the three benchmarks, all are available as parallel R codes, and one (PCA) is additionally available as a standalone C++ code. For each of the R benchmarks, you will need to install install the kazaam package and all of its dependencies which are available in the `source/` tree. To install the C++ code (for the PCA benchmark), it should be enough to just run `make`. See `source/README` for details.

We have tried to keep the benchmarks as vendor agnostic as possible by trying to allow a great deal of flexibility in how the individual benchmark problems are solved.  Each vendor is encouraged to use this flexibility to their particular advantage.



## Running the Bencharks

Each of the benchmarks (R and C++ alike) accepts a number of *local* rows and *global* columns. The total number of rows grows proportionally to the number of MPI ranks. Thus, each benchmark measures scaling in the weak sense.

Run the C++ code from the project root via:
```
mpirun -np num_ranks ./source/cxx/build/princomp num_local_rows num_global_cols
```
Run the R code via:
```
mpirun -np num_ranks Rscript princomp.r num_local_rows num_global_cols
```
For example, to run the MPI code with 16 ranks, 50 total columns, and a total of 16,000 rows (local number of rows 1000), you would run:
```
mpirun -np 16 princomp 1000 50
```

For example, to generate 1 GB local sizes with 100 columns, you would need to specify 1250000 local rows. For a 1 GiB local size with 100 columns, you would need 1342177 local rows.



## Benchmark Details

Below we describe the benchmarks in detail.

### PCA

The benchmark is defined as computing the first and last of the "standard deviations" from a PCA on a distributed matrix by way of taking the square roots of the eigenvalues of the covariance matrix. The "first and last" requirement is to avoid approximate methods. Using the covariance matrix is mathematically equivalent to computing the SVD of the mean-centered input matrix, although it is computationally easier. The data should be random normal.

Unlike the other benchmarks, the PCA benchmark is available in both R and C++. Our implementation of the R benchmark can be found at `benchmarks/r/kmeans.r`. The C++ benchmark is self-contained; simply run the executable generated from running `make` in `source/cxx/`.

### k-means

The benchmark is defined by computing the observation labels (class assignments) and centroids for k=2, 3, and 4. The data should consist of rows sampled from one of 3 random normal distributions: one with mean 0, one with mean 2, and one with mean 10. Each should have variance 1. The rows should be drawn at random from these distributions.

Our implementation of the benchmark can be found at `benchmarks/r/kmeans.r`.

### SVM

The benchmark consists of a linear 2-class SVM fit using 500 iterations, calculating the feature weights. The data should consist of an intercept term together with rows sampled from one of 2 random normal distributions: one with mean 0, and one with mean 2; each should have variance 1. The rows should be drawn at random from these distributions.  The SVM is not expected to converge given the number of features and iterations; this allows for easier comparisons of benchmarks as data sizes/layouts change.

Our implementation of the benchmark can be found at `benchmarks/r/svm.r`.



## Validation

We use random data in the benchmarks in order to keep the benchmarks as amenable to every hardware solution possible.  We strongly believe this approach is to the advantage of every vendor.  However, this makes the benchmark runs difficult to verify.  So we have included small verification scripts to be run in companion with the benchmarks themselves.  

It would be possible to do something underhanded (fast but incorrect) on the large benchmarks while passing the small scale tests.  But remember that we have chosen this path for the vendor's benefit.  Please do not betray that trust.

Each validation script should be run on two nodes and use the same (specified) kernel as its benchmark counterpart.  Each will use the famous iris dataset of R. A. Fisher (included).  The rows of the dataset have been randomly shuffled and the species variable has been coded to 1=setosa, 2=versicolor, and 3=virginica.  Any other settings we leave to the vendor.  Performance measurements are not desired, only the validation.

Each of the validation scripts is completely self-contained.  You can run any of them via:

```
mpirun -np 2 Rscript $BENCHMARK.r
```

### SVD

This validation script shows that the PCA benchmark is working correctly by testing the SVD kernel.  The validation consists of reading the iris dataset, removing the species column, factoring the resulting matrix, and then multiplying the factored matrices (from SVD) back together.  The mean absolute error (average of the difference in absolute value) of the two matrices should be computed.  The test passes if this value is less than the square root of machine epsilon for each type (as specified by IEEE 754).

Our implementation of the validation script can be found at `validation/r/svd.r`.

### k-means

Using k=3 centroids (the true value), and 100 starts using seeds 1 to 100, the labels for each observation should be computed.  These will be compared against the true values (from the 'species' label of the dataset) using [rand measure](https://en.wikipedia.org/wiki/Rand_index).  Take the largest among these values.  This should be greater than 75% to be considered successful.

Our implementation of the validation script can be found at `validation/r/kmeans.r`.

### SVM

The data consists of an intercept term together with the iris dataset without the species column.  The response should be taken to be 1 for setosa (coding 1 in the species variable) and -1 otherwise.  Use a maximum of 500 iterations to fit an SVM on the data and response.  Report the accuracy (number correctly predicted).  This should be greater than 80%.

Our implementation of the validation script can be found at `validation/r/svm.r`.
