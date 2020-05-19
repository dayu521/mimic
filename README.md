### abc

源于兴趣,加深对数据结构与算法的理解.

使用qt库对常用算法与数据结构的状态变化进行可视化.

### 环境

- Linux
  - ArchLinux上
    - 无要求,跟新滚动~~
  - openSUSE TW上
    - 无要求,跟新滚动即可
  - 其他发行版
    - 需要支持c++17,~~gcc是9.2.1版本;于openSUSE TW上测试,需要c++14以上~~
    - qt版本5.13

- Windows
- win8.1下 vs2017集成环境也测试通过,可能ui部分会有不同
- 其他
  - 编辑器为qt自带qtcreator,默认utf-8编码
  - 注意,utf-8编码

### 库

`loki`库来自`Modern C++ Design`,可以到[loki](http://loki-lib.sourceforge.net/)进行下载安装.

由于Linux下几乎没有发行版打包,并且如果你不想自行安装,可以到.pro文件移除掉~~`CONFIG +=loki_is_installed`~~

不过只使用了其中的`Typelist`,就单独加入了几个相关头文件.(虽然代码量很少,不该这样的,之后会考虑`git submodule`)

### 其他

算法目前来自于`算法导论 第三版`与`数据结构与算法分析c++描述 第二版`

可以查看`piece`文件了解一些已知问题和状况.

### 计划

- 红黑树
- 汉诺塔
- 迷宫生成
- 广度优先搜索与深度优先搜索
- 快速排序
