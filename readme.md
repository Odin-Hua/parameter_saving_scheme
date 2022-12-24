##### 设计背景

* 嵌入式项目中，为了保证系统的正常运转，通常需要保存一部分数据至非易失存储设备如flash中。本文提供了一种通用的方案用于快速在项目中集成参数保存功能，该方案有以下几点特征：
  
  * 接口简便，方便快速集成以及使用；
  
  * 参数类型包含常见的数据类型，同时预留了自定义数据类型的接口；
  
  * 拥有校验机制，提供了范围校验/相等校验的接口，同时预留了自定义校验的接口；
  
  * 参数区拥有版本号概念，同时预留了参数区版本号变更的操作接口；

##### 相关举例

* 为了添加参数时尽可能的方便，统一用一个结构体对参数进行描述
  
  ```c
  typedef struct {
      CONFIG_VALUE_TYPE_e     val_type;
      uint16_t                length;
      uint32_t                offset;
      CONFIG_VAL_CHECK_TYPE_e check_type;
      void                    *check_val;
      int32_t                 check_len;
      int32_t                 (*check_fun)(void*, void*);
  } CONFIG_UNIT_VALUE_DESCRIBE_t;
  ```
  
  * **CONFIG_VALUE_TYPE_e**枚举表示该参数的数据类型，比如uint8_t为**CONFIG_VALUE_TYPE_UINT8**；
  
  * **length**表示该参数的数据长度，以字节为单位；
  
  * **offset**表示该参数相对于整个参数区的偏移量，以字节为单位；
  
  * **CONFIG_VAL_CHECK_TYPE_e**枚举表示该参数的校验方式，比如范围校验为**CONFIG_VAL_CHECK_TYPE_RANGE**；
  
  * **check_val**和**check_len**分别表示校验使用到的数据以及数据长度；
  
  * **check_fun**则是具体的校验方法；

* 当需要添加一个参数时，只要添加上对该参数的描述即可，以加入版本号为例子：
  
  * 先在**CONFIG_TYPE_e**枚举中加入参数枚举
    
    ```c
    typedef enum {
        CONFIG_TYPE_VERSION = 0,
        CONFIG_TYPE_NUM,
    } CONFIG_TYPE_e;
    ```
  
  * 在**参数区默认值表**中加入版本号默认值，该表为跟随固件的全局变量；
    
    ```c
    config_val_t g_config_default_val = {
        .version        = (uint32_t)1,
    };
    ```
  
  * 接着在**参数描述表**中添加对版本号参数的描述；从下表可知该参数的数据类型为uint32_t，参数数据长度为4字节，相较于参数区文件的偏移量为0，参数校验方式为自定义校验，不需要使用校验数据，校验函数为Version_CustomCheck_Fun；
    
    ```c
    CONFIG_UNIT_VALUE_DESCRIBE_t g_config_val_describe_table[] = {
        [CONFIG_TYPE_VERSION] = {CONFIG_VALUE_TYPE_UINT32, 4, 0, CONFIG_VAL_CHECK_TYPE_CUSTOM_VERSION, NULL, 0, Version_CustomCheck_Fun}, 
    };
    ```
  
  * 使用如下的方法，对该参数进行读写
    
    ```c
    uint32_t val;
    Unit_Config_Read_Val(CONFIG_TYPE_VERSION, &val);
    val ++;
    Unit_Config_Write_Val(CONFIG_TYPE_VERSION, &val);
    ```
  
  ##### 整体流程
  
  ![](https://gitee.com/ACE_ZJH/parameter_saving_scheme/raw/master/img/process.png)
