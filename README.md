赛区：西北赛区

队名：师妃暄

题目链接 http://codecraft.huawei.com/home/detail
![Rank](https://github.com/SequinYF/Future_Route/blob/master/rank.png)


| Author | Sequin小红九|
| ---------- | -----------|
| Email   | SequinYF@xiyoulinux.org   |
| Csdn    | http://blog.csdn.net/sequin_yf |

## 背景
大视频解决方案中，视频业务体验非常关键，视频内容如何有效传送到最终消费者是决定视频体验好坏的核心环节。
## 概述
在给定结构的G省电信网络中，为了视频内容快速低成本的传送到每个住户小区，需要在这个给定网络结构中选择一些网络节点附近放置视频内容存储服务器。需要解决的问题是：在满足所有的住户小区视频播放需求的基本前提下，如何选择视频内容存储服务器放置位置，使得成本最小。

## 思路
对于NP完全问题，是根本不能去精确求解的，所以我们只能去求近似解，我们选择的是遗传算法，先假设源点服务器，再建一个汇点连接到所有消费节点，用网络流去求解。

### 其它

主要代码在SDK-gcc下的route.cpp文件，其他为官方提供的。

SDK-gcc目录下为c/c++版本的SDK，具体使用方法参看该路径下的readme。
case0目录下为一套测试例，其中topo.csv为图的信息文件，demand.csv为路径信息文件。
