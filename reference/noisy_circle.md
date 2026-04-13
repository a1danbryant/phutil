# Toy Data: Noisy circle

A simulated data set consisting of 100 points sampled from a circle with
additive Gaussian noise using a standard deviation of 0.05.

## Usage

``` r
noisy_circle_points

noisy_circle_ripserr

noisy_circle_tda_rips
```

## Format

### `noisy_circle_points`

A matrix with 100 rows and 2 columns listing the coordinates of the
points.

### `noisy_circle_ripserr`

An object of class 'PHom' as returned by the
[[`ripserr::vietoris_rips()`](https://rdrr.io/pkg/ripserr/man/vietoris_rips.html)](https://tdaverse.github.io/ripserr/reference/vietoris_rips.html)
function, which is a data frame with 3 variables:

- `dimension`: the dimension/degree of the feature,

- `birth`: the birth value of the feature,

- `death`: the death value of the feature.

### `noisy_circle_tda_rips`

A list of length 1 containing an object of class 'diagram' as returned
by the
[`TDA::ripsDiag()$diagram`](https://www.rdocumentation.org/packages/TDA/versions/1.9.1/topics/ripsDiag)
function, which is a matrix with 3 columns:

- `dimension`: the dimension/degree of the feature,

- `birth`: the birth value of the feature,

- `death`: the death value of the feature.

An object of class `PHom` (inherits from `data.frame`) with 101 rows and
3 columns.

An object of class `list` of length 1.

## Source

<https://tdaverse.github.io/tdaunif/reference/circles.html>,
<https://tdaverse.github.io/ripserr/reference/vietoris_rips.html>,
<https://www.rdocumentation.org/packages/TDA/versions/1.9.1/topics/ripsDiag>

## Details

The point cloud stored in `noisy_circle_points` has been generated using
the [**tdaunif**](https://tdaverse.github.io/tdaunif/) package using the
[[`tdaunif::sample_circle()`](https://tdaverse.github.io/tdaunif/reference/circles.html)](https://tdaverse.github.io/tdaunif/reference/circles.html)
function. Specifically, the following parameters were used: `n = 100`,
`sd = 0.05` and a seed of 1234.

The persistence diagram stored in `noisy_circle_ripserr` has been
computed using the [**ripserr**](https://tdaverse.github.io/ripserr/)
package with the
[[`ripserr::vietoris_rips()`](https://rdrr.io/pkg/ripserr/man/vietoris_rips.html)](https://tdaverse.github.io/ripserr/reference/vietoris_rips.html)
function. Specifically, the following parameters were used:
`max_dim = 1L`.

The persistence diagram stored in `noisy_circle_tda_rips` has been
computed using the **TDA** package with the
[[`TDA::ripsDiag()`](https://rdrr.io/pkg/TDA/man/ripsDiag.html)](https://www.rdocumentation.org/packages/TDA/versions/1.9.1/topics/ripsDiag)
function. Specifically, the following parameters were used:
`maxdimension = 1L` and `maxscale = 1.6322`.
