## 1. Run the program 
(assuming that you are in the dictionary src/)
```
    1) make
    2) ./DBProject
```
参考: 

makefile的语法及写法  : http://blog.csdn.net/zhoubl668/article/details/3952967
## 2. Debug the program
(">" 表示在gdb环境下)
```
    > gdb                (进入gdb调试环境)
    > make               (执行make命令)

    > file DBProject     (载入可执行文件)
    > r                  (运行程序)
    > quit               (退出gdb)

    > list 10            (显示第10行附近的内容)
    > list main          (显示main函数附近的行)

    > search main        (使用search命令查找字符串，并显示所有符合条件的代码，这里可以使用正则表达式)

    > break main         (在main函数开始运行时，停止)
    > break 26           (在line 26行设置断点)
    > break debug.c:24   (在文件debug.c的24行设置断点)
    > clear main         (删除main函数上的断点)
    > clear 26           (删除26行的断点)
    > clear debug.c:main (删除debug.c文件中main函数上的断点)
    > info breakpoints   (查看断点信息)

    > step               (单步步入)
    > next               (单步步过)
    > continue           (不管断点继续运行)

    > watch i            (监视变量i，当i的值变化之后，gdb将停止程序运行)
    > catch fork         (如果程序调用fork函数，gdb将停止程序运行)

    > print p            (p 为变量名: 结构体类型(2 = {x = 0, y = 0}); 数组(3 = {1, 2}); int类型(4 = 10))

```
参考: 

GDB命令使用总结（二） : http://blog.chinaunix.net/uid-20788636-id-1841301.html

使用gdb调试程序       : http://www.cnblogs.com/xuqiang/archive/2011/05/02/2034583.html
