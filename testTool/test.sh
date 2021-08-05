#!/usr/bin/env bash
if test "$1" == "-h"
then
  echo "-a :测试全部样例"
  echo "-s [file] 测试单个样例 file为文件名"
  echo "-h 查看帮助"
elif test "$1" == "-a"
then
  cd ../build || exit
  cmake ..
  make
  cd ../testTool || exit
  for file in ../testcase/*.sy
  do
    if test -e test.s
    then
      rm test.s
    fi
    if test -e test.sy
    then
      rm test.sy
    fi
    if test -e test
    then
      rm ./test
    fi
    cp ../testcase/"${file##*/}" testcase.sy
    echo "testcase: ${file##*/}"
    ../build/compiler -S -o ../testTool/testcase.s ../testTool/testcase.sy

    if test $? -eq 1
    then
      echo "testcase: ${file##*/} execute fail"
      continue
    fi

  done
fi