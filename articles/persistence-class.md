# The persistence class

``` r
library(phutil)
```

## Structure of the class

An object of class `persistence` is a list of 2 elements:

- `pairs`: A list of 2-column matrices containing birth-death pairs. The
  $i$-*th* element of the list corresponds to the $(i - 1)$-*th*
  homology dimension. If there is no pairs for a given dimension but
  there are pairs in higher dimensions, the corresponding element(s)
  is/are filled with a numeric matrix with 0 rows.

- `metadata`: A list of length 6 containing information about how the
  data was computed:

  - `orderered_pairs`: A boolean indicating whether the pairs in the
    `pairs` list are ordered (i.e. the first column is strictly less
    than the second column).
  - `data`: The name of the object containing the original data on which
    the persistence data was computed.
  - `engine`: The name of the package and the function of this package
    that computed the persistence data in the form
    `"package_name::package_function"`.
  - `filtration`: The filtration used in the computation in a
    human-readable format (i.e. full names, capitals where needed,
    etc.).
  - `parameters`: A list of parameters used in the computation.
  - `call`: The exact call that generated the persistence data.

## Supported inputs

The `persistence` class is designed to support a variety of inputs,
including

- A single numeric matrix:

  If the user provides a matrix, it must have at least 2 columns and
  each row represents a topological feature.

&nbsp;

- If it has 2 columns, we assume that the first column corresponds to
  the birth of a feature and the second column corresponds to the death
  of a feature, irrespective of the order of the columns. In this case,
  we assume that the homology dimension of the feature is 0.

- If it has more than 2 columns, we assume that the first column
  corresponds to the homology dimension of the feature, the second
  column corresponds to the birth of a feature, and the third column
  corresponds to the death of a feature, irrespective of the order of
  the columns. The remaining columns are ignored.

&nbsp;

- A list of numeric matrices:

  If the user provides a list of matrices, each list element corresponds
  to an homology dimension, from 0 to some maximum value. Each matrix
  must have at least 2 columns and each row represents a topological
  feature in the corresponding homology dimension (given by the matrix
  index in the list minus 1). Each matrix is parsed as described above.

- A dataframe:

  If the user provides an object of class `data.frame`, it must have at
  least 2 columns and each row represents a topological feature. If it
  has exactly 2 columns, we add a `dimension` column with all values set
  to 0. If it has more than 2 columns, we require that `birth` and
  `death` exist in the column names. The `birth` and `death` columns are
  parsed as described above. The remaining columns are ignored.

- An object of class ‘PHom’:

  If the user provides an object of class ‘PHom’ as typically produced
  by
  [`ripserr::vietoris_rips()`](https://rdrr.io/pkg/ripserr/man/vietoris_rips.html),
  it means that it is a
  [`base::data.frame`](https://rdrr.io/r/base/data.frame.html) with
  columns `dimension`, `birth`, and `death` in that specific order. The
  `dimension` column is of type integer while the `birth` and `death`
  columns are of type numeric. The `dimension` column is used to create
  a list of matrices, where each matrix corresponds to an homology
  dimension, from 0 to the maximum value in the `dimension` column.

- An object of class ‘diagram’:

  If the user provides an object of class ‘diagram’ as typically
  produced by `TDA::*Diag()` functions in entry `diagram`, it means that
  it is a [`base::matrix`](https://rdrr.io/r/base/matrix.html) with 3
  columns with names `dimension`, `Birth` and `Death` in that specific
  order. The `dimension` column is of type integer while the `Birth` and
  `Death` columns are of type numeric. Furthermore, the object stores as
  attributes the parameters used to compute the diagram and the entire
  call to the function that produced the diagram. We first lowercase
  `Birth` and `Death`. Next, the `dimension` column is used to create a
  list of matrices, where each matrix corresponds to an homology
  dimension, from 0 to the maximum value in the `dimension` column. The
  `birth` and `death` columns are parsed as described above. The
  remaining columns are ignored.

- An object of class ‘hclust’:

  If the user provides an object of class ‘hclust’ as typically produced
  by [`stats::hclust()`](https://rdrr.io/r/stats/hclust.html), it means
  that it is a [`base::list`](https://rdrr.io/r/base/list.html) which
  contains the `height` element which is a set of $n - 1$ real values
  (non-decreasing for ultrametric trees) storing the clustering height,
  that is, the value of the criterion associated with the clustering
  method for the particular agglomeration. This is used as homological
  feature death while a birth of `0` is typically used.
