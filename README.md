# ARMv7-M_DWT

ToDo: 长时长测量支持

Note: ARMv7m Architecture Reference Manual (Chapter ARMv7-M Debug)

## 简介

本软件包基于 ARMv7-M Debug 架构中 DWT 计数器而提供了高精度延时 和 计时功能 （精度：1 / processor-clock)，可用于benchmark等

## 使用说明

### 依赖

- ARMv7-M 架构 （cortex-m3, cortex-m4, cortex-m7）

### 获取软件包

使用 ARMv7-M_DWT 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```shell
    RT-Thread online packages
        miscellaneous packages --->
            [*] armv7m_dwt: High precision timing and delay --->
                Version (latest) --->
```

**Version**：软件包版本选择

### 使用软件包

首先需要进行初始化, 初始化接口如下：

```c
int dwt_init(uint32_t frquency);
```

_e.g. 对于 STM32 系列 HAL 库 ``` dwt_init(HAL_RCC_GetSysClockFreq()) ```_

初始化完成后就可以使用了，相关 API 请参考 [api.md](./api.md)

## 注意事项

* 不能并发使用, 除非你知道在干什么

* The function can't work when the processor is halted in Debug state.

## 联系人信息

维护人:

- [sogwms](https://github.com/sogwms)
