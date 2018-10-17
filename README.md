# 编译原理

## lab1

- 没有去掉side effect，不想动了。

## lab2

- 读取字符串时，pos不移动，确定adjust后再移动pos，保证报错总是在字符串开始位置。

## lab3

- if then 没有else的时候不能填NULL，要填NIL_exp。
- string为空默认返回NULL，要改为""