/*

Copyright (c) 2016, M. Kerber, D. Morozov, A. Nigmetov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or
upgrades to the features, functionality or performance of the source code
(Enhancements) to anyone; however, if you choose to make your Enhancements
available either publicly, or directly to copyright holder,
without imposing a separate written license agreement for such Enhancements,
then you hereby grant the following license: a  non-exclusive, royalty-free
perpetual license to install, use, modify, prepare derivative works, incorporate
into other computer software, distribute, and sublicense such enhancements or
derivative works thereof, in binary and source code form.

  */


#include <assert.h>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iterator>
#include <chrono>
#include <numeric>
#include <cmath>

#include "def_debug_ws.h"

#define PRINT_DETAILED_TIMING

#ifdef FOR_R_TDA
#undef DEBUG_AUCTION
#endif


namespace hera {
namespace ws {

// *****************************
// AuctionRunnerGS
// *****************************

template<class R, class AO, class PC>
AuctionRunnerGS<R, AO, PC>::AuctionRunnerGS(const PC& A,
                                        const PC& B,
                                        const AuctionParams<Real>& _params,
                                        const Prices& prices) :
    bidders(A),
    items(B),
    num_bidders(A.size()),
    num_items(B.size()),
    items_to_bidders(B.size(), k_invalid_index),
    bidders_to_items(A.size(), k_invalid_index),
    params(_params),
    oracle(bidders, items, params)

{
    if (!prices.empty())
        oracle.set_prices(prices);

    if (params.epsilon_common_ratio == 0)
        params.epsilon_common_ratio = 5;

    if (params.initial_epsilon == 0)
        params.initial_epsilon = oracle.max_val_ / 4;

    assert(params.initial_epsilon > 0.0 );
    assert(params.epsilon_common_ratio > 0.0 );
    assert(A.size() == B.size());
}

template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::assign_item_to_bidder(const IdxType item_idx, const IdxType bidder_idx)
{
    result.num_rounds++;
    sanity_check();
    // only unassigned bidders should submit bids and get items
    assert(bidders_to_items[bidder_idx] == k_invalid_index);
    IdxType old_item_owner = items_to_bidders[item_idx];

    // set new owner
    bidders_to_items[bidder_idx] = item_idx;
    items_to_bidders[item_idx] = bidder_idx;
    // remove bidder from the list of unassigned bidders
    unassigned_bidders.erase(bidder_idx);

    // old owner becomes unassigned
    if (old_item_owner != k_invalid_index) {
        bidders_to_items[old_item_owner] = k_invalid_index;
        unassigned_bidders.insert(old_item_owner);
    }
}


template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::flush_assignment()
{
    for(auto& b2i : bidders_to_items) {
        b2i = k_invalid_index;
    }
    for(auto& i2b : items_to_bidders) {
        i2b = k_invalid_index;
    }
    // we must flush assignment only after we got perfect matching
    assert(unassigned_bidders.empty());
    // all bidders become unassigned
    for(size_t bidder_idx = 0; bidder_idx < num_bidders; ++bidder_idx) {
        unassigned_bidders.insert(bidder_idx);
    }
    assert(unassigned_bidders.size() == bidders.size());

    oracle.adjust_prices();
}


template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::run_auction_phases()
{
    result.final_relative_error = std::numeric_limits<Real>::max();
    // choose some initial epsilon
    oracle.set_epsilon(params.initial_epsilon);
    result.start_epsilon = oracle.get_epsilon();
    result.final_epsilon = oracle.get_epsilon();
    assert( oracle.get_epsilon() > 0 );
    for(int phase_num = 0; phase_num < params.max_num_phases; ++phase_num) {
        flush_assignment();
        run_auction_phase();
        Real current_result = getDistanceToQthPowerInternal();
        Real denominator = current_result - num_bidders * oracle.get_epsilon();
        current_result = std::pow(current_result, 1 / params.wasserstein_power);

        if ( denominator > 0 ) {
            denominator = std::pow(denominator, 1 / params.wasserstein_power);
            Real numerator = current_result - denominator;
            result.final_relative_error = numerator / denominator;
            if (result.final_relative_error <= params.delta) {
                break;
            }
        }
        // decrease epsilon for the next iteration
        oracle.set_epsilon( oracle.get_epsilon() / params.epsilon_common_ratio );
        result.final_epsilon = oracle.get_epsilon();
    }

    result.prices = oracle.get_prices();
}


template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::run_auction()
{
    if (num_bidders == 1) {
            assign_item_to_bidder(0, 0);
            result.cost = get_item_bidder_cost(0,0);
    } else {
        run_auction_phases();

#ifdef DEBUG_AUCTION
        if (result.final_relative_error > params.delta and not params.tolerate_max_iter_exceeded) {
            std::cerr << "Maximum iteration number exceeded, exiting. Current result is: ";
            std::cerr << pow(result.cost, 1 / params.wasserstein_power) << std::endl;
            if (not params.tolerate_max_iter_exceeded)
                throw std::runtime_error("Maximum iteration number exceeded");
        }
#endif
    }

    result.compute_distance(params.wasserstein_power);
    is_distance_computed = true;

    if (params.return_matching) {
        result.clear_matching();
        for(size_t bidder_idx = 0; bidder_idx < num_bidders; ++bidder_idx) {
            int bidder_id = get_bidder_id(bidder_idx);
            int item_id = get_bidders_item_id(bidder_idx);
            result.add_to_matching(bidder_id, item_id);
        }
    }
}


template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::run_auction_phase()
{
    result.num_phases++;
    do {
        size_t bidder_idx = *unassigned_bidders.begin();
        auto optimal_bid = oracle.get_optimal_bid(bidder_idx);
        auto optimal_item_idx = optimal_bid.first;
        auto bid_value = optimal_bid.second;
        assign_item_to_bidder(optimal_bid.first, bidder_idx);
        oracle.set_price(optimal_item_idx, bid_value);
    } while (not unassigned_bidders.empty());

#ifdef DEBUG_AUCTION
    for(size_t bidder_idx = 0; bidder_idx < num_bidders; ++bidder_idx) {
        if ( bidders_to_items[bidder_idx] < 0 or bidders_to_items[bidder_idx] >= (IdxType)num_bidders) {
            std::cerr << "After auction terminated bidder " << bidder_idx;
            std::cerr << " has no items assigned" << std::endl;
            throw std::runtime_error("Auction did not give a perfect matching");
        }
    }
#endif

}

template<class R, class AO, class PC>
R AuctionRunnerGS<R, AO, PC>::get_item_bidder_cost(const size_t item_idx, const size_t bidder_idx, const bool tolerate_invalid_idx) const
{
    if (item_idx != k_invalid_index and bidder_idx != k_invalid_index) {
        return std::pow(dist_lp(bidders[bidder_idx], items[item_idx], params.internal_p, params.dim), params.wasserstein_power);
    } else {
        if (tolerate_invalid_idx)
            return R(0.0);
        else
            throw std::runtime_error("Invalid idx in get_item_bidder_cost, item_idx = " + std::to_string(item_idx) + ", bidder_idx = " + std::to_string(bidder_idx));
    }
}

template<class R, class AO, class PC>
R AuctionRunnerGS<R, AO, PC>::getDistanceToQthPowerInternal()
{
    sanity_check();
    Real r = 0.0;
    for(size_t bIdx = 0; bIdx < num_bidders; ++bIdx) {
        r += get_item_bidder_cost(bidders_to_items[bIdx], bIdx);
    }
    result.cost = r;
    return result.cost;
}

template<class R, class AO, class PC>
R AuctionRunnerGS<R, AO, PC>::get_wasserstein_distance()
{
    assert(is_distance_computed);
    return pow(get_wasserstein_cost(), 1/ params.wasserstein_power);
}

template<class R, class AO, class PC>
R AuctionRunnerGS<R, AO, PC>::get_wasserstein_cost()
{
    assert(is_distance_computed);
    return result.cost;
}



// Debug routines

template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::print_debug()
{
#ifdef DEBUG_AUCTION
    sanity_check();
    std::cout << "**********************" << std::endl;
    std::cout << "Current assignment:" << std::endl;
    for(size_t idx = 0; idx < bidders_to_items.size(); ++idx) {
        std::cout << idx << " <--> " << bidders_to_items[idx] << std::endl;
    }
    std::cout << "Weights: " << std::endl;
    //for(size_t i = 0; i < num_bidders; ++i) {
        //for(size_t j = 0; j < num_items; ++j) {
            //std::cout << oracle.weight_matrix[i][j] << " ";
        //}
        //std::cout << std::endl;
    //}
    std::cout << "Prices: " << std::endl;
    for(const auto price : oracle.get_prices()) {
        std::cout << price << std::endl;
    }
    std::cout << "**********************" << std::endl;
#endif
}


template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::sanity_check()
{
#ifdef DEBUG_AUCTION
    if (bidders_to_items.size() != num_bidders) {
        std::cerr << "Wrong size of bidders_to_items, must be " << num_bidders << ", is " << bidders_to_items.size() << std::endl;
        throw std::runtime_error("Wrong size of bidders_to_items");
    }

    if (items_to_bidders.size() != num_bidders) {
        std::cerr << "Wrong size of items_to_bidders, must be " << num_bidders << ", is " << items_to_bidders.size() << std::endl;
        throw std::runtime_error("Wrong size of items_to_bidders");
    }

    for(size_t bidder_idx = 0; bidder_idx < num_bidders; ++bidder_idx) {
        assert( bidders_to_items[bidder_idx] == k_invalid_index or ( bidders_to_items[bidder_idx] < (IdxType)num_items and bidders_to_items[bidder_idx] >= 0));

        if ( bidders_to_items[bidder_idx] != k_invalid_index) {

            if ( std::count(bidders_to_items.begin(),
                        bidders_to_items.end(),
                        bidders_to_items[bidder_idx]) > 1 ) {
                std::cerr << "Item " << bidders_to_items[bidder_idx];
                std::cerr << " appears in bidders_to_items more than once" << std::endl;
                throw std::runtime_error("Duplicate in bidders_to_items");
            }

            if (items_to_bidders.at(bidders_to_items[bidder_idx]) != static_cast<int>(bidder_idx)) {
                std::cerr << "Inconsitency: bidder_idx = " << bidder_idx;
                std::cerr << ", item_idx in bidders_to_items = ";
                std::cerr << bidders_to_items[bidder_idx];
                std::cerr << ", bidder_idx in items_to_bidders = ";
                std::cerr << items_to_bidders[bidders_to_items[bidder_idx]] << std::endl;
                throw std::runtime_error("inconsistent mapping");
            }
        }
    }

    for(IdxType item_idx = 0; item_idx < static_cast<IdxType>(num_bidders); ++item_idx) {
        assert( items_to_bidders[item_idx] == k_invalid_index or ( items_to_bidders[item_idx] < static_cast<IdxType>(num_items) and items_to_bidders[item_idx] >= 0));
        if ( items_to_bidders.at(item_idx) != k_invalid_index) {

            // check for uniqueness
            if ( std::count(items_to_bidders.begin(),
                        items_to_bidders.end(),
                        items_to_bidders[item_idx]) > 1 ) {
                std::cerr << "Bidder " << items_to_bidders[item_idx];
                std::cerr << " appears in items_to_bidders more than once" << std::endl;
                throw std::runtime_error("Duplicate in items_to_bidders");
            }
            // check for consistency
            if (bidders_to_items.at(items_to_bidders.at(item_idx)) != static_cast<int>(item_idx)) {
                std::cerr << "Inconsitency: item_idx = " << item_idx;
                std::cerr << ", bidder_idx in items_to_bidders = ";
                std::cerr << items_to_bidders[item_idx];
                std::cerr << ", item_idx in bidders_to_items= ";
                std::cerr << bidders_to_items[items_to_bidders[item_idx]] << std::endl;
                throw std::runtime_error("inconsistent mapping");
            }
        }
    }
#endif
}

template<class R, class AO, class PC>
void AuctionRunnerGS<R, AO, PC>::print_matching()
{
#ifdef DEBUG_AUCTION
    sanity_check();
    for(size_t bIdx = 0; bIdx < bidders_to_items.size(); ++bIdx) {
        if (bidders_to_items[bIdx] != k_invalid_index) {
            auto pA = bidders[bIdx];
            auto pB = items[bidders_to_items[bIdx]];
            std::cout <<  pA << " <-> " << pB << "+" << pow(dist_lp(pA, pB, params.internal_p, params.dimension), params.wasserstein_power) << std::endl;
        } else {
            assert(false);
        }
    }
#endif
}

} // ws
} // hera
