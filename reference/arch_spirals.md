# A sample of persistence diagrams from the arch spiral

A collection of 24 samples of size 120 on the arch spiral from which a
persistence diagram is computed using the
[`TDA::ripsDiag()`](https://rdrr.io/pkg/TDA/man/ripsDiag.html) function
with `maxdimension = 2` and `maxscale = 6`. Each diagram has been
generated using the
[`tdaunif::sample_arch_spiral()`](https://tdaverse.github.io/tdaunif/reference/arch-spirals.html)
function with the following parameters: `n = 120`,
``` arms = 2`` and  ```sd = 0.05\`. The seed was fixed to 28415.

## Usage

``` r
arch_spirals
```

## Format

A list of length 24, where each element is an object of class
'persistence'.
