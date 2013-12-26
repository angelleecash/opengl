//
//  MyClass.cpp
//  test
//
//  Created by chenliang on 13-12-24.
//  Copyright (c) 2013年 chenliang. All rights reserved.
//

#include "MyClass.h"

extern int aaa;

void MyNameSpace::MyClass::MyFun()
{
    aaa += 1;
}