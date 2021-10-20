#include "Float4.h"
#include <iostream>
#include <string>

using namespace std;

void UnitTestFloat4() {
    //Float4DefaultConstructorTest
    {
        Float4 actual;
        string testName = "Float4DefaultConstructorTest";
        Float4 expected( 0, 0, 0, 0 );

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4GetTest
    {
        Float4 test(1, 2, 3, 4);
        string testName = "Float4GetTest";
        float expected = 4;
        float actual = test[3];

        if (expected == actual)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4AddTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(1, 1, 1, 1);
        string testName = "Float4AddTest";
        float expected[4] = { 2, 3, 4, 5 };
        Float4 actual = test1 + test2;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4AddAndSetTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(1, 1, 1, 1);
        string testName = "Float4AddAndSetTest";
        float expected[4] = { 2, 3, 4, 5 };
        test1 += test2;
        Float4 actual = test1;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4SubTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(1, 1, 1, 1);
        string testName = "Float4SubTest";
        float expected[4] = { 0, 1, 2, 3 };
        Float4 actual = test1 - test2;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4SubAndSetTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(1, 1, 1, 1);
        string testName = "Float4SubAndSetTest";
        float expected[4] = { 0, 1, 2, 3 };
        test1 -= test2;
        Float4 actual = test1;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4MulTest
    {
        Float4 test1(1, 2, 3, 4);
        float test2 = 2;
        string testName = "Float4MulTest";
        float expected[4] = { 2, 4, 6, 8 };
        Float4 actual = test1 * test2;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4MulAndSetTest
    {
        Float4 test1(1, 2, 3, 4);
        float test2 = 2;
        string testName = "Float4MulAndSetTest";
        float expected[4] = { 2, 4, 6, 8 };
        test1 *= test2;
        Float4 actual = test1;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4DotTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(2, 2, 2, 2);
        string testName = "Float4DotTest";
        float expected = 20;
        float actual = test1.Dot(test2);

        if (expected == actual)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4CrossTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(2, 2, 2, 2);
        string testName = "Float4CrossTest";
        Float4 expected(-2, -2, 6, -2);
        Float4 actual = test1.Cross(test2);

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4CrossAndSetTest
    {
        Float4 test1(1, 2, 3, 4);
        Float4 test2(2, 2, 2, 2);
        string testName = "Float4CrossAndSetTest";
        Float4 expected(-2, -2, 6, -2);

        test1 = test1.CrossAndSet(test2);
        Float4 actual = test1;

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
            actual.Print();
        }
    }

    //Float4GetLengthTest
    {
        Float4 test1(1, 2, 3, 4);
        string testName = "Float4GetLengthTest";
        float expected = 5.48;
        float actual = test1.GetLength();
        float rounded = ((int)(actual * 100 + .5) / 100.0);
        actual = rounded;

        if (expected == actual)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4GetNormalized
    {
        Float4 test1(0, 0, 0, 4);
        string testName = "Float4GetNormalized";
        Float4 expected(0, 0, 0, 1);
        Float4 actual = test1.GetNormalized();

        if (expected[0] == actual[0] &&
            expected[1] == actual[1] &&
            expected[2] == actual[2] &&
            expected[3] == actual[3])
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Float4Normalize
    {
        Float4 test1(0, 0, 0, 4);
        string testName = "Float4Normalize";
        float expected = 1;
        test1.Normalize();
        float actual = test1.GetLength();

        if (expected == actual)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

}
