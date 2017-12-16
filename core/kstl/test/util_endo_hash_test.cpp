/*
 * Copyright (c) 2014 NetApp, Inc.
 * All rights reserved.
 */

#include <cxxtest/TestSuite.h>
#include <util_endohash.h>
#include <stdio.h>


/*
 * This is a really simple data structure, and will only have a correspondingly
 * simple unit test suite.  We'll use a small data object with a key/value
 * pair that can always be verified, and which knows when it's on the table.
 */

struct TestObject
{
    TestObject(int k) : key(k), value(~k)
    {
    }

    void validate_on_list() const
    {
        TS_ASSERT_EQUALS(value, ~key);
        TS_ASSERT(node.is_linked());
    }

    void validate_not_on_list() const
    {
        TS_ASSERT_EQUALS(value, ~key);
        TS_ASSERT(!node.is_linked());
    }

    uint32_t hash() const
    {
        return key;
    }

    bool operator== (const TestObject& ref) const
    {
        return (key == ref.key);
    }

    int key;
    int value;
    util::endolist_node node;
};

#define TEST_BUCKET_COUNT  255

typedef util::endohash<TestObject,TEST_BUCKET_COUNT> TestTable;


/*
 * Note the lack of main(): the unit test suite finds our methods
 * by reflection, such that all methods starting with the name "test"
 * within our class will be invoked during compile-time testing.
 */

class HashTest : public CxxTest::TestSuite
{
    public:


        /*
         * Exercises very rudimentary hash table behaviors.
         */

        void testBasicBehaviors()
        {
            TestTable table;

            // Define some interesting values to test with

            int test_keys[] = {
                    1,
                    2,
                    3,
                    3+TEST_BUCKET_COUNT,
                    3+TEST_BUCKET_COUNT*2,
                    4,
                    5,
                    TEST_BUCKET_COUNT-1,
                    TEST_BUCKET_COUNT,
                    TEST_BUCKET_COUNT+1
                };
            size_t test_key_count = sizeof(test_keys) / sizeof(test_keys[0]);

            int bad_test_keys[] = {
                    -1,
                    7+TEST_BUCKET_COUNT,
                    5+TEST_BUCKET_COUNT*2
            };
            size_t bad_test_key_count = sizeof(bad_test_keys) / sizeof(bad_test_keys[0]);

            // Create test objects corresponding to those keys

            TestObject** test_objects = new TestObject*[ test_key_count ];
            for (size_t ii = 0; ii < test_key_count; ++ii) {
                test_objects[ii] = new TestObject(test_keys[ii]);
            }

            // Validate they're initialized correctly

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                test_objects[ii]->validate_not_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), 0);

            // Push them all onto the table

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                table.insert(*test_objects[ii]);
            }

            // They're all intact and on there, right?

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                test_objects[ii]->validate_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), test_key_count);

            // Touch each object in turn

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                table.touch(*test_objects[ii]);
            }

            // They're still intact and on there, right?

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                test_objects[ii]->validate_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), test_key_count);

            // Make sure we can find them when we search

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                TestObject search(test_keys[ii]);
                search.validate_not_on_list();

                TestObject* found = table.find(search);
                TS_ASSERT_EQUALS(test_objects[ii], found);
                found->validate_on_list();

                const TestObject* c_found = table.find(search);
                TS_ASSERT_EQUALS(test_objects[ii], c_found);
                c_found->validate_on_list();
            }

            // Quick negative test, ensure we can't find bogus data

            for (size_t ii = 0; ii < bad_test_key_count; ++ii) {
                TestObject search(bad_test_keys[ii]);
                TS_ASSERT(!table.find(search));
            }

            // Remove them all

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                table.erase(*test_objects[ii]);
            }

            // They're gone, right?

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                test_objects[ii]->validate_not_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), 0);

            // Clean up

            for (size_t ii = 0; ii < test_key_count; ++ii) {
                delete test_objects[ii];
            }
            delete[] test_objects;
        }


        /*
         * Puts a lot of stuff in a table, ensures it doesn't fall apart
         */

        void testHeavyLoad()
        {
            TestTable table;

            size_t test_object_count = TEST_BUCKET_COUNT * 1000;
            TestObject** test_objects = new TestObject*[ test_object_count ];

            // Build some test data

            int next_key = 0;
            for (size_t ii = 0; ii < test_object_count; ++ii) {
                test_objects[ii] = new TestObject(next_key);
                next_key += 1 + random() % (TEST_BUCKET_COUNT*2);
            }

            // Put them all on the table

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                table.insert(*test_objects[ii]);
            }

            // They're still intact and on there, right?

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                test_objects[ii]->validate_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), test_object_count);

            // Make sure we can find them when we search

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                TestObject search(test_objects[ii]->key);
                search.validate_not_on_list();

                TestObject* found = table.find(search);
                TS_ASSERT_EQUALS(test_objects[ii], found);
                found->validate_on_list();

                const TestObject* c_found = table.find(search);
                TS_ASSERT_EQUALS(test_objects[ii], c_found);
                c_found->validate_on_list();
            }

            // Remove them all

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                table.erase(*test_objects[ii]);
            }

            // They're gone, right?

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                test_objects[ii]->validate_not_on_list();
            }
            TS_ASSERT_EQUALS(table.size(), 0);

            // Clean up

            for (size_t ii = 0; ii < test_object_count; ++ii) {
                delete test_objects[ii];
            }
            delete[] test_objects;
        }


        /*
         * Ensures that records with duplicate keys can all be found
         * using the find()/next_match() pattern.
         */

        void testDuplicates()
        {
            const uint32_t SMALL_BUCKET_COUNT = 1;
            util::endohash<TestObject,SMALL_BUCKET_COUNT> table;

            size_t matching_object_count = 10;
            size_t filler_object_count = matching_object_count;
            size_t total_object_count = matching_object_count + filler_object_count;
            TestObject** objects = new TestObject*[ total_object_count ];

            // Repeatedly insert matching objects then filler objects

            int matching_key = 123;
            int filler_key = 456;
            for (size_t ii = 0; ii < total_object_count; ) {
                table.insert(*( objects[ii++] = new TestObject(matching_key) ));
                table.insert(*( objects[ii++] = new TestObject(filler_key++) ));
            }

            // Walk the table looking for matching objects

            int found = 0;
            TestObject search(matching_key);
            for (const TestObject* walk = table.find(search); walk; walk = table.next_match(walk)) {
                found++;
            }
            TS_ASSERT_EQUALS(found, matching_object_count);

            // Clean up

            for (size_t ii = 0; ii < total_object_count; ++ii) {
                table.erase(*objects[ii]);
                delete objects[ii];
            }
            delete[] objects;
        }
};

