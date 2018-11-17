# NOLO CV1 Driver for ALVR

NOLO CV1で取得できる6自由度のトラッキング情報をALVRに流し込めるようにします．
ALVRのFreePIE連携機能をMemory Mapped IOを直接叩いて利用しています(FreePIEは必要ありません)．

## インストール

1. [NOLO Assistant](http://download.nolovr.com/download/noloassistant.html) と [ALVR](https://github.com/polygraphene/ALVR) をインストールします
1. このリポジトリをクローンしてビルドするか，[ここ](https://github.com/tokoro10g/NOLOALVRDriver/releases)からコンパイル済みのファイルをダウンロードします

## 使い方

1. NOLO CV1のベースステーションとコントローラの電源を入れます．
1. ヘッドセットのマーカーをUSBケーブルで**PCと**接続します．
   (RiftCat+NOLOのようなスタンドアロン環境には現状対応していません．)
1. NOLO Assistantを起動します．
1. NOLOALVRDriver.exeを起動します．
1. これでトラッキングできてるはず…！

## Note

* [VirtualHere USB](https://www.virtualhere.com/home)のようなものを使えば，スタンドアロン環境でも使えるかもしれません．
* 開発にあたって，こちらの記事を大いに参考にさせていただきました．  
  [Oculus GoとALVRとNOLO-CV1でSteam VR Gameを遊ぶ](https://qiita.com/ahinore@github/items/a4a8e675eb89b41921a0)
* `lib` フォルダ内のDLLファイルは，[NOLO Unity SDK](https://github.com/NOLOVR/NOLO-Unity-SDK)のものです．