#ifndef __UTIL_CONFIG_H_
#define __UTIL_CONFIG_H_

#include<stdint.h>
#define __packed    __attribute__((packed))
#define fp32        float
#define fp64        double

/*参数数据类型枚举*/
typedef enum {
    CONFIG_VALUE_TYPE_INT64 = 0,
    CONFIG_VALUE_TYPE_UINT64,
    CONFIG_VALUE_TYPE_INT32,
    CONFIG_VALUE_TYPE_UINT32,
    CONFIG_VALUE_TYPE_FP32,
    CONFIG_VALUE_TYPE_FP64,
    CONFIG_VALUE_TYPE_INT16,
    CONFIG_VALUE_TYPE_UINT16,
    CONFIG_VALUE_TYPE_INT8,
    CONFIG_VALUE_TYPE_UINT8,
    CONFIG_VALUE_TYPE_NUM,
} CONFIG_VALUE_TYPE_e;

/*参数枚举*/
typedef enum {
    CONFIG_TYPE_VERSION = 0,
    CONFIG_TYPE_TEST_UINT8,
    CONFIG_TYPE_TEST_INT8,
    CONFIG_TYPE_TEST_UINT32,
    CONFIG_TYPE_TEST_INT32,
    CONFIG_TYPE_TEST_UINT16,
    CONFIG_TYPE_TEST_INT16,
    CONFIG_TYPE_TEST_FP32,
    CONFIG_TYPE_TEST_FP64,
    CONFIG_TYPE_TEST_UINT64,
    CONFIG_TYPE_TEST_INT64,
    // CONFIG_TYPE_UPDATE_TEST,
    CONFIG_TYPE_NUM,
} CONFIG_TYPE_e;

/*校验方式*/
typedef enum {
    CONFIG_VAL_CHECK_TYPE_NULL = 0,
    CONFIG_VAL_CHECK_TYPE_RANGE,
    CONFIG_VAL_CHECK_TYPE_EQUAL,
    CONFIG_VAL_CHECK_TYPE_UNEQUAL,
    CONFIG_VAL_CHECK_TYPE_CUSTOM_VERSION,
} CONFIG_VAL_CHECK_TYPE_e;

/*参数区数值定义*/
typedef struct __packed {
    uint32_t    version;
    uint8_t     test_uint8_t;
    int8_t      test_int8_t;
    uint32_t    test_uint32_t;
    int32_t     test_int32_t;
    uint16_t    test_uint16_t;
    int16_t     test_int16_t;
    fp32        test_fp32_t;
    fp64        test_fp64_t;
    uint64_t    test_uint64_t;
    int64_t     test_int64_t;
    // fp32        update_test;
} config_val_t;

/*参数数据描述*/
typedef struct {
    CONFIG_VALUE_TYPE_e     val_type;
    uint16_t                length;
    uint32_t                offset;
    CONFIG_VAL_CHECK_TYPE_e check_type;
    void                    *check_val;
    int32_t                 check_len;
    int32_t                 (*check_fun)(void*, void*);
} CONFIG_UNIT_VALUE_DESCRIBE_t;

/*参数区文件是否存在*/
int32_t Unit_Config_Is_Exist(void);
/*删除参数区文件*/
int32_t Unit_Config_Clear(void);
/*用默认值创建参数区文件*/
int32_t Unit_Config_Creat_By_Default_Val(void);
/*读取某个参数的数值*/
int32_t Unit_Config_Read_Val(CONFIG_TYPE_e type, void *val);
/*写入数值到某个参数*/
int32_t Unit_Config_Write_Val(CONFIG_TYPE_e type, void *val);
/*读取所有参数区数值*/
int32_t Unit_Config_Read_Config_File(config_val_t *val);
/*写入整个参数区*/
int32_t Unit_Config_Write_Config_File(config_val_t *val);
/*遍历整个参数区*/
int32_t Unit_Config_Show_Config_Val(void);
/*校准参数区数值*/
int32_t Unit_Config_Correct_Val(void);

#endif
