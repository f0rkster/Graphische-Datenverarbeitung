#include "UnitTestMatrix4x4.h"
#include "Matrix4x4.h"

#include <iostream>
#include <string>

using namespace std;

void UnitTestMatrix4x4()
{
    //Matrix4x4DefaultConstructorTest
    {
        Matrix4x4 test;
        string testName = "Matrix4x4DefaultConstructorTest";
        float expected[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };

        float actual[4][4];

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                actual[row][col] = test.GetValue(row, col);
            }
        }

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual[row][col] != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Matrix4x416AttributesConstructorTest
    {
        Matrix4x4 test(1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4);
        string testName = "Matrix4x416AttributesConstructorTest";
        float expected[4][4] = { {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4} };

        float actual[4][4];

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                actual[row][col] = test.GetValue(row, col);
            }
        }

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual[row][col] != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

     //Matrix4x4AddTest
    {
        Matrix4x4 test1(1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4);
        Matrix4x4 test2(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
        string testName = "Matrix4x4AddTest";
        float expected[4][4] = { {2, 3, 4, 5}, {2, 3, 4, 5}, {2, 3, 4, 5}, {2, 3, 4, 5} };

        Matrix4x4 actual = test1+test2;

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual.GetValue(row, col) != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Matrix4x4AddAndSetTest
    {
        Matrix4x4 test1(1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4);
        Matrix4x4 test2(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
        string testName = "Matrix4x4AddAndSetTest";
        float expected[4][4] = { {2, 3, 4, 5}, {2, 3, 4, 5}, {2, 3, 4, 5}, {2, 3, 4, 5} };

        test1 += test2;
        Matrix4x4 actual = test1;

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual.GetValue(row, col) != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Matrix4x4MulTest
    {
        Matrix4x4 test1(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
        Matrix4x4 test2(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        string testName = "Matrix4x4MulTest";
        float expected[4][4] = { {2, 4, 6, 8}, {2, 4, 6, 8}, {2, 4, 6, 8}, {2, 4, 6, 8} };

        Matrix4x4 actual = test1 * test2;

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual.GetValue(row, col) != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }

    //Matrix4x4MulAndSetTest
    {
        Matrix4x4 test1(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
        Matrix4x4 test2(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        string testName = "Matrix4x4MulAndSetTest";
        float expected[4][4] = { {2, 4, 6, 8}, {2, 4, 6, 8}, {2, 4, 6, 8}, {2, 4, 6, 8} };
        
        test1 *= test2;
        Matrix4x4 actual = test1;

        bool hasSucceded = true;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (actual.GetValue(row, col) != expected[row][col] || !hasSucceded)
                {
                    hasSucceded = false;
                }
            }
        }

        if (hasSucceded)
        {
            cout << "PASSED! " << testName << endl;
        }
        else
        {
            cout << "FAILED!!! " << testName << endl;
        }
    }
}
