#include "sysinit.h" 

/***********************************
 * 函数名：error
 * 功   能：错误闪灯
 * 参   数：无
 * 注意事项：无
**********************************/
void error()
{
    volatile uint32_t i = 0;
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    while (1)
    {
        for(i = 500000; i > 0; i--)
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
        }
        for(i = 500000; i > 0; i--)
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
        }
    }
    
}
/***********************************
 * 函数名：MY_Systeminit
 * 功   能：配置系统时钟频率
 * 参   数：无
 * 注意事项：核心电压一定要设置为1
**********************************/
void MY_Systeminit()
{
    //关闭看门狗
    MAP_WDT_A_holdTimer();

    //开启FPU（float point unit)
    MAP_FPU_enableModule();//使能fpu
    MAP_FPU_enableLazyStacking();

    //配置时钟引脚，使用外部时钟源
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION); //Low
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION); //High
    MAP_CS_setExternalClockSourceFrequency(32768, 48000000);//设置两个时钟源的频率

    //改变电源核心电压 必须改变
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /*更改闪存控制器使用的等待状态数用于读取操作。
    当改变时钟频率范围时，必须使此函数可读闪存
    通俗来讲就是CPU跑的太快了，Flash跟不上，让CPU等等它*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0,1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1,1);
   
   //设置晶体驱动等级
    CS_startHFXT(false);          
    //bool CS_startHFXT(bool bypassMode)  HFXT的bypass = 0
    CS_startLFXT(CS_LFXT_DRIVE3); //驱动等级3
	//bool CS_startLFXT(uint32_t xtDrive)  LFXT的xtDRIver = 3

    //初始化系统时钟(MCLK)为48MHZ　初始化子系统时钟为48MHZ
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  //48MHz   16分频时，滴答延时可达到最长
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); //48MHz

}

