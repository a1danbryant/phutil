# A sample of persistence diagrams from the trefoil

A collection of 24 samples of size 120 on the trefoil from which a
persistence diagram is computed using the
[`TDA::ripsDiag()`](https://rdrr.io/pkg/TDA/man/ripsDiag.html) function
with `maxdimension = 2` and `maxscale = 6`. Each diagram has been
generated using the
[`tdaunif::sample_trefoil()`](https://tdaverse.github.io/tdaunif/reference/trefoil.html)
function with the following parameters: `n = 120` and `sd = 0.05`. The
seed was fixed to 28415.

## Usage

``` r
trefoils
```

## Format

A list of length 24, where each element is an object of class
'persistence'.
