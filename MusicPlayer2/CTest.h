#pragma once

//���ڲ��ԵĴ���
class CTest
{
public:
	CTest();
	~CTest();

	static void Test();

private:
	static void TestStringMatch();
	static void TestCrash();
	static void TestShortCut();
	static void TestCommon();
    static void TestOSUFile();
    static void TestReg();
    static void TestMediaLib();
};

