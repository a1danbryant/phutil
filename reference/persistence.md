# An `S3` class for storing persistence data

A collection of functions to coerce persistence data into objects of
class `persistence` (See **Value** section for more details on this
class). It is currently possible to coerce persistence data from the
following sources:

- a matrix with at least 3 columns (dimension/degree, start/birth,
  end/death) as returned by
  [[`ripserr::vietoris_rips()`](https://rdrr.io/pkg/ripserr/man/vietoris_rips.html)](https://tdaverse.github.io/ripserr/reference/vietoris_rips.html)
  in the form of the 'PHom' class,

- a list as returned by any `*Diag()` function in the **TDA** package.

## Usage

``` r
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'list'
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'persistence'
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'data.frame'
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'matrix'
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'diagram'
as_persistence(x, warn = TRUE, ...)

# S3 method for class 'PHom'
as_persistence(x, ...)

# S3 method for class 'hclust'
as_persistence(x, warn = TRUE, birth = NULL, ...)

# S3 method for class 'persistence'
print(x, ...)

# S3 method for class 'persistence'
format(x, ...)

get_pairs(x, dimension, ...)

# S3 method for class 'persistence'
as.matrix(x, ...)

# S3 method for class 'persistence'
as.data.frame(x, row.names = NULL, optional = TRUE, ...)
```

## Arguments

- x:

  An `R` object containing the persistence data to be coerced into an
  object of class `persistence`. Currently supported forms are:

  - a \\\geq 2\\-column matrix (or object coercible to one) with
    dimension/degree, start/birth and end/death columns; if it has only
    2 columns, we assume that the `dimension` column is missing and we
    set it to `0` (i.e. we assume that the data is in the form `birth`
    and `death`),

  - a [`base::data.frame`](https://rdrr.io/r/base/data.frame.html) (or
    object coercible to one) with at least 3 columns containing the
    persistence data; if it has only 2 columns, we assume that the
    `dimension` column is missing and we set it to `0`,

  - a list of 2-column matrices (or objects coercible to one) with the
    first column being the birth and the second column being the death
    of homological features; indexed by dimension, i.e. the \\i\\-*th*
    element of the list corresponds to the \\(i-1)\\-*th* homology
    dimension,

  - an object of class 'PHom' as returned by
    [[`ripserr::vietoris_rips()`](https://rdrr.io/pkg/ripserr/man/vietoris_rips.html)](https://tdaverse.github.io/ripserr/reference/vietoris_rips.html),

  - (a list as returned by a `*Diag()` function in **TDA** (e.g.
    [[`TDA::ripsDiag()`](https://rdrr.io/pkg/TDA/man/ripsDiag.html)](https://www.rdocumentation.org/packages/TDA/versions/1.9.1/topics/ripsDiag))
    whose first element is) an object of class 'diagram',

  - an object of class
    [`stats::hclust`](https://rdrr.io/r/stats/hclust.html) in which case
    we use the entry `height` as the death of homological features and 0
    as the birth of all features.

- warn:

  A boolean specifying whether to issue a warning if the input
  persistence data contained unordered pairs. Defaults to `TRUE`.

- ...:

  Parameters passed to methods.

- birth:

  A numeric value specifying the height at which to declare all leaves
  were born. Defaults to `0` if all heights are non-negative and `-Inf`
  otherwise.

- dimension:

  A non-negative integer specifying the homology dimension for which to
  recover a matrix of persistence pairs.

- row.names:

  `NULL` or a character vector giving the row names for the data frame.
  Missing values are not allowed.

- optional:

  logical. If `TRUE`, setting row names and converting column names (to
  syntactic names: see
  [`make.names`](https://rdrr.io/r/base/make.names.html)) is optional.
  Note that all of R's base package
  [`as.data.frame()`](https://rdrr.io/r/base/as.data.frame.html) methods
  use `optional` only for column names treatment, basically with the
  meaning of
  [`data.frame`](https://rdrr.io/r/base/data.frame.html)`(*, check.names = !optional)`.
  See also the `make.names` argument of the `matrix` method.

## Value

An object of class `persistence` which is a list of 2 elements:

- `pairs`: A list of 2-column matrices containing birth-death pairs. The
  \\i\\-*th* element of the list corresponds to the \\(i-1)\\-*th*
  homology dimension. If there is no pairs for a given dimension but
  there are pairs in higher dimensions, the corresponding element(s)
  is/are filled with a \\0 \times 2\\ numeric matrix.

- `metadata`: A list of length 6 containing information about how the
  data was computed:

  - `orderered_pairs`: A boolean indicating whether the pairs in the
    `pairs` list are ordered (i.e. the first column is strictly less
    than the second column).

  - `data`: The name of the object containing the original data on which
    the persistence data was computed.

  - `engine`: The name of the package and the function of this package
    that computed the persistence data in the form
    `"package_name::package_function"`.

  - `filtration`: The filtration used in the computation in a
    human-readable format (i.e. full names, capitals where need, etc.).

  - `parameters`: A list of parameters used in the computation.

  - `call`: The exact call that generated the persistence data.

## Details

**Caution.** When providing an *unnamed* input matrix, the matrix
coercer assumes that it has at least 3 columns, with the first column
being the dimension/degree, the second column being the start/birth and
the third column being the end/death.

## Examples

``` r
as_persistence(noisy_circle_ripserr)
#> 
#> ── Persistence Data ────────────────────────────────────────────────────────────
#> ℹ There are 99 and 2 pairs in dimensions 0 and 1 respectively.
#> ℹ Computed from a Vietoris-Rips/Cubical filtration using `ripserr::<vietoris_rips/cubical>()`.
#> ! With unknown parameters.

x <- as_persistence(noisy_circle_tda_rips)
x
#> 
#> ── Persistence Data ────────────────────────────────────────────────────────────
#> ℹ There are 100 and 2 pairs in dimensions 0 and 1 respectively.
#> ℹ Computed from a Vietoris-Rips filtration using `TDA::ripsDiag()`.
#> ℹ With the following parameters: maxdimension = 1 and maxscale = 1.6322.

as_persistence(x)
#> 
#> ── Persistence Data ────────────────────────────────────────────────────────────
#> ℹ There are 100 and 2 pairs in dimensions 0 and 1 respectively.
#> ℹ Computed from a Vietoris-Rips filtration using `TDA::ripsDiag()`.
#> ℹ With the following parameters: maxdimension = 1 and maxscale = 1.6322.

get_pairs(x, dimension = 1)
#>            [,1]       [,2]
#> [1,] 0.38239273 1.63218717
#> [2,] 0.07209847 0.07589632

as.data.frame(x)
#>     dimension      birth       death
#> 1           0 0.00000000 1.632200000
#> 2           0 0.00000000 0.367739240
#> 3           0 0.00000000 0.332581692
#> 4           0 0.00000000 0.322667292
#> 5           0 0.00000000 0.209691635
#> 6           0 0.00000000 0.195353064
#> 7           0 0.00000000 0.180419597
#> 8           0 0.00000000 0.177862977
#> 9           0 0.00000000 0.176442651
#> 10          0 0.00000000 0.169506194
#> 11          0 0.00000000 0.162200631
#> 12          0 0.00000000 0.160023839
#> 13          0 0.00000000 0.157216617
#> 14          0 0.00000000 0.157096157
#> 15          0 0.00000000 0.149973549
#> 16          0 0.00000000 0.145317069
#> 17          0 0.00000000 0.135505886
#> 18          0 0.00000000 0.130637844
#> 19          0 0.00000000 0.130409100
#> 20          0 0.00000000 0.124250763
#> 21          0 0.00000000 0.118818087
#> 22          0 0.00000000 0.117432967
#> 23          0 0.00000000 0.116982730
#> 24          0 0.00000000 0.111985578
#> 25          0 0.00000000 0.108047450
#> 26          0 0.00000000 0.107157343
#> 27          0 0.00000000 0.100600042
#> 28          0 0.00000000 0.100207281
#> 29          0 0.00000000 0.098008489
#> 30          0 0.00000000 0.096993058
#> 31          0 0.00000000 0.095547148
#> 32          0 0.00000000 0.094035885
#> 33          0 0.00000000 0.093127873
#> 34          0 0.00000000 0.093009295
#> 35          0 0.00000000 0.091994695
#> 36          0 0.00000000 0.091677657
#> 37          0 0.00000000 0.089061655
#> 38          0 0.00000000 0.087967986
#> 39          0 0.00000000 0.086923406
#> 40          0 0.00000000 0.084885493
#> 41          0 0.00000000 0.081223989
#> 42          0 0.00000000 0.080077051
#> 43          0 0.00000000 0.075611873
#> 44          0 0.00000000 0.075196405
#> 45          0 0.00000000 0.074676735
#> 46          0 0.00000000 0.073243933
#> 47          0 0.00000000 0.072876701
#> 48          0 0.00000000 0.072391768
#> 49          0 0.00000000 0.072299367
#> 50          0 0.00000000 0.071887979
#> 51          0 0.00000000 0.069296633
#> 52          0 0.00000000 0.068480870
#> 53          0 0.00000000 0.068176443
#> 54          0 0.00000000 0.065670399
#> 55          0 0.00000000 0.063840563
#> 56          0 0.00000000 0.063234392
#> 57          0 0.00000000 0.063189342
#> 58          0 0.00000000 0.059868540
#> 59          0 0.00000000 0.059285719
#> 60          0 0.00000000 0.058866113
#> 61          0 0.00000000 0.056665629
#> 62          0 0.00000000 0.056131890
#> 63          0 0.00000000 0.055221726
#> 64          0 0.00000000 0.054383073
#> 65          0 0.00000000 0.053424110
#> 66          0 0.00000000 0.052711260
#> 67          0 0.00000000 0.051592529
#> 68          0 0.00000000 0.050061244
#> 69          0 0.00000000 0.049106015
#> 70          0 0.00000000 0.048538771
#> 71          0 0.00000000 0.048131669
#> 72          0 0.00000000 0.047295623
#> 73          0 0.00000000 0.047067993
#> 74          0 0.00000000 0.046208831
#> 75          0 0.00000000 0.045668485
#> 76          0 0.00000000 0.045216427
#> 77          0 0.00000000 0.044062798
#> 78          0 0.00000000 0.043163334
#> 79          0 0.00000000 0.042355499
#> 80          0 0.00000000 0.040113039
#> 81          0 0.00000000 0.039172078
#> 82          0 0.00000000 0.035989881
#> 83          0 0.00000000 0.035517071
#> 84          0 0.00000000 0.035223243
#> 85          0 0.00000000 0.034865081
#> 86          0 0.00000000 0.034063248
#> 87          0 0.00000000 0.033248437
#> 88          0 0.00000000 0.029776983
#> 89          0 0.00000000 0.029389071
#> 90          0 0.00000000 0.027899370
#> 91          0 0.00000000 0.027031372
#> 92          0 0.00000000 0.026069254
#> 93          0 0.00000000 0.025996476
#> 94          0 0.00000000 0.024769890
#> 95          0 0.00000000 0.024621462
#> 96          0 0.00000000 0.020555701
#> 97          0 0.00000000 0.014004707
#> 98          0 0.00000000 0.009238432
#> 99          0 0.00000000 0.008548402
#> 100         0 0.00000000 0.008162723
#> 101         1 0.38239273 1.632187167
#> 102         1 0.07209847 0.075896319

# distances between cities
euroclust <- hclust(eurodist, method = "ward.D")
as_persistence(euroclust)
#> 
#> ── Persistence Data ────────────────────────────────────────────────────────────
#> ℹ There are 21 pairs in dimension 0 respectively.
#> ℹ Computed from a Ward.D-Linkage filtration using `stats::hclust()`.
#> ! With unknown parameters.

# `hclust()` can accommodate negative distances
d <- as.dist(rbind(c(0, 3, -4), c(3, 0, 5), c(-4, 5, 0)))
hc <- hclust(d, method = "single")
ph <- as_persistence(hc, birth = -10)
get_pairs(ph, 0)
#>      birth death
#> [1,]   -10    -4
#> [2,]   -10     3
#> [3,]   -10   Inf
```
