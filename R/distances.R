#' Distances between two persistence diagrams
#'
#' This collection of functions computes the distance between two persistence
#' diagrams of the same homology dimension. The diagrams must be represented as
#' 2-column matrices. The first column of the matrix contains the birth times
#' and the second column contains the death times of the points.
#'
#' A matching \eqn{\varphi : D_1 \to D_2} between persistence diagrams is a
#' bijection of multisets, where both diagrams are assumed to have all points on
#' the diagonal with infinite multiplicity. The _\eqn{p}-Wasserstein distance_
#' between \eqn{D_1} and \eqn{D_2} is defined as the infimum over all matchings
#' of the expression
#'
#' \deqn{W_p(D_1,D_2) = \inf_{\varphi: D_1 \to D_2} \left( \sum_{x \in D_1}{\lVert x - \varphi(x) \rVert^p}
#' \right)^{\frac{1}{p}}}
#'
#' that can be thought of as the Minkowski distance between the diagrams viewed
#' as vectors on the shared coordinates defined by the matching \eqn{\varphi}.
#' The norm \eqn{\lVert \cdot \rVert} can be arbitrary; as implemented here, it
#' is the infinity norm \eqn{\lVert (x_1,x_2) \rVert_\infty = \max(x_1,x_2)}. In
#' the limit \eqn{p \to \infty}, the Wasserstein distance becomes the
#' _bottleneck distance_:
#'
#' \deqn{B(D_1,D_2) = \inf_{\varphi: D_1 \to D_2} \sup_{x \in D_1}{\lVert x - \varphi(x) \rVert}.}
#'
#' @param x Either a matrix of shape \eqn{n \times 2} or an object of class
#'   [persistence] specifying the first persistence diagram.
#' @param y Either a matrix of shape \eqn{m \times 2} or an object of class
#'   [persistence] specifying the second persistence diagram.
#' @param tol A numeric value specifying the relative error. Defaults to `1e-4`.
#'   For the Bottleneck distance, it can be set to `0.0` in which case the exact
#'   Bottleneck distance is computed, while an approximate Bottleneck distance
#'   is computed if `tol > 0.0`. For the Wasserstein distance, it must be
#'   strictly positive.
#' @param p A numeric value specifying the power for the Wasserstein distance.
#'   Defaults to `1.0`.
#' @param validate A boolean value specifying whether to validate the input
#'   persistence diagrams. Defaults to `TRUE`. If `FALSE`, the function will not
#'   check if the input persistence diagrams are valid. This can be useful for
#'   performance reasons, but it is recommended to keep it `TRUE` for safety.
#' @param dimension An integer value specifying the homology dimension for which
#'   to compute the distance. Defaults to `0L`. This is only used if `x` and `y`
#'   are objects of class [persistence].
#'
#' @returns A numeric value storing either the Bottleneck or the Wasserstein
#'   distance between the two persistence diagrams.
#'
#' @seealso [the Hera C++ library](https://github.com/anigmetov/hera)
#'
#' @examples
#' bottleneck_distance(
#'   persistence_sample[[1]]$pairs[[1]],
#'   persistence_sample[[2]]$pairs[[1]]
#' )
#'
#' bottleneck_distance(
#'   persistence_sample[[1]],
#'   persistence_sample[[2]]
#' )
#'
#' wasserstein_distance(
#'   persistence_sample[[1]]$pairs[[1]],
#'   persistence_sample[[2]]$pairs[[1]]
#' )
#'
#' wasserstein_distance(
#'   persistence_sample[[1]],
#'   persistence_sample[[2]]
#' )
#'
#' @name distances
NULL

#' @rdname distances
#' @export
bottleneck_distance <- function(x, y,
                                tol = 1e-4,
                                validate = TRUE,
                                dimension = 0L) {
  if (inherits(x, "persistence")) {
    x <- get_pairs(x, dimension = dimension)
  } else if (validate) {
    status <- check_2column_matrix(x, warn = FALSE)
    if (!status) {
      cli::cli_abort(
        "{.arg x} contains pairs with death prior to birth."
      )
    }
  }

  x <- x[x[, 1] < x[, 2], , drop = FALSE]

  if (inherits(y, "persistence")) {
    y <- get_pairs(y, dimension = dimension)
  } else if (validate) {
    status <- check_2column_matrix(y, warn = FALSE)
    if (!status) {
      cli::cli_abort(
        "{.arg y} contains pairs with death prior to birth."
      )
    }
  }

  y <- y[y[, 1] < y[, 2], , drop = FALSE]

  bottleneckDistance(
    x = x,
    y = y,
    delta = tol
  )
}

