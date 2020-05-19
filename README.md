### mimic

源于兴趣,加深对数据结构与算法的理解.

使用qt库对常用算法与数据结构的状态变化进行可视化.

### 计划

- 红黑树
- 汉诺塔
- 迷宫生成
- 广度优先搜索与深度优先搜索
- 快速排序

### 构建

#### 需求

- c++17(尽量使用较新的编译器)

- qt 5.14(使用qtcreator编辑器,且是utf-8(遇到过编码问题,所以还是繁琐地提下))

- 使用了`loki`库(最初来源于`Modern C++ Design`),可以到[loki](http://loki-lib.sourceforge.net/)进行下载安装.

  > 如果你的网络**还可以**,则先不用考虑它,之后可通过git submodule从github拉取

####  Linux(ArchLinux,openSUSE Tumbleweed)
> 其他发行版没有试过,应该问题不大

- 使用过gcc 9

如果安装了loki

~~~bash
  git clone --depth=1 <current_repository_url> src
  cd src
  qmake
  make
~~~
 如果没有安装loki,且网络环境较好
  ~~~bash
  git clone --depth=1 --recurse-submodules --shallow-submodules  <current_repository_url> src
  cd src
  qmake CONFIG+="use_git_submodule_loki"
  make
  ~~~

####  Windows(win8.1,win10)

  - 编译器使用过msvc2017

  - 安装[git](https://git-scm.com/download/win),克隆源码:

      - 如果安装了`loki`: *git clone --depth=1 <current_repository_url> src*
      - 否则:*git clone --depth=1 --recurse-submodules --shallow-submodules  <current_repository_url> src*

  - 进入`src`目录,使用`qtcreator`打开其中的.pro文件,然后构建运行.

    > 如果没有安装loki,则打开.pro中`CONFIG += use_git_submodule_loki`

如果由于网络原因无法得到源码或库,也可以到[github action](https://github.com/dayu521/mimic/actions)中下载,构建好的压缩包(当然,同样的原因,这一步更加困难).

### 其他

算法目前来自于`算法导论 第三版`与`数据结构与算法分析c++描述 第二版
