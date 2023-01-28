# `TextEditor`

代码编辑框

运用公开库QScintilla

父类：`QTabWidget`

[Qt编译QScintilla(C++版)过程记录_太阳风暴的博客-CSDN博客_qscintilla 编译](https://blog.csdn.net/qq_43680827/article/details/122611652)

使用QList&lt;QsciScintilla *&gt;管理各个页面

### `void init();`

初始化

### `void initsci();`

创建新的sciScintilla 对象,并设定相关设置（补全）等

### `void newpage(QString name);`

创建新的标签页，名为name，内部调用initsci()，

### `void readfromfile(QString filename);`

读取文件内容并显示在当前页

### `void setcontent(QByteArray,int index=-1)；`

按序号显示在内容，-1为当前页

# `GDbProgress`

### 父类：`QProcess`

管理test.exe 进程

### `QByteArray readoutput();`

读取gdb程序输出

### `QByteArray run(QString );`

运行gdb语句，并返回输出

### `QByteArray listcode();`

采用list 1,100000000输出源码//未实验多文件

### `QByteArray StartRun();`

tbreak 在mai处设置临时断点，运行run

### `QByteArray GetLocalInfo();`

info local获取输出

# StringHandler

解析gdb输出，并进行字符处理

QMap&lt;变量名，QPair<变量值，变量类型&gt;>

### `static QMap<QString,QPair<QString,QString>> GetLocalValue(QString,QMap<QString, QPair<QString, QString> >res =QMap<QString, QPair<QString, QString>>());`

获得本地变量的值，存在map中，

参考

[QScintilla入门指南之边界栏 - Leaos - 博客园 (cnblogs.com)](https://www.cnblogs.com/tuilk/p/16184269.html)

[正则表达式在线测试 | 菜鸟工具 (runoob.com)](https://c.runoob.com/front-end/854/?optionGlobl=global)

[记录执行gdb的过程 - 100个 gdb 小技巧 - 开发文档 - 文江博客 (wenjiangs.com)](https://www.wenjiangs.com/doc/gdb-set-logging)