# 编译原理

## lab1

- 没有去掉side effect，不想动了。

## lab2

- 读取字符串时，pos不移动，确定adjust后再移动pos，保证报错总是在字符串开始位置。

## lab3

- if then 没有else的时候不能填NULL，要填NIL_exp。
- string为空默认返回NULL，要改为""

## lab4

- 用了非常愚蠢的办法去解决loop var不能被赋值。

## lab5

- else又改为了NULL，我只能说沙雕测试用例，以前是需要判定为NIL，但是现在的情况是，真的有if then 和 if then else nil。
- 如果没有else的时候返回nil，上面两种情况就混淆了，mdzz。
- 更改了loop var的判定方式。
- 这个lab基本上是靠review别人的代码写的，流下了弱者的眼泪。

## 参考资料

[TerCZ/Compiler-labs-2017](https://github.com/TerCZ/Compiler-labs-2017)

[lncoder/tiger](https://github.com/lhcoder/tiger)