# OpenHarmony 适配说明

该目录提供一个最小的 OpenHarmony 原生集成示例，便于在 PR 评审时复现 `cppjieba` 的构建、打包与运行方式。

## 变更说明（按模块拆分）

- **构建/配置**：`CMakeLists.txt` 新增 `cppjieba_openharmony_demo` 示例目标，并通过 `CPPJIEBA_OPENHARMONY_DICT_DIR` 注入词典目录。
- **示例代码**：`openharmony_demo.cpp` 展示了 OpenHarmony 原生侧最小集成方式，显式传入词典、HMM、用户词典、IDF 和停用词路径。
- **测试验证**：示例已接入现有 `ctest`，主机构建时会自动编译并运行，保证示例始终可用。

## OpenHarmony 构建脚本/配置

> `cppjieba` 本身是头文件库。OpenHarmony 侧只需要把 `include/`、`dict/` 和 `deps/limonp/include/` 纳入原生工程，并为示例可执行文件或业务 so 提供词典目录。

主机侧验证命令：

```sh
git submodule update --init --recursive
cmake -S . -B build
cmake --build build -j4
ctest --test-dir build --output-on-failure
./build/examples/openharmony/cppjieba_openharmony_demo
```

OpenHarmony 交叉编译示例（以 arm64 为例）：

```sh
git submodule update --init --recursive
cmake -S . -B build-ohos \
  -DCMAKE_TOOLCHAIN_FILE=$OHOS_NDK_HOME/native/build/cmake/ohos.toolchain.cmake \
  -DOHOS_ARCH=arm64-v8a \
  -DOHOS_STL=c++_shared \
  -DCPPJIEBA_OPENHARMONY_DICT_DIR=/data/storage/el2/base/files/cppjieba/dict
cmake --build build-ohos -j4 --target cppjieba_openharmony_demo
```

## 最小示例

`openharmony_demo.cpp` 会对如下文本执行精确模式分词：

```txt
OpenHarmony 原生应用可以直接集成 cppjieba
```

运行结果会输出到标准输出，适合作为接入阶段的冒烟验证。

## 与现有平台兼容性说明

- 未修改 `cppjieba` 核心分词逻辑，仅新增示例和构建入口；
- 现有 Linux、macOS、Windows 构建方式保持不变；
- OpenHarmony 原生侧建议将 `dict/` 复制到应用可访问的文件系统目录后，再把实际路径传给 `cppjieba::Jieba`，因为词典加载依赖标准文件路径。

## 可复现测试步骤

1. 执行上面的主机侧验证命令，确认单元测试和 `cppjieba_openharmony_demo` 都通过；
2. 在 OpenHarmony 工程中复用相同的 `CPPJIEBA_OPENHARMONY_DICT_DIR` 配置方式；
3. 部署词典文件后运行示例，确认能够输出分词结果。
