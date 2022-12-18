#include <stdio.h>
#include <stdlib.h>
#include "ins_util_config.h"

/*
    1. 支持多种校验方式：范围校验, 枚举校验, 自定义校验
    2. 支持自定义数据结构
*/

int main()
{
    config_val_t val;

    if (Ins_Unit_Config_Is_Exist()) {
        Ins_Unit_Config_Read_Config_File(&val);
    } else {
        Ins_Unit_Config_Creat_By_Default_Val();
    }
    Ins_Unit_Config_Show_Config_Val();

    system("pause");
    return 0;
}
