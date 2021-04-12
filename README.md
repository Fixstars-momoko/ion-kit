# ion-kit

A framework to compile user-defined pipeline.

## ビルド方法(更新中)

### Conanのインストール

python3とpipは事前にインストールしてください。

```sh
pip install conan
```

### conanパッケージの準備

下記コマンドを実行することで必要なconanパッケージがローカルに作成される。

```sh
./install_packages.sh
```

### BBライブラリのパッケージ更新

上記コマンドでBBライブラリはすべてエクスポートされるが、BBの実装に変更を加えた場合は更新が必要になる。

更新する場合は更新したBBライブラリに対して下記のコマンドを実行すること。


```sh
conan export [更新したBBライブラリ]
```

### サンプルのビルド(JIT)

`example/image-io`に移動して下記のコマンドを実行すると`main`が生成される。

```sh
mkdir build
cd build
conan install .. --build=missing
cmake ..
make
```

### サンプルのビルド(AOT)

`example/core_aot/compile`に移動して下記のコマンドを実行すると`core.a`と`core.h`が生成される。

```sh
mkdir build
cd build
conan install .. --build=missing
cmake ..
make
```

次に、`example/core_aot/run`に移動して下記のコマンドを実行すると`main`が生成される。

```sh
mkdir build
cd build
conan install .. --build=missing
cmake ..
make
```
