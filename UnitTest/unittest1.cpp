#include "stdafx.h"
#include "CppUnitTest.h"
#include "../xll/xll.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace xll;

namespace UnitTest
{		
	TEST_CLASS(OPER)
	{
	public:
		
		TEST_METHOD(OPERConstructors)
		{
            OPER12 o;
            Assert::IsTrue (o.size() == 0);
            Assert::IsTrue (o.xltype == xltypeMissing);
            OPER12 o2(o);
            Assert::IsTrue (o2.xltype == xltypeMissing);
            Assert::IsTrue (o == o2);
            o = o2;
            Assert::IsTrue (o.xltype == xltypeMissing);
            Assert::IsTrue (o == o2);
            Assert::IsTrue (!(o != o2));
        }
	};
}