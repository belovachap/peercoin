// Copyright (c) 2020 Peercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <kernel.h>
#include <test/test_bitcoin.h>


BOOST_FIXTURE_TEST_SUITE(kernel_tests, BasicTestingSetup)

// Cases to consider for each kernel protocol:

// The first transaction time of the protocol
//   * block at same time -> true
//   * block at same time - 1 -> false
//   * block at maximum time difference -> true
//   * block at maximum time difference + 1 -> false

// The last transaction time of the protocol
//   * block at same time -> true
//   * block at same time - 1 -> false
//   * block at maximum time difference -> true
//   * block at maximum time difference + 1 -> false

BOOST_AUTO_TEST_CASE(check_coin_stake_timestamp_protocol_v02_test)
{
    // In the v0.2 kernel the timestamp of the coin stake transaction must be
    // on or before (up to 2 hours) the block
    // timestamp.

    // first time of the protocol: since the beginning of time
    // last time of the protocol:
    //     unsigned int nProtocolV03SwitchTime     = 1363800000 - 1;
    //     unsigned int nProtocolV03TestSwitchTime = 1359781000 - 1;
    // maximum time difference:
    //     static const int64_t MAX_FUTURE_BLOCK_TIME_PREV9 = 2 * 60 * 60;

    int64_t last_protocol_transaction_time = 1363800000 - 1;
    int64_t maximum_future_block_time = 2 * 60 * 60; // 2 hours

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time,
            last_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time - 1,
            last_protocol_transaction_time
        ),
        false
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time + maximum_future_block_time,
            last_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time + maximum_future_block_time + 1,
            last_protocol_transaction_time
        ),
        false
    );
}

BOOST_AUTO_TEST_CASE(check_coin_stake_timestamp_protocol_v03_test)
{
    // In the v0.3 kernel the timestamp of the block and the coin stake
    // transaction must match.

    // first time of the protocol: since the beginning of time
    //     unsigned int nProtocolV03SwitchTime     = 1363800000;
    //     unsigned int nProtocolV03TestSwitchTime = 1359781000;
    // last time of the protocol:
    //     const unsigned int nProtocolV09SwitchTime     = 1590062400 - 1; // Thu 21 May 12:00:00 UTC 2020 - 1
    //     const unsigned int nProtocolV09TestSwitchTime = 1581940800 - 1; // Mon 17 Feb 12:00:00 UTC 2020 - 1
    // maximum time difference: 0 (transaction time must match block time)

    int64_t first_protocol_transaction_time = 1363800000;
    int64_t last_protocol_transaction_time = 1590062400 - 1;

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time,
            first_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time - 1,
            first_protocol_transaction_time
        ),
        false
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time + 1,
            first_protocol_transaction_time
        ),
        false
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time,
            last_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time - 1,
            last_protocol_transaction_time
        ),
        false
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            last_protocol_transaction_time + 1,
            last_protocol_transaction_time
        ),
        false
    );
}

BOOST_AUTO_TEST_CASE(check_coin_stake_timestamp_protocol_v09_test)
{
    // In the v0.9 kernel the timestamp of the coin stake transaction must be
    // on or before (up to MAX_FUTURE_BLOCK_TIME seconds) the block timestamp.

    // first time of the protocol:
    //     const unsigned int nProtocolV09SwitchTime     = 1590062400; // Thu 21 May 12:00:00 UTC 2020
    //     const unsigned int nProtocolV09TestSwitchTime = 1581940800; // Mon 17 Feb 12:00:00 UTC 2020
    // last time of the protocol: until the end of time
    // maximum time difference:
    //     static const int64_t MAX_FUTURE_BLOCK_TIME_PREV9 = 15 * 60;

    int64_t first_protocol_transaction_time = 1590062400;
    int64_t maximum_future_block_time = 15 * 60; // 15 minutes

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time,
            first_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time - 1,
            first_protocol_transaction_time
        ),
        false
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time + maximum_future_block_time,
            first_protocol_transaction_time
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        CheckCoinStakeTimestamp(
            first_protocol_transaction_time + maximum_future_block_time + 1,
            first_protocol_transaction_time
        ),
        false
    );
}

BOOST_AUTO_TEST_SUITE_END()
