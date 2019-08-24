# API 介绍

## 延时

```c
void        dwt_delay(uint32_t tick);
```

```c
void        dwt_udelay(uint32_t us);
```

- us UINT32_MAX 不是有效最大值！

```c
void        dwt_mdelay(uint32_t ms);
```

## 测量

*measure 要结对使用*

```c
void        dwt_measure_begin(void);
```

```c
uint32_t    dwt_measure_end(void);
```

- 返回值为 tick

## 其它

```c
int         dwt_init(uint32_t frquency);
```

```c
float       dwt_set_frequency(uint32_t frquency);
```

重设 dwt 计算用的主频

```c
uint32_t    dwt_tick_to_ns(uint32_t count);
```

将 dwt 的 tick 转为 对应的时间（ns）

```c
uint32_t    dwt_get_count(void);
```

获取 dwt 当前计数值
