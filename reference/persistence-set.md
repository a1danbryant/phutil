# An 'S3' class object for storing sets of persistence diagrams

An 'S3' class object for storing sets of persistence diagrams

## Usage

``` r
as_persistence_set(x)

# S3 method for class 'persistence_set'
format(x, ...)

# S3 method for class 'persistence_set'
print(x, ...)
```

## Arguments

- x:

  A list of objects of class
  [persistence](https://tdaverse.github.io/phutil/reference/persistence.md).

- ...:

  Additional arguments passed to the function.

## Value

An object of class 'persistence_set' containing the set of persistence
diagrams.

## Examples

``` r
# Create a persistence set from a list of persistence diagrams
as_persistence_set(persistence_sample[1:10])
#> 
#> ── Persistence Data Set ────────────────────────────────────────────────────────
#> ℹ A collection of 10 persistence diagrams.
```
