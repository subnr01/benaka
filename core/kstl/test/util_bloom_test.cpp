/*
 * Copyright (c) 2014 NetApp, Inc.
 * All rights reserved.
 */

#include <cxxtest/TestSuite.h>
#include <util_bloom.h>
#include <stdio.h>


/*
 * Doing a bloomarray of a 32-bit or 64-bit integer is easy.  To
 * make things harder for the unit test, we'll exercise the classes
 * using a 24-bit storage field and ensure that it packs the elements
 * tightly as it goes.
 */

struct val24 {
    uint32_t val : 24;
    val24(uint32_t in) : val(in) { }
    operator uint32_t () { return val; }
} __attribute__((packed));


/*
 * Note the lack of main(): the unit test suite finds our methods
 * by reflection, such that all methods starting with the name "test"
 * within our class will be invoked during compile-time testing.
 */

class BloomTest : public CxxTest::TestSuite
{
    public:
        void testBloomFilter();
        void testExtrinsicBloomFilter();
        void testBloomArray();
        void testExtrinsicBloomArray();
};


/*
 * Exercise a basic bloom filter.  We'll do a hundred passes of a
 * general test: add some known random elements to the filter,
 * ensure that the filter reports that they're all still in there,
 * and test the filter against thousands of other random numbers
 * to see what kind of false-positive rate we get.
 */

void BloomTest::testBloomFilter()
{
    printf("\n");

    static const uint32_t bytes_in_bloom_filter = 128;
    uint32_t count_to_insert = 30;
    uint32_t count_to_test = 30000;
    uint32_t test_pass_count = 100;
    uint32_t extraneous = 0;

    util::bloomfilter<bytes_in_bloom_filter,val24> filter;

    for (uint32_t pass = 0; pass < test_pass_count; ++pass) {
        uint32_t vals[count_to_insert];
        for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
            vals[jj] = (pass * 100) + jj * 3 + (pass ^ jj);
            filter.add(vals[jj]);
        }

        for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
            TS_ASSERT(filter.contains(vals[jj]));
        }

        for (uint32_t test_val = 0; test_val < count_to_test; ++test_val) {
            bool really_exists = false;
            for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
                if (test_val == vals[jj]) {
                    really_exists = true;
                    break;
                }
            }

            if (really_exists) {
                TS_ASSERT(filter.contains(test_val));
            } else {
                if (filter.contains(test_val)) {
                    ++extraneous;
                }
            }
        }
        filter.clear();
    }

    if (extraneous > 0) {
        float error_rate = (float)(count_to_test * test_pass_count) / extraneous;
        TS_ASSERT(error_rate > 10000);  // Better be less than 1 error in 10,000 at this size
        printf("bloom filter error rate: 1 out of %u\n", (uint32_t)error_rate);
    }
}


/*
 * Very rudimentary test on the ability to maintain a bloomfilter
 * in your own storage rather than in a discrete object.
 */

void BloomTest::testExtrinsicBloomFilter()
{
    printf("\n");

    static const uint32_t bytes_in_bloom_filter = 256;
    uint32_t count_to_insert = 100;
    uint32_t count_to_test = 30000;

    typedef util::bloomfilter<bytes_in_bloom_filter,uint32_t> filter;

    char buffer[bytes_in_bloom_filter];
    filter::clear(buffer);

    uint32_t vals[count_to_insert];
    for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
        vals[jj] = jj * 1023;
        filter::add(buffer, vals[jj]);
    }

    filter dup (buffer);
    for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
        TS_ASSERT(filter::contains(buffer, vals[jj]));
        TS_ASSERT(dup.contains(vals[jj]));
    }

    for (uint32_t test_val = 0; test_val < count_to_test; ++test_val) {
        bool really_exists = false;
        for (uint32_t jj = 0; jj < count_to_insert; ++jj) {
            if (test_val == vals[jj]) {
                really_exists = true;
                break;
            }

            bool in_filter = filter::contains(buffer, test_val);
            bool in_dup_filter = dup.contains(test_val);
            TS_ASSERT_EQUALS(in_filter, in_dup_filter);
            if (really_exists) {
                TS_ASSERT(in_filter && in_dup_filter);
            }
        }
    }
}


/*
 * Exercise a basic bloom array.  Again we'll do a hundred passes
 * of a general test: we'll add elements to the structure until
 * it's as full as it can get, and then we'll add some more--pushing
 * it over the edge into becoming a bloom filter.  We'll need to
 * check that the data isn't lost during the conversion, and we'll
 * again check the false-positive rate afterwards.
 */

