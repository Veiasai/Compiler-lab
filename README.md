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

## lab6

- init
  - color.c与color.h的声明不匹配。
  - main.c中Tempmap没有声明。

- codegen
  - 实际上，做到ir-tree的时候，脑海中应该已经没有tiger的语法了，完全语言无关了。但是，tree的可能结构，实在太多了，有很多也是不会在tiger语言中出现的，最后就偷懒不做了。

- translate
  - 关于loop，非常令人困惑，loop cond本来写的小于，然后又改成了小于等于，因为from 0 to 7是跑8次。但是这样写汇编是有溢出风险的，只是我也不知道 from minint to maxint要怎么实现，感觉永远退不出来。

- frame
  - 关于fp的识别还有待优化。

## 参考资料

排名不分先后，感谢救命之恩。

[vinx13/tiger-compiler](https://github.com/vinx13/tiger-compiler)

[Wesley-Jzy/SJTU-COMPILER-LAB-2017](https://github.com/Wesley-Jzy/SJTU-COMPILER-LAB-2017)

[TerCZ/Compiler-labs-2017](https://github.com/TerCZ/Compiler-labs-2017)

[lncoder/tiger](https://github.com/lhcoder/tiger)

