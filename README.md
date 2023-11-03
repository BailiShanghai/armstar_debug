# 工程介绍

用于 BK7236 验证，仿真，包括多核，spinlock，s/ns 跳转等等。

# 环境安装

当前仅支持 keil 编译，需要安装 keil 环境，请参考王总宝典，安装时注意下边几个问题：

1. 下载好 MDK5.36，点击 MDK536.exe 安装，一直点 next 到完成
2. 点击  ARM.CMSIS.5.8.0.pack 与 ArmChina.START.2.0.0.pack 安装 pack
3. 以管理员权限打开 keil, 按 file -> License Management 找到 CID，然后使用 keygen.exe 生成 key
4. 点 flash -> Config flash tools ... -> C/C++ 头文件 path 的前两行，替换成 步骤 2 中 pack 头文件目录

# 编译

将 S 设置成 active 工程，编译 S；将 NS 设置成 active 工程，编译 NS。先编译 S 再编译 NS。

# 合并/加 CRC

使用 merge/merge.bat 可以合并 bk7236_spe.bin/bk7236_nspe.bin，同时加 CRC。三核时相应做一些调整。

# 配置

通过 verification_config.h 来配置。

# 日志

S/NS 均使用 UART0 打印，因此涉及 S/NS 切换时，要同时切换 UART0 安全属性。

# 链接 

1. 链接文件是 bk7236.sc 文件，在 flash -> Config flash tools -> linker 中配置
2. 链接文件可见：
   bk7236_spe.bin: flash 0 地址
   bk7236_nspe.bin: flash 0x20000 //128KB
