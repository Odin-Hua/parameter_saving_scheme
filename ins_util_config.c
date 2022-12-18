#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ins_util_config.h"

#define CONFIG_PATH     "config"            /*参数区文件*/
typedef struct {
    int32_t     exist_flag;
} g_config_ctrl_t;                          /*参数区全局操作变量定义*/
/*范围检测*/
static int32_t RangeCheck_Fun(CONFIG_TYPE_e type, void *val);

/*参数区校验值*/
uint8_t     test_uint8_t_check_val[10] = {10, 20, 30, 40, 50, 60, 90, 120, 130, 150};
int8_t      test_int8_t_check_val[4]   = {-50, -30, -20, 20};
uint32_t    test_uint32_t_check_val[2] = {0xFFFFFFF, 0x9FFFFFFF};
int32_t     test_int32_t_check_val[4]  = {-0x1FFFFFFF, 0, 1, 10};
uint16_t    test_uint16_t_check_val[2] = {0xFFF, 0x9FFF};
int16_t     test_int16_t_check_val[4]  = {-0x1FFF, 0, 1, 10};
fp32        test_fp32_check_val[4]     = {(fp32)0, (fp32)12, (fp32)50, (fp32)100};
fp64        test_fp64_check_val[4]     = {(fp64)-11111111, (fp64)0, (fp64)1, (fp64)10};
uint64_t    test_uint64_t_check_val[2] = {(uint64_t)0xFFFFFFFFFFFFFFFULL, (uint64_t)0x9FFFFFFFFFFFFFFFULL};
int64_t     test_int64_t_check_val[4]  = {(int64_t)-0x1FFFFFFFFFFFFFFFLL, (int64_t)0, (int64_t)1, (int64_t)10};

/*参数区数值描述: CONFIG_VALUE_TYPE_e, length, offset, CONFIG_VAL_CHECK_TYPE_e, check_val, check_len, check_fun*/
CONFIG_UNIT_VALUE_DESCRIBE_t g_config_val_describe_table[] = {
    [CONFIG_TYPE_VERSION]      = {CONFIG_VALUE_TYPE_UINT32,    4,   0,  CONFIG_VAL_CHECK_TYPE_NULL,     NULL,                       0,   NULL},
    [CONFIG_TYPE_TEST_UINT8]   = {CONFIG_VALUE_TYPE_UINT8,     1,   4,  CONFIG_VAL_CHECK_TYPE_RANGE,    test_uint8_t_check_val,     5,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_INT8]    = {CONFIG_VALUE_TYPE_INT8,      1,   5,  CONFIG_VAL_CHECK_TYPE_RANGE,    test_int8_t_check_val,      2,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_UINT32]  = {CONFIG_VALUE_TYPE_UINT32,    4,   6,  CONFIG_VAL_CHECK_TYPE_RANGE,    test_uint32_t_check_val,    1,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_INT32]   = {CONFIG_VALUE_TYPE_INT32,     4,   10, CONFIG_VAL_CHECK_TYPE_RANGE,    test_int32_t_check_val,     2,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_UINT16]  = {CONFIG_VALUE_TYPE_UINT16,    2,   14, CONFIG_VAL_CHECK_TYPE_RANGE,    test_uint16_t_check_val,    1,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_INT16]   = {CONFIG_VALUE_TYPE_INT16,     2,   16, CONFIG_VAL_CHECK_TYPE_RANGE,    test_int16_t_check_val,     2,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_FP32]    = {CONFIG_VALUE_TYPE_FP32,      4,   18, CONFIG_VAL_CHECK_TYPE_RANGE,    test_fp32_check_val,        2,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_FP64]    = {CONFIG_VALUE_TYPE_FP64,      8,   22, CONFIG_VAL_CHECK_TYPE_RANGE,    test_fp64_check_val,        2,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_UINT64]  = {CONFIG_VALUE_TYPE_UINT64,    8,   30, CONFIG_VAL_CHECK_TYPE_RANGE,    test_uint64_t_check_val,    1,   RangeCheck_Fun},
    [CONFIG_TYPE_TEST_INT64]   = {CONFIG_VALUE_TYPE_INT64,     8,   38, CONFIG_VAL_CHECK_TYPE_RANGE,    test_int64_t_check_val,     2,   RangeCheck_Fun},
};
/*参数区默认值*/
config_val_t g_config_default_val = {
    .version        = (uint32_t)1,
    .test_uint8_t   = (uint8_t)143,                         /*0x8f*/
    .test_int8_t    = (int8_t)-15,                          /*-0xf*/
    .test_uint32_t  = (uint32_t)2415919103,                 /*0x8FFFFFFF*/
    .test_int32_t   = (int32_t)-268435455,                  /*-0xFFFFFFF*/
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
        printf("%s error\n", __func__);
    } else {
        g_config_ctrl.exist_flag = 0;
        printf("%s success\n", __func__);
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
        printf("%s create file error\n", __func__);
        return -1;
    }
    ret = fwrite(&g_config_default_val, sizeof(config_val_t), 1, file);
    if (ret < 0) {
        printf("%s write file error\n", __func__);
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
        printf("%s error input param\n", __func__);
        return -1;
    }

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("%s open file error\n", __func__);
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
        printf("%s error input param\n", __func__);
        return -1;
    }

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("%s  open file error\n", __func__);
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
        printf("%s error input param\n", __func__);
        return -1;
    }
    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("%s open file error\n", __func__);
        return -1;
    }
    ret = fread(val, sizeof(config_val_t), 1, file);
    fclose(file);

    return ret;
}

