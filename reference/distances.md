# Distances between two persistence diagrams

This collection of functions computes the distance between two
persistence diagrams of the same homology dimension. The diagrams must
be represented as 2-column matrices. The first column of the matrix
contains the birth times and the second column contains the death times
of the points.

## Usage

``` r
bottleneck_distance(
  x,
  y,
  tol = sqrt(.Machine$double.eps),
  validate = TRUE,
  dimension = 0L
)

wasserstein_distance(
  x,
  y,
  tol = sqrt(.Machine$double.eps),
  p = 1,
  validate = TRUE,
  dimension = 0L
)

kantorovich_distance(
  x,
  y,
  tol = sqrt(.Machine$double.eps),
  p = 1,
  validate = TRUE,
  dimension = 0L
)
```

## Arguments

- x:

  Either a matrix of shape \\n \times 2\\ or an object of class
  [persistence](https://tdaverse.github.io/phutil/reference/persistence.md)
  specifying the first persistence diagram.

- y:

  Either a matrix of shape \\m \times 2\\ or an object of class
  [persistence](https://tdaverse.github.io/phutil/reference/persistence.md)
  specifying the second persistence diagram.

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

- p:

  A numeric value specifying the power for the Wasserstein distance.
  Defaults to `1.0`.

## Value

A numeric value storing either the Bottleneck or the Wasserstein
distance between the two persistence diagrams.

## Details

A matching \\\varphi : D_1 \to D_2\\ between persistence diagrams is a
bijection of multisets, where both diagrams are assumed to have all
points on the diagonal with infinite multiplicity. The
*\\p\\-Wasserstein distance* between \\D_1\\ and \\D_2\\ is defined as
the infimum over all matchings of the expression

\$\$W_p(D_1,D_2) = \inf\_{\varphi: D_1 \to D_2} \left( \sum\_{x \in
D_1}{\lVert x - \varphi(x) \rVert^p} \right)^{\frac{1}{p}}\$\$

that can be thought of as the Minkowski distance between the diagrams
viewed as vectors on the shared coordinates defined by the matching
\\\varphi\\. The norm \\\lVert \cdot \rVert\\ can be arbitrary; as
implemented here, it is the infinity norm \\\lVert (x_1,x_2)
\rVert\_\infty = \max(x_1,x_2)\\. In the limit \\p \to \infty\\, the
Wasserstein distance becomes the *bottleneck distance*:

\$\$B(D_1,D_2) = \inf\_{\varphi: D_1 \to D_2} \sup\_{x \in D_1}{\lVert
x - \varphi(x) \rVert}.\$\$

The Wasserstein metric is also called the Kantorovich metric in
recognition of the originator of the metric.

## See also

[the Hera C++ library](https://github.com/anigmetov/hera)

## Examples

``` r
bottleneck_distance(
  persistence_sample[[1]]$pairs[[1]],
  persistence_sample[[2]]$pairs[[1]]
)
#> [1] 0.06197453

bottleneck_distance(
  persistence_sample[[1]],
  persistence_sample[[2]]
)
#> [1] 0.06197453

wasserstein_distance(
  persistence_sample[[1]]$pairs[[1]],
  persistence_sample[[2]]$pairs[[1]]
)
#> [1] 1.53403

wasserstein_distance(
  persistence_sample[[1]],
  persistence_sample[[2]]
)
#> [1] 1.53403
```
