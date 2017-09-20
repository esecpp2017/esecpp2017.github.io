// LibArray.cpp : 定义控制台应用程序的入口点。
//

// 实验内容：
// 1：使用C语言实现一个长度可扩充的数组（包含必要的数据结构及函数）；
// 2：要求能存放任意类型的数据（建议先实现存储整形的代码，之后改写成适应任意类型的代码）；
// 3：所写程序需能通过测试程序
// 4：除本文件（测试文件）之外，其他文件（如CLibArray.cpp及CLibArray.h文件）、以及工程由同学自己建立。过程中可翻书，可查看msdn。

// 实验目的：
// 1：熟悉相关的指针操作, 复习动态内存的相关操作.
// 2：理解C程序如何实现数据类型和围绕数据类型上操作的集合
// 3：为未来理解类实现的数组vector做准备

// 只提交CLibArray.cpp及CLibArray.h

#include "stdafx.h"

#include <assert.h>

#include "CLibArray.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CArray array;
    array_initial(array); 

    array_recap(array, 10); 
    assert(array_capacity(array) == 10); 

    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 20; ++i)
    {
        array_append(array, i); 
    }
    assert(array_size(array) == 20); 
    
    for (int i = 0; i < array_size(array); ++i)
    {
        assert(array_at(array, i) == i); 
    }

    //////////////////////////////////////////////////////////////////////////
    CArray array2, array3; 
    array_initial(array2); 
    array_initial(array3); 

    array_copy(array, array2); 
    assert(array_compare(array, array2) == true); 
    array_copy(array, array3); 
    assert(array_compare(array, array3) == true); 

    //////////////////////////////////////////////////////////////////////////
    array_insert(array2, 2, 3); 
    assert(array_compare(array, array2) == false); 

    //////////////////////////////////////////////////////////////////////////
    array_at(array3, 2) = 5; 
    assert(array_compare(array, array3) == false); 

    //////////////////////////////////////////////////////////////////////////
    array_destroy(array); 
    array_destroy(array2); 
    array_destroy(array3); 

	return 0;
}