/*写入整个参数区*/
int32_t Ins_Unit_Config_Write_Config_File(config_val_t *val)
{
    int ret;
    FILE *file;

    file = fopen(CONFIG_PATH, "wb+");
    if (file < 0) {
        printf("%s create file error\n", __func__);
        return -1;
    }
    ret = fwrite(val, sizeof(config_val_t), 1, file);
    if (ret < 0) {
        printf("%s write file error\n", __func__);
    }
    fflush(file);
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
        printf("%s open file error\n", __func__);
        return -1;
    }
    ret = fread(&val, sizeof(config_val_t), 1, file);
    if (file < 0) {
        printf("%s open file error\n", __func__);
        goto _SHOW_VAL_DONE;
    }

    printf("========================================================================\n");
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
    printf("========================================================================\n");

_SHOW_VAL_DONE:
    fclose(file);
    return ret;
}

/*校准参数区数值*/
int32_t Ins_Unit_Config_Correct_Val(void)
{
    int ret, i;
    FILE *file;
    config_val_t val;
    uint8_t *p_val = (uint8_t*)&val;
    int32_t need_to_correct = 0;

    file = fopen(CONFIG_PATH, "rb+");
    if (file < 0) {
        printf("%s open file error\n", __func__);
        return -1;
    }
    ret = fread(&val, sizeof(config_val_t), 1, file);
    if (file < 0) {
        printf("%s open file error\n", __func__);
        goto _CORRECT_VAL_DONE;
    }

    for (i = 0; i < CONFIG_TYPE_NUM; i ++) {
        switch (g_config_val_describe_table[i].check_type) {
            case CONFIG_VAL_CHECK_TYPE_NULL:
            case CONFIG_VAL_CHECK_TYPE_EQUAL:
            case CONFIG_VAL_CHECK_TYPE_UNEQUAL:
            case CONFIG_VAL_CHECK_TYPE_CUSTOM:
                break;
            case CONFIG_VAL_CHECK_TYPE_RANGE:
                need_to_correct = g_config_val_describe_table[i].check_fun((CONFIG_TYPE_e)i, p_val);
                break;
            default:
                break;
        }
        p_val += g_config_val_describe_table[i].length;
    }

    if (need_to_correct == 1) {
        fseek(file, 0, SEEK_SET);
        ret = fwrite(&val, sizeof(config_val_t), 1, file);
        if (ret < 0) {
            printf("%s write file error\n", __func__);
            goto _CORRECT_VAL_DONE;
        }
        printf("%s correct file success\n", __func__);
    } else if (need_to_correct == 0) {
        printf("%s not need to correct file\n", __func__);
    }

_CORRECT_VAL_DONE:
    fflush(file);
    fclose(file);
    return ret;
}

/*范围检测*/
static int32_t RangeCheck_Fun(CONFIG_TYPE_e type, void *val)
{
    if (type > CONFIG_TYPE_NUM || val == NULL) {
        printf("%s err input\n", __func__);
        return -1;
    }

    CONFIG_VALUE_TYPE_e val_type    = g_config_val_describe_table[type].val_type;
    int32_t             check_len   = g_config_val_describe_table[type].check_len;
    int32_t i, ret, not_need_to_correct = 0;

    for (i = 0; i < check_len; i ++) {
        switch (val_type) {
            case CONFIG_VALUE_TYPE_UINT8: {
                uint8_t *p_check_val = (uint8_t*)g_config_val_describe_table[type].check_val;
                uint8_t _val = (*(uint8_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_INT8: {
                int8_t *p_check_val = (int8_t*)g_config_val_describe_table[type].check_val;
                int8_t _val = (*(int8_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_UINT32: {
                uint32_t *p_check_val = (uint32_t*)g_config_val_describe_table[type].check_val;
                uint32_t _val = (*(uint32_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_INT32: {
                int32_t *p_check_val = (int32_t*)g_config_val_describe_table[type].check_val;
                int32_t _val = (*(int32_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_UINT16: {
                uint16_t *p_check_val = (uint16_t*)g_config_val_describe_table[type].check_val;
                uint16_t _val = (*(uint16_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_INT16: {
                int16_t *p_check_val = (int16_t*)g_config_val_describe_table[type].check_val;
                int16_t _val = (*(int16_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_FP32: {
                fp32 *p_check_val = (fp32*)g_config_val_describe_table[type].check_val;
                fp32 _val = (*(fp32*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_FP64: {
                fp64 *p_check_val = (fp64*)g_config_val_describe_table[type].check_val;
                fp64 _val = (*(fp64*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_UINT64: {
                uint64_t *p_check_val = (uint64_t*)g_config_val_describe_table[type].check_val;
                uint64_t _val = (*(uint64_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            case CONFIG_VALUE_TYPE_INT64: {
                int64_t *p_check_val = (int64_t*)g_config_val_describe_table[type].check_val;
                int64_t _val = (*(int64_t*)(val));
                if (p_check_val[2*i] < _val && _val < p_check_val[2*i+1]) {
                    not_need_to_correct = 1;
                    goto _CORRECT_OPERA;
                }
                break;
            }
            default:
                break;
        }
    }

_CORRECT_OPERA:
    if (not_need_to_correct == 1) {
        ret = 0;
    } else if (not_need_to_correct == 0) {
        uint8_t *p_default_val = (uint8_t*)(&g_config_default_val);
        p_default_val += g_config_val_describe_table[type].offset;
        memcpy(val, p_default_val, g_config_val_describe_table[type].length);
        ret = 1;
    }

    return ret;
}