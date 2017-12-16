/*
 * Copyright (c) 2014 NetApp, Inc.
 * All rights reserved.
 */

#include <cxxtest/TestSuite.h>
#include <util_container_test.h>
#include <util_endolist.h>

/*
 * Note the lack of main(): the unit test suite finds our methods
 * by reflection, such that all methods starting with the name "test"
 * within our class will be invoked during compile-time testing.
 */

class EndoListTest : public CxxTest::TestSuite,
                     public IteratorTest
{
    public:
        void testSize();
        void testIterator();
        void testSplice();
};


void EndoListTest::testSize()
{
  typedef TestElem<util::endolist_node, const uint32_t> Elem;
  typedef util::endolist<Elem> TestList;

  TestList empty;
  const TestList& const_empty = empty;

  TS_ASSERT(empty.size() == 0);
  TS_ASSERT(const_empty.size() == 0);

  TestList list_1;
  Elem zero(0);

  TS_ASSERT(list_1.size() == 0);
  list_1.insert(list_1.begin(), zero);
  TS_ASSERT(list_1.size() == 1);
  list_1.erase(list_1.begin());
  TS_ASSERT(list_1.size() == 0);
}


void EndoListTest::testIterator()
{
  typedef TestElem<util::endolist_node, const uint32_t> Elem;
  typedef util::endolist<Elem> TestList;

  TestList empty;
  const TestList& const_empty = empty;

  TS_ASSERT(empty.begin() == empty.end());
  TS_ASSERT(const_empty.begin() == const_empty.end());

  TS_ASSERT((countForward<TestList::iterator,TestList::iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countForward<TestList::iterator,TestList::const_iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countForward<TestList::const_iterator,TestList::const_iterator>(empty.begin(), empty.end()) == 0));

  TS_ASSERT((countBackward<TestList::iterator,TestList::iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countBackward<TestList::iterator,TestList::const_iterator>(empty.begin(), empty.end()) == 0));
  TS_ASSERT((countBackward<TestList::const_iterator,TestList::const_iterator>(empty.begin(), empty.end()) == 0));

  TestList list_1;
  const TestList& const_list_1 = list_1;

  TS_ASSERT(list_1.begin() == list_1.end());
  TS_ASSERT(const_list_1.begin() == const_list_1.end());

  Elem zero(0);
  list_1.push_back(zero);

  TS_ASSERT(list_1.begin() != list_1.end());
  TS_ASSERT(const_list_1.begin() != const_list_1.end());

  TS_ASSERT((countForward<TestList::iterator,TestList::iterator>(list_1.begin(), list_1.end()) == 1));
  TS_ASSERT((countForward<TestList::iterator,TestList::const_iterator>(list_1.begin(), list_1.end()) == 1));
  TS_ASSERT((countForward<TestList::const_iterator,TestList::const_iterator>(list_1.begin(), list_1.end()) == 1));

  TS_ASSERT((countBackward<TestList::iterator,TestList::iterator>(list_1.begin(), list_1.end()) == 1));
  TS_ASSERT((countBackward<TestList::iterator,TestList::const_iterator>(list_1.begin(), list_1.end()) == 1));
  TS_ASSERT((countBackward<TestList::const_iterator,TestList::const_iterator>(list_1.begin(), list_1.end()) == 1));

  list_1.clear();

  TS_ASSERT(list_1.begin() == list_1.end());
  TS_ASSERT(const_list_1.begin() == const_list_1.end());
}

void EndoListTest::testSplice()
{
  typedef TestElem<util::endolist_node, const uint32_t> Elem;
  typedef util::endolist<Elem, &Elem::node> TestList;

  int j;

  // setup
  TestList list;
  Elem e1(1), e2(2), e3(3), e4(4), e5(5);

  list.insert(list.end(), e1);
  list.insert(list.end(), e2);
  list.insert(list.end(), e3);
  list.insert(list.end(), e4);
  list.insert(list.end(), e5);

  TS_ASSERT_EQUALS(list.size(), 5);

  /////////////////////////////////////////////////////////////////////////////

  // Test in place splice


  { // move 1st two elements to end
    list.splice(list.end(), list, TestList::iterator(&e1.node), TestList::iterator(&e3.node));

    uint32_t test_results[5] = {3, 4, 5, 1, 2};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results[j]);
    }
  }

  { // move them back
    list.splice(list.begin(), list, TestList::iterator(&e1.node), list.end());

    uint32_t test_results[5] = {1, 2, 3, 4, 5};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results[j]);
    }
  }

  { // splice the entire list back to itself
    list.splice(list.end(), list, list.begin(), list.end());

    uint32_t test_results[5] = {1, 2, 3, 4, 5};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results[j]);
    }
  }

  { // splice to the middle of the list
    list.splice(TestList::iterator(&e2.node), list, TestList::iterator(&e3.node), TestList::iterator(&e5.node));

    uint32_t test_results[5] = {1, 3, 4, 2, 5};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results[j]);
    }
  }

  // restore the list for subsequent tests
  list.splice(TestList::iterator(&e3.node), list, TestList::iterator(&e2.node), TestList::iterator(&e5.node));

  /////////////////////////////////////////////////////////////////////////////

  // Test different list splice

  // setup
  TestList otherlist;
  Elem e6(6), e7(7), e8(8), e9(9), e10(10);

  otherlist.insert(otherlist.end(), e6);
  otherlist.insert(otherlist.end(), e7);
  otherlist.insert(otherlist.end(), e8);
  otherlist.insert(otherlist.end(), e9);
  otherlist.insert(otherlist.end(), e10);

  TS_ASSERT_EQUALS(otherlist.size(), 5);

  { // move 1st two elements of otherlist to list end
    list.splice(list.end(), otherlist, TestList::iterator(&e6.node), TestList::iterator(&e8.node));

    TS_ASSERT_EQUALS(list.size(), 7);
    TS_ASSERT_EQUALS(otherlist.size(), 3);

    uint32_t test_results_list[7] = {1, 2, 3, 4, 5, 6, 7};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_list[j]);
    }

    uint32_t test_results_otherlist[3] = {8, 9, 10};
    j = 0;
    for (TestList::iterator i=otherlist.begin(); i != otherlist.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_otherlist[j]);
    }
  }

  { // move them back
    otherlist.splice(otherlist.begin(), list, TestList::iterator(&e6.node), list.end());

    TS_ASSERT_EQUALS(list.size(), 5);
    TS_ASSERT_EQUALS(otherlist.size(), 5);

    uint32_t test_results_list[5] = {1, 2, 3, 4, 5};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_list[j]);
    }

    uint32_t test_results_otherlist[5] = {6, 7, 8, 9, 10};
    j = 0;
    for (TestList::iterator i=otherlist.begin(); i != otherlist.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_otherlist[j]);
    }
  }

  { // splice to the middle of the list
    list.splice(TestList::iterator(&e3.node), otherlist, TestList::iterator(&e7.node), TestList::iterator(&e9.node));

    uint32_t test_results_list[7] = {1, 2, 7, 8, 3,  4, 5};
    j = 0;
    for (TestList::iterator i=list.begin(); i != list.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_list[j]);
    }

    uint32_t test_results_otherlist[3] = {6, 9, 10};
    j = 0;
    for (TestList::iterator i=otherlist.begin(); i != otherlist.end(); ++i, ++j) {
      TS_ASSERT_EQUALS(i->key, test_results_otherlist[j]);
    }
  }

  // cleanup
  list.erase(list.begin(), list.end());
  otherlist.erase(otherlist.begin(), otherlist.end());
}

