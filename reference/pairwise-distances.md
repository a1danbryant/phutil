# Pairwise distances within a set of persistence diagrams

This collection of functions computes the pairwise distance matrix
between all pairs in a set of persistence diagrams of the same homology
dimension. The diagrams must be represented as 2-column matrices. The
first column of the matrix contains the birth times and the second
column contains the death times of the points.

## Usage

``` r
bottleneck_pairwise_distances(
  x,
  tol = sqrt(.Machine$double.eps),
  validate = TRUE,
  dimension = 0L,
  ncores = 1L
)

wasserstein_pairwise_distances(
  x,
  tol = sqrt(.Machine$double.eps),
  p = 1,
  validate = TRUE,
  dimension = 0L,
  ncores = 1L
)

kantorovich_pairwise_distances(
  x,
  tol = sqrt(.Machine$double.eps),
  p = 1,
  validate = TRUE,
  dimension = 0L,
  ncores = 1L
)
```

## Arguments

- x:

  A list of either 2-column matrices or objects of class
  [persistence](https://tdaverse.github.io/phutil/reference/persistence.md)
  specifying the set of persistence diagrams.

- tol:

  A numeric value specifying the relative error. Defaults to
  `sqrt(.Machine$double.eps)`. For the Bottleneck distance, it can be
  set to `0.0` in which case the exact Bottleneck distance is computed,
  while an approximate Bottleneck distance is computed if `tol > 0.0`.
  For the Wasserstein distance, it must be strictly positive.

- validate:

  A boolean value specifying whether to validate the input persistence
  diagrams. Defaults to `TRUE`. If `FALSE`, the function will not check
  if the input persistence diagrams are valid. This can be useful for
  performance reasons, but it is recommended to keep it `TRUE` for
  safety.

- dimension:

  An integer value specifying the homology dimension for which to
  compute the distance. Defaults to `0L`. This is only used if `x` and
  `y` are objects of class
  [persistence](https://tdaverse.github.io/phutil/reference/persistence.md).

- ncores:

  An integer value specifying the number of cores to use for parallel
  computation. Defaults to `1L`.

- p:

  A numeric value specifying the power for the Wasserstein distance.
  Defaults to `1.0`.

## Value

An object of class 'dist' containing the pairwise distance matrix
between the persistence diagrams.

## Examples

``` r
spl <- persistence_sample[1:10]

# Extract the list of 2-column matrices for dimension 0 in the sample
x <- lapply(spl[1:10], function(x) x$pairs[[1]])

# Compute the pairwise Bottleneck distances
Db <- bottleneck_pairwise_distances(spl)
Db <- bottleneck_pairwise_distances(x)

# Compute the pairwise Wasserstein distances
Dw <- wasserstein_pairwise_distances(spl)
Dw <- wasserstein_pairwise_distances(x)
```
