# ion-kit

A framework to compile user-defined pipeline.

## ビルド方法(更新中)

### Conanのインストール

```sh
pip install conan
```

### Halideのパッケージ作成

```sh
conan create external/halide-10
```

### ion-kitのパッケージ作成

```sh
conan create ion-core
```

### BBライブラリのパッケージ作成

今のところ`ion-bb-core`と`ion-bb-image-io`のみ。

BB更新時は再度実行すること。

```sh
conan create ion-bb/ion-bb-core
conan create ion-bb/ion-bb-image-io
```

### サンプルのビルド(JIT)

`example/image-io`に移動して下記のコマンドを実行すると`main`が生成される。

```sh
mkdir build
cd build
conan install ..
cmake ..
make
```

### サンプルのビルド(AOT)

`example/core_aot/compile`に移動して下記のコマンドを実行すると`core.a`と`core.h`が生成される。

```sh
mkdir build
cd build
conan install ..
cmake ..
make
```

次に、`example/core_aot/run`に移動して下記のコマンドを実行すると`main`が生成される。

```sh
mkdir build
cd build
conan install ..
cmake ..
make
```
