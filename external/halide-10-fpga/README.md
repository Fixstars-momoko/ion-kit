# FPGAバックエンドを有効にしたHalideパッケージ化方法

今のところHalideのビルドはレシピ化されていません。

下記の手順でパッケージ化してください。

## 1. Halideのビルド

`CMAKE_INSTALL_PREFIX`に適当なディレクトリを指定してHalideをインストールしてください。

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<install_dir> [その他オプション] ..
cmake --build .. --target install
```

## 2. conanパッケージ化

同梱の`conanfile.py`をインストールディレクトリにコピーして下記コマンドを実行してください。

```sh
conan create .
```
