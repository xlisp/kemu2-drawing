# 用AI来辅助画画：画武术的图，还有画学车的图

* 提示词画局部小的可以，画大的就生成效果很差: 你不知道怎么画的局部可以提示词生成一下图，然后参考模仿画局部
* 整体画线，区域分开就像素描画人一样: 用网格，矩形和圆形来表示物体，比如画抓球的手可以先画球，然后画贴住球的手的点，再画手出来, 手就立体了 (球体定点求导)
* 如果GPT无法生成你要的局部素材，就谷歌搜索素材: 模仿多图层临摹在procreate上
* 买一个小车的模型回来：帮助自己画出示意图，粉笔在地上画出来车库的线 => 武术则可以买两个玩具人来比划，画出来人物招式对打 : 模型或者是道具，就像拍电影和开发游戏一样，绿色背景，脸的点位捕捉同步替换外星人脸 => 类似于代码开发的跑起来再抄是最快的
* 用8x变焦iPhone来切换解说不同的镜头视角是不错的: 方便学习看不同点位来停车, 记住点位来帮助考试, 理解视觉差，距离，方向感等空间概念
* 从开车模拟器游戏中中参考画出来图：这样各种不同的镜头也可以使用了，车的模型可以GTA5里面去看不同的角度
* 用Shapr3D来画3D建模图：然后切换不同场景查看！其实就是不同的mirror的适应视觉差的问题, 先画几个轮子然后定点来画车底板再画车盖，最后画目标标记线即可。

## 一些提示词树

```
- 重新画一下此图，要求简洁图示意，和文字说明
	 - gpt客户端可以做到多模态，cl不行。

	 - 生成一个科目二侧方停车的示意图，
		 - 要求简洁的示意图，平面显示小车过程123…步骤，还有方向文字说明

- 画一个示意图： 当车身进入库剩下最后一根虚线，方向盘回半圈加四分之一
	 - 是侧方停车的平面示意图

- 画一个示意图： 一个S曲线，然后一个车中心对准s曲线的入口中心

```