void BloomTest::testBloomArray()
{
    printf("\n");

    static const uint32_t bytes_in_bloom_array = 128;
    uint32_t count_to_fill_array = bytes_in_bloom_array / 3;
    uint32_t count_to_insert_after = 5;
    uint32_t test_pass_count = 100;
    uint32_t count_to_test = 30000;
    uint32_t extraneous = 0;
    uint32_t mask = (1<<24)-1;

    util::bloomarray<bytes_in_bloom_array,val24> array;
    TS_ASSERT_EQUALS(array.capacity(), count_to_fill_array);

    for (uint32_t pass = 0; pass < test_pass_count; ++pass) {
        uint32_t vals[count_to_fill_array + count_to_insert_after];

        for (uint32_t jj = 0; jj < count_to_fill_array; ++jj) {
            vals[jj] = ((pass * 97) + jj * 5 + (pass ^ jj)) & mask;
            array.add(vals[jj]);
            TS_ASSERT_EQUALS(array.type(), util::BLOOMTYPE_ARRAY);
            TS_ASSERT_EQUALS(array.size(), jj+1);
            for (uint32_t kk = 0; kk < count_to_fill_array; ++kk) {
                if (kk <= jj) {
                    TS_ASSERT(array.containsIndex(kk));
                } else {
                    TS_ASSERT(!array.containsIndex(kk));
                }
            }
        }

        for (uint32_t jj = 0; jj < count_to_fill_array; ++jj) {
            TS_ASSERT(array[jj] == vals[jj]);
            vals[jj] = (vals[jj]+3) & mask;
            array[jj] = vals[jj];
            TS_ASSERT_EQUALS(array[jj], vals[jj]);
            TS_ASSERT_EQUALS(array.type(), util::BLOOMTYPE_ARRAY);
            TS_ASSERT_EQUALS(array.size(), count_to_fill_array);
            TS_ASSERT(array.contains(vals[jj]));
        }

        uint32_t next_val_to_insert = pass * pass;
        for (uint32_t jj = 0; jj < count_to_insert_after; ++jj) {
            while (array.contains(next_val_to_insert)) {
                ++next_val_to_insert;
            }
            array.add(next_val_to_insert);
            vals[count_to_fill_array+jj] = next_val_to_insert;
            TS_ASSERT_EQUALS(array.type(), util::BLOOMTYPE_FILTER);
        }

        for (uint32_t jj = 0; jj < count_to_fill_array + count_to_insert_after; ++jj) {
            TS_ASSERT(array.contains(vals[jj]));
        }

        for (uint32_t test_val = 0; test_val < count_to_test; ++test_val) {
            bool really_exists = false;
            for (uint32_t jj = 0; jj < count_to_fill_array + count_to_insert_after; ++jj) {
                if (test_val == vals[jj]) {
                    really_exists = true;
                    break;
                }
            }

            if (really_exists) {
                TS_ASSERT(array.contains(test_val));
            } else {
                if (array.contains(test_val)) {
                    ++extraneous;
                }
            }
        }
        array.clear();
    }

    if (extraneous > 0) {
        float error_rate = (float)(count_to_test * test_pass_count) / extraneous;
        TS_ASSERT(error_rate > 1000);  // Better be less than 1 error in 1,000 at this size
        printf("bloom array error rate: 1 out of %u\n", (uint32_t)error_rate);
    }
}

/*
 * Very rudimentary test on the ability to maintain a bloomarray
 * in your own storage rather than in a discrete object.
 */

void BloomTest::testExtrinsicBloomArray()
{
    printf("\n");

    static const uint32_t bytes_in_bloom_array = 256;
    uint32_t count_to_fill_array = bytes_in_bloom_array / 4;
    uint32_t count_to_insert_after = 5;
    uint32_t count_to_test = 30000;

    typedef util::bloomarray<bytes_in_bloom_array,uint32_t> array;
    TS_ASSERT_EQUALS(array::capacity(), count_to_fill_array);

    char buffer[bytes_in_bloom_array];
    util::bloomtype type;
    array::clear(buffer, &type);

    uint32_t vals[count_to_fill_array + count_to_insert_after];
    for (uint32_t jj = 0; jj < count_to_fill_array; ++jj) {
        vals[jj] = jj * 2047;
        array::add(buffer, &type, vals[jj]);
        TS_ASSERT_EQUALS(type, util::BLOOMTYPE_ARRAY);
        TS_ASSERT_EQUALS(array::size(buffer, type), jj+1);
    }

    uint32_t next_val_to_insert = 3095;
    for (uint32_t jj = 0; jj < count_to_insert_after; ++jj) {
        while (array::contains(buffer, type, next_val_to_insert)) {
            ++next_val_to_insert;
        }
        array::add(buffer, &type, next_val_to_insert);
        vals[count_to_fill_array+jj] = next_val_to_insert;
        TS_ASSERT_EQUALS(type, util::BLOOMTYPE_FILTER);
    }

    array dup(buffer, type);
    for (uint32_t jj = 0; jj < count_to_fill_array + count_to_insert_after; ++jj) {
        TS_ASSERT(array::contains(buffer, type, vals[jj]));
        TS_ASSERT(dup.contains(vals[jj]));
    }

    for (uint32_t test_val = 0; test_val < count_to_test; ++test_val) {
        bool really_exists = false;
        for (uint32_t jj = 0; jj < count_to_fill_array + count_to_insert_after; ++jj) {
            if (test_val == vals[jj]) {
                really_exists = true;
                break;
            }
        }

        bool in_array = array::contains(buffer, type, test_val);
        bool in_dup_array = dup.contains(test_val);
        TS_ASSERT_EQUALS(in_array, in_dup_array);
        if (really_exists) {
            TS_ASSERT(in_array && in_dup_array);
        }
    }
}