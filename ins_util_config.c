#include <stdio.h>
#include <stdlib.h>
#include "ins_util_config.h"

#define CONFIG_PATH     "config"            /*参数区文件*/
typedef struct {
    int32_t     exist_flag;
} g_config_ctrl_t;                          /*参数区全局操作变量定义*/

/*参数区数值描述: CONFIG_TYPE_e, CONFIG_VALUE_TYPE_e, length, offset*/
CONFIG_UNIT_VALUE_DESCRIBE_t g_config_val_describe_table[] = {
    [CONFIG_TYPE_VERSION]      = {CONFIG_VALUE_TYPE_UINT32,    4,   0},
    [CONFIG_TYPE_TEST_UINT8]   = {CONFIG_VALUE_TYPE_UINT8,     1,   4},
    [CONFIG_TYPE_TEST_INT8]    = {CONFIG_VALUE_TYPE_INT8,      1,   5},
    [CONFIG_TYPE_TEST_UINT32]  = {CONFIG_VALUE_TYPE_UINT32,    4,   6},
    [CONFIG_TYPE_TEST_INT32]   = {CONFIG_VALUE_TYPE_INT32,     4,   10},
    [CONFIG_TYPE_TEST_UINT16]  = {CONFIG_VALUE_TYPE_UINT16,    2,   14},
    [CONFIG_TYPE_TEST_INT16]   = {CONFIG_VALUE_TYPE_INT16,     2,   16},
    [CONFIG_TYPE_TEST_FP32]    = {CONFIG_VALUE_TYPE_FP32,      4,   18},
    [CONFIG_TYPE_TEST_FP64]    = {CONFIG_VALUE_TYPE_FP64,      8,   22},
    [CONFIG_TYPE_TEST_UINT64]  = {CONFIG_VALUE_TYPE_UINT64,    8,   30},
    [CONFIG_TYPE_TEST_INT64]   = {CONFIG_VALUE_TYPE_INT64,     8,   38},
};
/*参数区默认值*/
config_val_t g_config_default_val = {
    .version        = (uint32_t)1,
    .test_uint8_t   = (uint8_t)143,                         /*0x8f*/
    .test_int8_t    = (int8_t)-15,                          /*-0xf*/          
    .test_uint32_t  = (uint32_t)2415919103,                 /*0x8FFFFFFF*/
    .test_int32_t   = (int32_t)-1048575,                    /*-0xFFFFF*/
    .test_uint16_t  = (uint16_t)36863,                      /*0x8FFF*/
    .test_int16_t   = (int16_t)-4095,                       /*-0xFFF*/
    .test_fp32_t    = (fp32)11.1100,
    .test_fp64_t    = (fp64)-10241024.1024,
    .test_uint64_t  = (uint64_t)10376293541461623000ULL,    /*0x8FFFFFFFFFFFFFFF*/ 
    .test_int64_t   = (int64_t)-1152921504606847000LL,      /*-0xFFFFFFFFFFFFFFF*/
};
/*参数区全局操作变量*/
g_config_ctrl_t g_config_ctrl;

/*参数区文件是否存在*/
int32_t Ins_Unit_Config_Is_Exist(void)
{
    FILE *file;

    if (g_config_ctrl.exist_flag) {
        return 1;
    }
	file = fopen(CONFIG_PATH, "rb+");
	if (file > 0) {
        fclose(file);
        g_config_ctrl.exist_flag = 1;
    } else {
        g_config_ctrl.exist_flag = 0;
    }

	return g_config_ctrl.exist_flag;
}

/*删除参数区文件*/
int32_t Ins_Unit_Config_Clear(void)
{
    int ret;

    ret = remove(CONFIG_PATH);
    if (ret < 0) {
        printf("Ins_Unit_Config_Clear error\n");
    } else {
        g_config_ctrl.exist_flag = 0;
        printf("Ins_Unit_Config_Clear success\n");
    }

    return ret;
}

/*用默认值创建参数区文件*/
int32_t Ins_Unit_Config_Creat_By_Default_Val(void)
{
    int ret;
    FILE *file;

    file = fopen(CONFIG_PATH, "wb+");
    if (file < 0) {
        printf("Ins_Unit_Config_Creat_By_Default_Val create file error\n");
        return -1;
    }
    ret = fwrite(&g_config_default_val, sizeof(config_val_t), 1, file);
    if (ret < 0) {
        printf("Ins_Unit_Config_Creat_By_Default_Val write file error\n");
    }
    fflush(file);
    fclose(file);

    return ret;
}

