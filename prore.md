---
markdown:
  image_dir: /assets
  path: README.md
  ignore_from_front_matter: true
  absolute_image_path: false
export_on_save:
    markdown: true
---
# Required Packages
* GCC 5.4.x or later
* Cmake 3.5 or higher
* [OpenCV 3.0.0 or higher](https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html)

# Program Construct(undone)
```viz{filename="ad.png"}
digraph G{
    node[shape="record"];
    subThread1[label="drok"];
    elec[label="lower system"]
    subgraph cluster_communiction{
       lable="mainThread"
       bgcolor="beige"
       info1[label="LineInfo"]
       main[label="main"]
       serial
    }
    subThread1->info1[label="info" dir=both];
    info1->main[label="info" dir=both];
    main->subThread1[label="detach" style=dotted];
    main->serial;
    serial->elec;
}
```
#程序简介
此程序用于参加2018robocon比赛的我校手动机器人中，是机器人的视觉程序，作为电控的辅助系统。
#功能分类
通信:
main serial Info
对接:
lineTest linesOption lineInfo
看线:
rtlFinder rtlInfo