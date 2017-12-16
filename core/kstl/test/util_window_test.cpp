/*
 * Copyright (c) 2015 NetApp, Inc.
 * All rights reserved.
 */

#include <cxxtest/TestSuite.h>
#include <util_window.h>
#include <stdio.h>


/*
 * Note the lack of main(): the unit test suite finds our methods
 * by reflection, such that all methods starting with the name "test"
 * within our class will be invoked during compile-time testing.
 */

class WindowTest : public CxxTest::TestSuite
{
    public:
        void testSlidingWindow();
};


/*
 * Exercise a basic sliding window.
 */

void WindowTest::testSlidingWindow()
{
    printf("\n");

    util::window<4,uint32_t> win;
    TS_ASSERT(win.size() == 0 && win.average() == 0 && win.maximum() == 0 && win.total() == 0 && win.delta() == 0);

    win += 5;   // window should now contain {5}
    TS_ASSERT(win.size() == 1 && win.average() == 5 && win.maximum() == 5 && win.total() == 5 && win.delta() == 0);

    win += 7;   // window should now contain {5, 7}
    TS_ASSERT(win.size() == 2 && win.average() == 6 && win.maximum() == 7 && win.total() == 12 && win.delta() == 2);

    win += 12;  // window should now contain {5, 7, 12}
    TS_ASSERT(win.size() == 3 && win.average() == 8 && win.maximum() == 12 && win.total() == 24 && win.delta() == 7);

    win += 4;   // window should now contain {5, 7, 12, 4}
    TS_ASSERT(win.size() == 4 && win.average() == 7 && win.maximum() == 12 && win.total() == 28 && win.delta() == -1);

    win += 17;  // window should now contain {7, 12, 4, 17}
    TS_ASSERT(win.size() == 4 && win.average() == 10 && win.maximum() == 17 && win.total() == 40 && win.delta() == 10);

    for (int ii = 0; ii < 1000; ++ii) {
        win += rand();
    }

    win += 2;
    win += 4;
    win += 6;
    win += 8;
    TS_ASSERT(win == 5);   // these last four insertions should swamp the noise
    printf("sliding window averager checks out\n");
}

