#じゃんけん分類器

##概要
カメラ画像中に映り込んだ手先領域の形状から、じゃんけんでどの手(グー・チョキ・パー)を出しているか判定するシステム。

##使用ライブラリ
- OpenCV-2.3.1

  - ヘッダファイル
    - opencv.hpp

  - ライブラリファイル
    - opencv_core231d.lib
    - opencv_imgproc231d.lib
    - opencv_highgui231d.lib
    - opencv_legacy231d.lib
    - opencv_video231d.lib

##動作環境
- Visual Studio 2012 on Windows 7
で動作することを確認済み。

##判定原理
1. カメラ画像中から肌の色に近い部分を抽出。
1. モルフォロジー処理(小さな領域の除去、抽出領域中の欠損の補完)。
1. 抽出領域を決定。
1. 抽出領域の輪郭を取得。
1. 抽出領域の面積と周囲長から円形度を算出。
1. 円形度をもとにじゃんけんの手を判定。

##工夫
- モルフォロジー処理を行うことで、環境に大きく左右されず円形度を計算することができる。
- 円形度を記録することができ、あとで分析に使用することもできる。

##実験結果
<img src="http://karakuri-do.sakura.ne.jp/tokky/recruit2016winter/1.png" width="500">
<img src="http://karakuri-do.sakura.ne.jp/tokky/recruit2016winter/2.png" width="500">
<img src="http://karakuri-do.sakura.ne.jp/tokky/recruit2016winter/3.png" width="500">
<img src="http://karakuri-do.sakura.ne.jp/tokky/recruit2016winter/graph.png" width="500">