#' @rdname distances
#' @export
wasserstein_distance <- function(x, y,
                                 tol = 1e-4,
                                 p = 1.0,
                                 validate = TRUE,
                                 dimension = 0L) {
  if (inherits(x, "persistence")) {
    x <- get_pairs(x, dimension = dimension)
  } else if (validate) {
    status <- check_2column_matrix(x, warn = FALSE)
    if (!status) {
      cli::cli_abort(
        "{.arg x} contains pairs with death prior to birth."
      )
    }
  }

  x <- x[x[, 1] < x[, 2], , drop = FALSE]

  if (inherits(y, "persistence")) {
    y <- get_pairs(y, dimension = dimension)
  } else if (validate) {
    status <- check_2column_matrix(y, warn = FALSE)
    if (!status) {
      cli::cli_abort(
        "{.arg y} contains pairs with death prior to birth."
      )
    }
  }

  y <- y[y[, 1] < y[, 2], , drop = FALSE]

  if (p > 20) {
    return(bottleneck_distance(
      x = x,
      y = y,
      tol = tol,
      validate = FALSE,
      dimension = dimension
    ))
  }

  wassersteinDistance(
    x = x,
    y = y,
    delta = tol,
    wasserstein_power = p
  )
}

#' Pairwise distances within a set of persistence diagrams
#'
#' This collection of functions computes the pairwise distance matrix between
#' all pairs in a set of persistence diagrams of the same homology dimension.
#' The diagrams must be represented as 2-column matrices. The first column of
#' the matrix contains the birth times and the second column contains the death
#' times of the points.
#'
#' @param x A list of either 2-column matrices or objects of class [persistence]
#'   specifying the set of persistence diagrams.
#' @inheritParams distances
#' @param ncores An integer value specifying the number of cores to use for
#'   parallel computation. Defaults to `1L`.
#'
#' @returns An object of class 'dist' containing the pairwise distance matrix
#'   between the persistence diagrams.
#'
#' @examples
#' spl <- persistence_sample[1:10]
#'
#' # Extract the list of 2-column matrices for dimension 0 in the sample
#' x <- lapply(spl[1:10], function(x) x$pairs[[1]])
#'
#' # Compute the pairwise Bottleneck distances
#' Db <- bottleneck_pairwise_distances(spl)
#' Db <- bottleneck_pairwise_distances(x)
#'
#' # Compute the pairwise Wasserstein distances
#' Dw <- wasserstein_pairwise_distances(spl)
#' Dw <- wasserstein_pairwise_distances(x)
#'
#' @name pairwise-distances
NULL

#' @rdname pairwise-distances
#' @export
bottleneck_pairwise_distances <- function(x,
                                          tol = 1e-4,
                                          validate = TRUE,
                                          dimension = 0L,
                                          ncores = 1L) {
  indices <- seq_along(x)
  for (i in indices) {
    if (inherits(x[[i]], "persistence")) {
      x[[i]] <- get_pairs(x[[i]], dimension = dimension)
    } else if (validate) {
      status <- check_2column_matrix(x[[i]], warn = FALSE)
      if (!status) {
        cli::cli_abort(
          "{.arg x[{i}]} contains pairs with death prior to birth."
        )
      }
    }

    x[[i]] <- x[[i]][x[[i]][, 1] < x[[i]][, 2], , drop = FALSE]
  }

  D <- bottleneckPairwiseDistances(
    x = x,
    delta = tol,
    ncores = ncores
  )
  attr(D, "Size") <- length(x)
  attr(D, "Labels") <- indices
  attr(D, "Diag") <- FALSE
  attr(D, "Upper") <- FALSE
  attr(D, "method") <- "bottleneck"
  attr(D, "class") <- "dist"
  D
}

#' @rdname pairwise-distances
#' @export
wasserstein_pairwise_distances <- function(x,
                                           tol = 1e-4,
                                           p = 1.0,
                                           validate = TRUE,
                                           dimension = 0L,
                                           ncores = 1L) {
  indices <- seq_along(x)
  for (i in indices) {
    if (inherits(x[[i]], "persistence")) {
      x[[i]] <- get_pairs(x[[i]], dimension = dimension)
    } else if (validate) {
      status <- check_2column_matrix(x[[i]], warn = FALSE)
      if (!status) {
        cli::cli_abort(
          "{.arg x[{i}]} contains pairs with death prior to birth."
        )
      }
    }

    x[[i]] <- x[[i]][x[[i]][, 1] < x[[i]][, 2], , drop = FALSE]
  }

  if (p > 20) {
    return(bottleneck_pairwise_distances(
      x = x,
      tol = tol,
      validate = FALSE,
      dimension = dimension,
      ncores = ncores
    ))
  }

  D <- wassersteinPairwiseDistances(
    x = x,
    delta = tol,
    wasserstein_power = p,
    ncores = ncores
  )
  attr(D, "Size") <- length(x)
  attr(D, "Labels") <- indices
  attr(D, "Diag") <- FALSE
  attr(D, "Upper") <- FALSE
  attr(D, "method") <- "wasserstein"
  attr(D, "class") <- "dist"
  D
}
