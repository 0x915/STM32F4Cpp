# STM32F4Cpp
 硬件 STM32F411CEU6 
 开发 CLion-2021.1
 编译 GCC-ARM-10.3-2021.10
 调试 OpenOCD-20211118-0.11.0

佛系开发者，需要什么学什么，C++ C Python PowerShell 日常混用，大概率会出点bug或算法效率问题，见谅

该项目为闲没事干给手上的核心板写的

编写初衷：CircuitPython空间占用也太大了！还是用C++自己封装HAL和外设驱动吧

ASCII字库[0x20,0x7E] 8x6/16x10基于Minecraft字体点阵 16x8基于Auraka點陣宋 修改

字库二维数组 定义了 字符宽度 位于字符末列，外设驱动会根据字符宽度绘制字体