/*读取某个参数的数值*/
int32_t Ins_Unit_Config_Read_Val(CONFIG_TYPE_e type, void *val)
{
    int ret;
    FILE *file;

    if (val == NULL || type >= CONFIG_TYPE_NUM) {
        printf("Ins_Unit_Config_Read_Val error input param\n");
        return -1;
    }

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("Ins_Unit_Config_Read_Val open file error\n");
        return -1;
    }
    fseek(file, g_config_val_describe_table[type].offset, SEEK_SET);
    ret = fread(val, g_config_val_describe_table[type].length, 1, file);
    fclose(file);

    return ret;
}

/*写入数值到某个参数*/
int32_t Ins_Unit_Config_Write_Val(CONFIG_TYPE_e type, void *val)
{
    int ret;
    FILE *file;

    if (val == NULL || type >= CONFIG_TYPE_NUM) {
        printf("Ins_Unit_Config_Write_Val error input param\n");
        return -1;
    }

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("Ins_Unit_Config_Write_Val open file error\n");
        return -1;
    }
    fseek(file, g_config_val_describe_table[type].offset, SEEK_SET);
    ret = fwrite(val, g_config_val_describe_table[type].length, 1, file);
    fflush(file);
    fclose(file);

    return ret;
}

/*读取整个参数区*/
int32_t Ins_Unit_Config_Read_Config_File(config_val_t *val)
{
    int ret;
    FILE *file;

    if (val == NULL) {
        printf("Ins_Unit_Config_Read_Val error input param\n");
        return -1;
    }
    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("Ins_Unit_Config_Read_Val open file error\n");
        return -1;
    }
    ret = fread(val, sizeof(config_val_t), 1, file);
    fclose(file);

    return ret;
}

/*遍历整个参数区*/
int32_t Ins_Unit_Config_Show_Config_Val(void)
{
    int ret, i;
    FILE *file;
    config_val_t val;
    uint8_t *p_val = (uint8_t*)(&val);

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("Ins_Unit_Config_Read_Val open file error\n");
        return -1;
    }
    ret = fread(&val, sizeof(config_val_t), 1, file);
    if (file < 0) {
        printf("Ins_Unit_Config_Read_Val open file error\n");
        goto _SHOW_VAL_DONE;
    }

    for (i = 0; i < CONFIG_TYPE_NUM; i ++) {
        switch (g_config_val_describe_table[i].val_type) {
            case CONFIG_VALUE_TYPE_UINT8: {
                uint8_t val = (*(uint8_t*)(p_val));
                printf("%d. uint8_t :   %u\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_INT8: {
                int8_t val = (*(int8_t*)(p_val));
                printf("%d. int8_t  :   %d\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_UINT32: {
                uint32_t val = (*(uint32_t*)(p_val));
                printf("%d. uint32_t:   %u\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_INT32: {
                int32_t val = (*(int32_t*)(p_val));
                printf("%d. int32_t :   %d\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_UINT16: {
                uint16_t val = (*(uint16_t*)(p_val));
                printf("%d. uint16_t:   %u\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_INT16: {
                int16_t val = (*(int16_t*)(p_val));
                printf("%d. int16_t :   %d\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_FP32: {
                fp32 val = (*(fp32*)(p_val));
                printf("%d. fp32    :   %.4f\n", i,val);
                break;
            }
            case CONFIG_VALUE_TYPE_FP64: {
                fp64 val = (*(fp64*)(p_val));
                printf("%d. fp64    :   %.4lf\n", i,val);
                break;
            }
            case CONFIG_VALUE_TYPE_UINT64: {
                uint64_t val = (*(uint64_t*)(p_val));
                printf("%d. uint64_t:   %llu\n", i, val);
                break;
            }
            case CONFIG_VALUE_TYPE_INT64: {
                int64_t val = (*(int64_t*)(p_val));
                printf("%d. int64_t :   %lld\n", i, val);
                break;
            }
            default:
                break;
        }
        p_val += g_config_val_describe_table[i].length;
    }

_SHOW_VAL_DONE:
    fclose(file);
    return ret;
}
