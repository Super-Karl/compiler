#!/usr/bin/env bash
a=0
if test $# == $[a]
then
  echo "-a :测试全部样例"
  echo "-s [file] 测试单个样例 file为文件名"
  echo "-h 查看帮助"
  echo "为了保证本脚本能够正确运行,请将静态运行时库 .a文件放到testTool文件夹下,并部署到树莓派上进行测试"
fi

if test "$1" == "-h"
then
  echo -e "为了保证本脚本能够正确运行,请将静态运行时库 .a文件放到testTool文件夹下\n并部署到树莓派上进行测试\n"
  echo "-a :测试testcase目录下的全部样例"
  echo -e "-s [input] 测试单个样例 input为要测试的文件名\n           默认的测试文件输入为同名的in文件\n           默认输出为testTool文件夹下的testcase.log"
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
      echo "testcase: ${file##*/} assembly fail"
      continue
    fi

    arm-linux-gnueabihf-gcc -x assembler testcase.s -Werror -o test -static -L . -lsysy

    if test $? -eq 1
    then
      echo "testcase ${file##/*} link fail"
      continue
    fi

    basename=${base $file}
    inputfile="${basename}"".in"
    if test -e "../testcase/${inputfile}"
    then
      echo "存在输入文件"
    fi
  done
fi