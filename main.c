#include <stdio.h>
#include <stdlib.h>
#include "util_config.h"

/*
    剩余功能:
    1. 枚举校验
    2. 支持自定义数据结构
    3. 写入时校验
*/
/*范围校验测试用例*/
static int32_t test_case_range_check(void);
/*自定义校验以及参数区版本更新测试用例*/
static int32_t test_case_version_update_custom_check(void);

int main()
{
    test_case_version_update_custom_check();

    system("pause");
    return 0;
}

/*范围校验测试用例*/
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

    Unit_Config_Clear();                            //remove file
    if (Unit_Config_Is_Exist() == 0) {
        Unit_Config_Creat_By_Default_Val();         //create file
    }
    Unit_Config_Show_Config_Val();                  //show default val
    Unit_Config_Write_Config_File(&val);            //write error val
    Unit_Config_Show_Config_Val();                  //show error val
    Unit_Config_Correct_Val();                      //correct val
    Unit_Config_Show_Config_Val();                  //show val affter correct
    Unit_Config_Correct_Val();                      //correct val again

    return 0;
}

/*自定义校验以及参数区版本更新测试用例*/
static int32_t test_case_version_update_custom_check(void)
{
    #define STEP    (1)
    /*==================first===================*/
    /*modify version and delete config val(update_test);
      change STEP 1, recompile*/
#if (STEP == 1)
    Unit_Config_Clear();                            //remove file
    if (Unit_Config_Is_Exist() == 0) {
        Unit_Config_Creat_By_Default_Val();         //create file
    }
    Unit_Config_Show_Config_Val();                  //show default val
#endif
    /*==================second==================*/
    /*simulate upgrade, modify version and add config val(update_test);
      change STEP 3, recompile*/

    /*==================thrid==================*/
#if (STEP == 3)
    Unit_Config_Correct_Val();                      //correct val
    Unit_Config_Show_Config_Val();                  //show default val
#endif

    /*==================fourth==================*/
    /*simulate demote, modify version and delete config val(update_test);
    change STEP 5, recompile*/

    /*==================fifth==================*/
#if (STEP == 5)
    Unit_Config_Correct_Val();                      //correct val
    Unit_Config_Show_Config_Val();                  //show default val
#endif

    return 0;
}