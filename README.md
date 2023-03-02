# Course-Exercise-of-Software-Design

该程序为windows环境下的可视化的C++调试器，同时可以为代码添加图片和音频注释

## 使用说明

### 前提条件

**确保环境变量PATH中包含有`gcc`,`g++`,`gdb`**，可在cmd输入`gcc -v`等命令加以验证

### 如何使用

点击Course-Exercise-of-Software-Design.exe即可开始运行程序

## 源码编译说明

### 前提条件

1. 系统已安装Qt5.14.2版本
2. 编译安装QScintilla库到Qt

### 安装Qt5.14.2

1. 打开Qt下载官网  [Index of /archive/qt/5.14/5.14.2](https://download.qt.io/archive/qt/5.14/5.14.2/)
2. 点击系统对应的安装包进行下载，例如windows选择的是  [qt-opensource-windows-x86-5.14.2.exe](https://download.qt.io/archive/qt/5.14/5.14.2/qt-opensource-windows-x86-5.14.2.exe)
3. 打开下载的安装程序，按照程序的引导完成Qt的安装

### 编译安装QScintilla

1. 打开QScintilla的官方下载地址   [Riverbank Computing | Download](https://www.riverbankcomputing.com/software/qscintilla/download)
2. 选择压缩包下载解压
3. 使用 QtCreater打开 **src/qscintilla.pro** 工程文件，点击 编译或运行按钮都可编译
4. 把编译的动态库放在Qt的lib 、bin路径下，如qscintilla2_qt5d.dll，libqscintilla2_qt5d.a
5. 把./src/Qsci 放在Qt的include header路径下
6. 把./src/qscintilla_\*.qm 下的多语言文件放在 translation文件夹下
7. 把./src/features该库的特性文件prf放在Qt的mkspecs/features 路径下

参考教程：[Qt编译QScintilla(C++版)过程记录_qscintilla编译_太阳风暴的博客-CSDN博客](https://blog.csdn.net/qq_43680827/article/details/122611652)

### 编译Course-Exercise-of-Software-Design

1. clone 仓库 `git clone https://github.com/guofuwei/Course-Exercise-of-Software-Design.git`
2. 用QtCreator打开**src/Course-Exercise-of-Software-Design**文件
3. 点击编译或者运行即可

## 图片注释和音频注释的规范

图片存放的位置:`$ROOT/comment/pic`

音频存放的位置:`$ROOT/comment/audio`

代码中图片注释:`[COMMENT-PIC-$FILENAME-$DATETIME]`

代码中音频注释:`[COMMENT-AUDIO-$FILENAME-$DATETIME]`

$FILENAME:当前文件名

$DATETIME:格式为yyyy_MM_dd_hh_mm_ss，其中yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒

$ROOT:程序运行根目录

**注意：文件存放路径必须存在，否则会报错**
