/*
 * Copyright (c) 2014 NetApp, Inc.
 * All rights reserved.
 */

#include <cxxtest/TestSuite.h>
#include <util_container_test.h>
#include <util_endomap.h>

/*
 * Note the lack of main(): the unit test suite finds our methods
 * by reflection, such that all methods starting with the name "test"
 * within our class will be invoked during compile-time testing.
 */

class EndoMapTest : public CxxTest::TestSuite,
                    public IteratorTest
{
    public:
        void testSize();
        void testIterator();
};


void EndoMapTest::testSize()
{
  typedef TestElem<util::endomap_node, const uint32_t> Elem;
  typedef util::endomap<Elem, const uint32_t> TestMap;

  TestMap empty;
  const TestMap& const_empty = empty;

  TS_ASSERT(empty.size() == 0);
  TS_ASSERT(const_empty.size() == 0);

  TestMap map_1;
  Elem zero(0);

  TS_ASSERT(map_1.size() == 0);
  map_1.insert(zero);
  TS_ASSERT(map_1.size() == 1);
  map_1.erase(zero);
  TS_ASSERT(map_1.size() == 0);
}


void EndoMapTest::testIterator()
{
  typedef TestElem<util::endomap_node, const uint32_t> Elem;
  typedef util::endomap<Elem, const uint32_t> TestMap;

  TestMap empty;
  const TestMap& const_empty = empty;

  TS_ASSERT(empty.begin() == empty.end());
  TS_ASSERT(empty.begin() == const_empty.begin());
  TS_ASSERT(const_empty.begin() == empty.end());
  TS_ASSERT(const_empty.begin() == const_empty.end());

  TS_ASSERT((countForward<TestMap::iterator,TestMap::iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countForward<TestMap::iterator,TestMap::const_iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countForward<TestMap::const_iterator,TestMap::const_iterator>(empty.begin(), empty.end()) == 0));

  TS_ASSERT((countBackward<TestMap::iterator,TestMap::iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countBackward<TestMap::iterator,TestMap::const_iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countBackward<TestMap::const_iterator,TestMap::const_iterator>(empty.begin(), empty.end()) == 0));

  TestMap map_1;
  const TestMap& const_map_1 = map_1;

  TS_ASSERT(map_1.begin() == map_1.end());
  TS_ASSERT(map_1.begin() == const_map_1.end());
  TS_ASSERT(const_map_1.begin() == map_1.end());
  TS_ASSERT(const_map_1.begin() == const_map_1.end());

  Elem zero(0);
  map_1.insert(zero);

  TS_ASSERT(map_1.begin() != map_1.end());
  TS_ASSERT(map_1.begin() != const_map_1.end());
  TS_ASSERT(const_map_1.begin() != map_1.end());
  TS_ASSERT(const_map_1.begin() != const_map_1.end());

  TS_ASSERT((countForward<TestMap::iterator,TestMap::iterator>(map_1.begin(), map_1.end()) == 1));
  TS_ASSERT((countForward<TestMap::iterator,TestMap::const_iterator>(map_1.begin(), map_1.end()) == 1));
  TS_ASSERT((countForward<TestMap::const_iterator,TestMap::const_iterator>(map_1.begin(), map_1.end()) == 1));

  TS_ASSERT((countBackward<TestMap::iterator,TestMap::iterator>(map_1.begin(), map_1.end()) == 1));
  TS_ASSERT((countBackward<TestMap::iterator,TestMap::const_iterator>(map_1.begin(), map_1.end()) == 1));
  TS_ASSERT((countBackward<TestMap::const_iterator,TestMap::const_iterator>(map_1.begin(), map_1.end()) == 1));

  map_1.erase(zero);

  TS_ASSERT(map_1.begin() == map_1.end());
  TS_ASSERT(map_1.begin() == const_map_1.end());
  TS_ASSERT(const_map_1.begin() == map_1.end());
  TS_ASSERT(const_map_1.begin() == const_map_1.end());
}
