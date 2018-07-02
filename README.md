# Meltdown 攻击实验

## 实验原理

### Root cause
> The root cause of the simplicity and strength of Melt-
down are side effects caused by _out-of-order execution_.

CPU 流水线执行遇到分支指令时，下一周期指令读入时，分支的条件判断还未计算出来，为避免使用 Stall 保证而降低性能，乱序执行被提出。乱序执行不按照编程顺序，当前指令需要的资源被满足则执行，否则延后，与编程顺序可能会有不同。加上寄存器重命名机制，操作数可以在计算出来时就被利用，而不用等待上条指令提交结果，这保证了乱序执行时，依赖仍然是顺序的。

内存隔离是今天的操作系统安全性的核心特征之一，程序之间互相内存隔离，用户程序之间不能互相读取内存，内核与用户进程也通过 supervisor bit 定义的该页是否可访问来实现。而 meltdown 破坏了这种隔离性。

### 实现方案

在 CPU 流水线执行时，执行在内存受保护的区域中读取时会进入条件判断，条件判断未完成时会通过分支预测先执行某个分支的指令，此时读取不具有权限的内存部分，将读取的数据乘以4096作为下标访问数组元素。CPU 发现分支预测失败时恢复，但此时访问过的元素会被加载到缓存中。之后再通过加载时间判断哪个元素被缓存过得到的下标计算出之前读取的数据。

每次读取1bit可以使得效率更高。
### 攻击环境
**Kernel** : x86_64 Linux 4.17.3-1-ARCH

**CPU** : Intel Core i5-7200U @ 4x 3.1GHz
#### nokaslr
Linux 内核从 4.12 开始默认使用 kaslr (Kernel Address Space Layout Randomization)，内核参数。有这个参数时，系统内核代码加载时会随机加载到内存中的一个地址，每次启动都会改变，使得攻击者更难发起攻击。

#### nopti

KPTI，也称 PTI，KAISER，是 kernel page-table isolation 的缩写，可以在一定程度上减缓 Meltdown 带来的问题。

**关闭 kaslr 及 pti**:
临时更改可以在开机 grub 的启动界面按 'e' 进入参数编辑界面，在 linux 开头的行尾加入 nokaslr nopti，ctrl+x 启动。

永久更改可以在 /boot/grub/grub.cfg 中更改（不推荐）

或修改 /etc/default/grub 中 GRUB_CMDLINE_LINUX_DEFAULT 选项中加入 nokaslr nopti，再重新生成 grub.cfg

### 攻击过程
**本实验代码由[https://github.com/IAIK/meltdown](https://github.com/IAIK/meltdown)修改得到。**

首先用`git clone`下本仓库代码，使用`make`编译。

在一个终端使用`sudo ./leak`设置需要攻击的信息。
```sh
Secret: You got the flag!! flag is: flag{zF9X]MW?P}
Physical address of secret: 23d769728
Exit with Ctrl+C if you are done reading the secret
```

在另外一个终端中使用`./meltdown 23d769728`
```sh
[+] Physical address       : 0x390fff400
[+] Physical offset        : 0xffff880000000000
[+] Reading virtual address: 0xffff880390fff400

You got the flag!! flag is: flag{zF9X]MW?P}
```
## 参考文献
[https://meltdownattack.com/meltdown.pdf](https://meltdownattack.com/meltdown.pdf)

[https://github.com/IAIK/meltdown](https://github.com/IAIK/meltdown)