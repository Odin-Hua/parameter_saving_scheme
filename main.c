#include <stdio.h>
#include <stdlib.h>
#include "ins_util_config.h"

/*
    剩余功能:
    1. 枚举校验, 自定义校验
    2. 支持自定义数据结构
    3. 写入时校验
*/
static int32_t test_case_range_check(void);

int main()
{
    config_val_t val;

    test_case_range_check();

    system("pause");
    return 0;
}

static int32_t test_case_range_check(void)
{
    config_val_t val = {
        .version        = (uint32_t)1,
        .test_uint8_t   = (uint8_t)6,
        .test_int8_t    = (int8_t)30,
        .test_uint32_t  = (uint32_t)6,
        .test_int32_t   = (int32_t)30,
        .test_uint16_t  = (uint16_t)6,
        .test_int16_t   = (int16_t)30,
        .test_fp32_t    = (fp32)30,
        .test_fp64_t    = (fp64)30,
        .test_uint64_t  = (uint64_t)6,
        .test_int64_t   = (int64_t)30,
    };

    Ins_Unit_Config_Clear();                            //remove file
    if (Ins_Unit_Config_Is_Exist() == 0) {
        Ins_Unit_Config_Creat_By_Default_Val();         //create file
    }
    Ins_Unit_Config_Show_Config_Val();                  //show default val
    Ins_Unit_Config_Write_Config_File(&val);            //write error val
    Ins_Unit_Config_Show_Config_Val();                  //show error val
    Ins_Unit_Config_Correct_Val();                      //correct val
    Ins_Unit_Config_Show_Config_Val();                  //show val affter correct
    Ins_Unit_Config_Correct_Val();                      //correct val again

    return 0;
}