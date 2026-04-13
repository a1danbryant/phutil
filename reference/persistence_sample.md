# Toy Data: A sample of persistence diagrams

A collection of 100 samples of size 100 on the sphere from which a
persistence diagram is computed using the
[`TDA::ripsDiag()`](https://rdrr.io/pkg/TDA/man/ripsDiag.html) function
with parameters `maxdimension = 1L` and `maxscale = 1.6322`. Each
diagram has been generated using the
[`tdaunif::sample_2sphere()`](https://tdaverse.github.io/tdaunif/reference/sphere.html)
function with the following parameters: `n = 100` and `sd = 0.05`. The
seed was fixed to 1234.

## Usage

``` r
persistence_sample
```

## Format

A list of length 100, where each element is an object of class
'persistence'.
