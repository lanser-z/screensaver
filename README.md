# screensaver
A screen-saver-like program coded with Qt5

==大致需求==
*显示在大屏幕上的类似答题程序
*隔一段时间显示一个数字
*答题者自己把这些数字的和加起来
*界面类似屏保
==实现==
*用Qt5实现
*主窗口showFullScreen，背景固定黑色
*用户自定义要显示的文字、字体、颜色、显示时长
*循环显示用户设置的文字，除非移动鼠标或按下键盘暂停显示
*用F1启动设置界面，用F5恢复循环显示，按ESC退出程序
*用户设置用ini保存，用QTableWidget展现
