/*******************************************************************************
* Copyright 2015      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：Jr
* 完成日期：2015-1-16
* 编译环境：D:\software\IAR_for_ARM\arm   IAR for ARM 6.50 
* 
* 源代码说明：rand函数是真正的随机数生成器，而srand()会设置供rand()使用的随机数种子。
*             如果在第一次调用rand()之前没有调用srand()，那么系统会为你自动调用srand(1)。
*             而使用同种子相同的数调用 rand()会导致相同的随机数序列被生成。
*           
*            #include <stdlib.h>
*
*            #if _ILONG
*                #define RAND_MAX      0x3fff,ffff (...)
*            #else  
*                #define RAND_MAX      0x7fff (32767)
*            #endif  
*           
*            库函数中系统提供了两个函数用于产生随机数：srand()和rand()。 原型为：
*            函数一：int rand(void)；
*            返回一个[0,RAND_MAX]间的随机整数。
*            函数二：void srand(unsigned seed)；
*            参数seed是rand()的种子，用来初始化rand()的起始值。
*
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/
__IO u32 randomSeedValue = 0u;
__IO u32 randomValue = 0u;

u32 randomSeedValue4bit = 0u;




/*******************************************************************************
* Description : randomSeed() initializes the pseudo-random number generator, 
*               causing it to start at an arbitrary point in its random sequence. This sequence, while very long, and random, is always the same.
*               If it is important for a sequence of values generated by random() to differ, on subsequent executions of a sketch, use randomSeed() 
*               to initialize the random number generator with a fairly random input, such as analogRead() on an unconnected pin.
*               Conversely, it can occasionally be useful to use pseudo-random sequences that repeat exactly. 
*               This can be accomplished by calling randomSeed() with a fixed number, before starting the random sequence.
* Syntax      : 
* Parameters I: long, int - pass a number to generate the seed.
* Parameters O: 
* return      : 
*******************************************************************************/
void randomSeed(unsigned int seed)
{
	/* Check the parameters */
	ASSERT (seed != 0);
    
    if (seed != 0) 
    {
        srand(seed);
    }
    else
    {
    }    
}

/*******************************************************************************
* Description : The random function generates pseudo-random numbers.
* Note:
* If it is important for a sequence of values generated by random() to differ, 
* on subsequent executions of a sketch, use randomSeed() to initialize the random number generator with a fairly random input, 
* such as analogRead() on an unconnected pin.
* Conversely, it can occasionally be useful to use pseudo-random sequences that repeat exactly. 
* This can be accomplished by calling randomSeed() with a fixed number, before starting the random sequence.
* Syntax      : 
* Parameters I: max - upper bound of the random value, exclusive
* Parameters O: 
* return      : a random number between 0 and max-1 (long)
*******************************************************************************/
u32 random(u32 howbig)
{
	/* Check the parameters */
	ASSERT (howbig < RAND_MAX);
    
    if (0u == howbig) 
    {
        return 0u;
    }
    
    return (rand() % howbig);
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u32 randomRange(u32 howsmall, u32 howbig)
{
    u32 diff = 0u;
    
	/* Check the parameters */
	ASSERT (howbig < RAND_MAX);
    ASSERT (howsmall < howbig);
    
//    if (howsmall >= howbig) 
//    {
//        return howsmall;
//    }
    
    diff = howbig - howsmall;
    
    return (random(diff) + howsmall);
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void randomTest(void)
{
	/* Check the parameters */
	//ASSERT ();
	
//    float AD_value = 0;
    randomSeedValue4bit = 0u;
    randomSeedValue4bit = 0u;
    
    randomSeedValue4bit = randomSeedValue%0x0F;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<4) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<8) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<12) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<16) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<20) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<24) | randomSeedValue4bit;
    delayMs(10);
    randomSeedValue4bit = ((randomSeedValue%0x0F)<<28) | randomSeedValue4bit;
    delayMs(10);
    
    //AD_value = (float)randomSeedValue/4096*3.3;
    //printf("AD:  %f \r\n", AD_value);
    //printf("randomValue LSB 4bit: %d \r\n", randomSeedValue4bit);
    randomSeed(randomSeedValue4bit);
    randomValue = random(0x3ffffffe);
    printf("randomValue: %d \r\n", randomValue);
    delayMs(100);
}


















