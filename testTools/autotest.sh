rm  test.log
if test $1 == "-h"
then
    echo "-a: 全测试"
    echo "-s [order]: 单样例测试"
    echo "-h 查看帮助"

elif test $1 == "-a"
    then
    set i = 0
    for file in ../testcase/*.sy
    do 
        set i = $i + 1
        echo $i
        echo "testfile ${file##*/}"
        echo "testfile ${file##*/}" >> test.log
        ./../build/compiler -a -printAST ../testcase/${file##*/}
        python3 Cgen.py -a > test.c 
        echo $?
        gcc -o Exe test.c -g
        ./Exe
        echo $? > this.out
        #echo "filename ${file##*/%.*}"
        diff this.out  ${file%.sy}.out >> test.log
    done
elif test $1 = "-s"
    then
    echo "into -s cond"
    file="../testcase/$2*.sy"
    echo "${file}"
    echo "testfile ${file##*/}"
    echo "testfile ${file##*/}" >> test.log
    ./../build/compiler -a -printAST ../testcase/${file##*/}
    python3 Cgen.py -a > test.c 
    echo $?
    gcc -o Exe test.c -g
    ./Exe
    echo $? > this.out
    #echo "filename ${file##*/%.*}"
    diff -b -q this.out  ${file%.sy}.out >> test.log
    if test $? -eq 0
    then
        echo "test $file failed" >> test.log
    else
        echo "test $file passed" >> test.log
    fi
fi