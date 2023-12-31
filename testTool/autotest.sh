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
  echo -e "-s [input] 测试单个样例 input为要测试的文件名\n           默认的测试文件输入为同名的in文件\n
           默认输出为testTool文件夹下的testcase.log"
  echo "-a 测试testcase目录下的全部样例"
  echo "-b 自动build 工程"
  echo "-h 查看帮助"
elif test "$1" == "-a"
then
  if test [! -d "../build"]
  then
    mkdir ../build
    cd ../build || exit
    cmake ..
    make
    cd ../testTool || exit
  fi

  total=0
  success=0

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
    ../build/compiler -S -o testcase.s testcase.sy

    if test $? -eq 1
    then
      echo "testcase: ${file##*/} assembly fail"
      continue
    else
      arm-linux-gnueabihf-gcc -x assembler testcase.s -Werror -o testcase -static -L . -lsysy
    fi

    if test $? -eq 1
    then
      echo "testcase ${file##*/} link fail"
      continue
    fi

    filename=${file##*/}
    basename=${filename%.*}
    inputfile="${basename}.in"
    if test -e "../testcase/${inputfile}"
    then
      cp "../testcase/${inputfile}" testcase.in
      ./testcase < testcase.in > testcase.out
      echo -e "\n" testcase.out
      echo $? >> testcase.out
      sed '/^\s*$/d' testcase.out
      sed '/^[  ]*$/d' testcase.out
    else
      ./testcase > testcase.out
      echo -e "\n" > testcase.out
      echo $? >> testcase.out
      sed '/^\s*$/d' testcase.out
      sed '/^[  ]*$/d' testcase.out
    fi

#    sed -i '1d' testcase.out

    outputfile="${basename}.out"
    diff -b -q "../testcase/${outputfile}" testcase.out

#    if test $? -eq 1
#    then
#      echo "test ${file##*/} assembly error"
#    else
#      echo "test ${file##*/} pass"
#    fi

  done

elif test "$1" == "-s"
then
  inputFile="$2"
  if test -e "testcase.s"
  then
  rm testcase.s
  fi

  if test -e "testcase.sy"
  then
  rm testcase.sy
  fi
  cp "${inputFile}" testcase.sy

  if test -e "testcase.in"
  then
    rm testcase.in
  fi

  dir=$(dirname "$inputFile")

  basename=${inputFile##*/}

  inputfile="${basename%%.*}.in"

  if test -e "testcase.out"
  then
    rm testcase.out
  fi

  ../build/compiler -S -o testcase.s testcase.sy

  if test $? == 1
  then
    echo "${inputFile} assembly fail"
  else
    arm-linux-gnueabihf-gcc -x assembler testcase.s -Werror -o testcase -static -L . -lsysy
  fi

  if test -e "${dir}/${inputfile}"
  then
    cp "${dir}/${inputfile}" testcase.in
    ./testcase < testcase.in >testcase.out
#    echo -e "\n" > testcase.out
    echo $? >>testcase.out
#    sed '/^\s*$/d' testcase.out
#    sed '/^[  ]*$/d' testcase.out
  else
    ./testcase >testcase.out
#    echo -e "\n" > testcase.out
    echo $? >>testcase.out
#    sed '/^\s*$/d' testcase.out
#    sed '/^[  ]*$/d' testcase.out
  fi

#  sed -i "1d" testcase.out

  outputfile="${basename%%.*}.out"
  diff -b testcase.out "${dir}/${outputfile}"

#  if test $? -eq 1
#  then
#    echo -e "test ${inputFile##/*} fail"
#  else
#    echo -e "test ${inputFile##/*} pass"
#  fi

#build项目
elif test "$1" == "-b"
then
    cd ../build || exit
    cmake ..
    make
    cd ../testTool || exit

fi