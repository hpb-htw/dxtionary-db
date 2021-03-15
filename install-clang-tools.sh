#!/usr/bin/env sh

# Remove all existing alternatives
sudo update-alternatives --remove-all llvm
sudo update-alternatives --remove-all clang
sudo update-alternatives --remove-all clang++

# exit on first error
set -e

# To Install llvm/clang 10
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-add-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main"
sudo apt update
sudo apt install -y clang-11 llvm-11 lld-11 lldb-11
sudo apt install -y clang-format-11 clang-tidy-11 clang-tools-11


# llvm-11 and clang-11
sudo update-alternatives \
  --install /usr/lib/llvm              llvm             /usr/lib/llvm-11  20 \
  --slave   /usr/bin/llvm-config       llvm-config      /usr/bin/llvm-config-11  \
	--slave   /usr/bin/llvm-ar           llvm-ar          /usr/bin/llvm-ar-11 \
	--slave   /usr/bin/llvm-as           llvm-as          /usr/bin/llvm-as-11 \
	--slave   /usr/bin/llvm-bcanalyzer   llvm-bcanalyzer  /usr/bin/llvm-bcanalyzer-11 \
	--slave   /usr/bin/llvm-cov          llvm-cov         /usr/bin/llvm-cov-11 \
	--slave   /usr/bin/llvm-diff         llvm-diff        /usr/bin/llvm-diff-11 \
	--slave   /usr/bin/llvm-dis          llvm-dis         /usr/bin/llvm-dis-11 \
	--slave   /usr/bin/llvm-dwarfdump    llvm-dwarfdump   /usr/bin/llvm-dwarfdump-11 \
	--slave   /usr/bin/llvm-extract      llvm-extract     /usr/bin/llvm-extract-11 \
	--slave   /usr/bin/llvm-link         llvm-link        /usr/bin/llvm-link-11 \
	--slave   /usr/bin/llvm-mc           llvm-mc          /usr/bin/llvm-mc-11 \
	--slave   /usr/bin/llvm-mcmarkup     llvm-mcmarkup    /usr/bin/llvm-mcmarkup-11 \
	--slave   /usr/bin/llvm-nm           llvm-nm          /usr/bin/llvm-nm-11 \
	--slave   /usr/bin/llvm-objdump      llvm-objdump     /usr/bin/llvm-objdump-11 \
	--slave   /usr/bin/llvm-profdata     llvm-profdata    /usr/bin/llvm-profdata-11 \
	--slave   /usr/bin/llvm-ranlib       llvm-ranlib      /usr/bin/llvm-ranlib-11 \
	--slave   /usr/bin/llvm-readobj      llvm-readobj     /usr/bin/llvm-readobj-11 \
	--slave   /usr/bin/llvm-rtdyld       llvm-rtdyld      /usr/bin/llvm-rtdyld-11 \
	--slave   /usr/bin/llvm-size         llvm-size        /usr/bin/llvm-size-11 \
	--slave   /usr/bin/llvm-stress       llvm-stress      /usr/bin/llvm-stress-11 \
	--slave   /usr/bin/llvm-symbolizer   llvm-symbolizer  /usr/bin/llvm-symbolizer-11 \
	--slave   /usr/bin/llvm-tblgen       llvm-tblgen      /usr/bin/llvm-tblgen-11

sudo update-alternatives \
  --install /usr/bin/clang                 clang                  /usr/bin/clang-11     2000 \
  --slave   /usr/bin/lld                   lld                    /usr/bin/lld-11 \
  --slave   /usr/bin/clang-format          clang-format           /usr/bin/clang-format-11  \
  --slave   /usr/bin/clang-tidy            clang-tidy             /usr/bin/clang-tidy-11  \
  --slave   /usr/bin/clang-tidy-diff.py    clang-tidy-diff.py     /usr/bin/clang-tidy-diff-11.py \
  --slave   /usr/bin/clang-include-fixer   clang-include-fixer    /usr/bin/clang-include-fixer-11 \
  --slave   /usr/bin/clang-offload-bundler clang-offload-bundler  /usr/bin/clang-offload-bundler-11 \
  --slave   /usr/bin/clangd                clangd                 /usr/bin/clangd-11 \
  --slave   /usr/bin/clang-check           clang-check            /usr/bin/clang-check-11 \
  --slave   /usr/bin/scan-view             scan-view              /usr/bin/scan-view-11 \
  --slave   /usr/bin/clang-apply-replacements clang-apply-replacements /usr/bin/clang-apply-replacements-11 \
  --slave   /usr/bin/clang-query           clang-query            /usr/bin/clang-query-11 \
  --slave   /usr/bin/modularize            modularize             /usr/bin/modularize-11 \
  --slave   /usr/bin/sancov                sancov                 /usr/bin/sancov-11 \
  --slave   /usr/bin/c-index-test          c-index-test           /usr/bin/c-index-test-11 \
  --slave   /usr/bin/clang-reorder-fields  clang-reorder-fields   /usr/bin/clang-reorder-fields-11 \
  --slave   /usr/bin/clang-change-namespace clang-change-namespace  /usr/bin/clang-change-namespace-11 \
  --slave   /usr/bin/clang-import-test     clang-import-test      /usr/bin/clang-import-test-11 \
  --slave   /usr/bin/scan-build            scan-build             /usr/bin/scan-build-11 \
  --slave   /usr/bin/scan-build-py         scan-build-py          /usr/bin/scan-build-py-11 \
  --slave   /usr/bin/clang-cl              clang-cl               /usr/bin/clang-cl-11 \
  --slave   /usr/bin/clang-rename          clang-rename           /usr/bin/clang-rename-11 \
  --slave   /usr/bin/find-all-symbols      find-all-symbols       /usr/bin/find-all-symbols-11 \
  --slave   /usr/bin/lldb                  lldb                   /usr/bin/lldb-11 \
  --slave   /usr/bin/lldb-server           lldb-server            /usr/bin/lldb-server-11

sudo update-alternatives \
  --install /usr/bin/clang++               clang++                /usr/bin/clang++-11 2000

sudo update-alternatives --config clang
sudo update-alternatives --config clang++
