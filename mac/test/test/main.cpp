//
//  main.cpp
//  test
//
//  Created by chenliang on 13-12-24.
//  Copyright (c) 2013年 chenliang. All rights reserved.
//

#include "main.h"
int aaa = 4;
#include "MyClass.h"

int main(int argc, char** argv)
{
    printf("%d \n", aaa);
    MyNameSpace::MyClass mc;
    mc.MyFun();
    
    printf("%d \n", aaa);
    return 0;
}