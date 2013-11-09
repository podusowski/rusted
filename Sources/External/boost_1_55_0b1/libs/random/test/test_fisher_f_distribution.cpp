/* test_fisher_f_distribution.cpp
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id: test_fisher_f_distribution.cpp 71018 2011-04-05 21:27:52Z steven_watanabe $
 *
 */

#include <boost/random/fisher_f_distribution.hpp>
#include <limits>

#define BOOST_RANDOM_DISTRIBUTION boost::random::fisher_f_distribution<>
#define BOOST_RANDOM_ARG1 m
#define BOOST_RANDOM_ARG2 n
#define BOOST_RANDOM_ARG1_DEFAULT 1.0
#define BOOST_RANDOM_ARG2_DEFAULT 1.0
#define BOOST_RANDOM_ARG1_VALUE 7.5
#define BOOST_RANDOM_ARG2_VALUE 0.25

#define BOOST_RANDOM_DIST0_MIN 0.0
#define BOOST_RANDOM_DIST0_MAX (std::numeric_limits<double>::infinity)()
#define BOOST_RANDOM_DIST1_MIN 0.0
#define BOOST_RANDOM_DIST1_MAX (std::numeric_limits<double>::infinity)()
#define BOOST_RANDOM_DIST2_MIN 0.0
#define BOOST_RANDOM_DIST2_MAX (std::numeric_limits<double>::infinity)()

#define BOOST_RANDOM_TEST1_PARAMS (1.0, 2.1)
#define BOOST_RANDOM_TEST2_PARAMS (10.0, 10.0)

#include "test_distribution.ipp"
