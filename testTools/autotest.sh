#!/usr/bin/env bash
touch test.log
rm  test.log

if test $1 == "-h"
then
    echo "-a : 全测试 仅输出diff比较结果"
    echo "-s [order]: 单样例测试 输出完整信息"
    echo "-n [num] :测试前n个样例"
    echo "-l 展示所有测试样例的名字"
    echo "-h 查看帮助"
    echo "-d 测试本目录下的testcase.sy文件"
    echo "-b 编译项目"
elif test "$1" == "-b"
then
    cd ..
    cd build
    cmake ..
    make
    cd ..
    cd testTools
elif test $1 == "-l"
then
    for file in ../testcase/*.sy
    do 
        echo ${file##/}
    done
elif test $1 == "-n"
then
    let "i=0"
    for file in ../testcase/*.sy
    do 
        echo $i $2
        if test $i -gt $2 
        then
            break
        fi
        let "i++"
        touch testcase.sy
        rm testcase.sy
        touch this.out
        rm this.out
        cp ../testcase/${file##*/} testcase.sy 
        #echo "${file}"
        echo "testfile ${file##*/}:"
        echo "testfile ${file##*/}" >> test.log
        ./../build/compiler -printIR ../testcase/${file##*/} >> test.log
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error: Ir generated failed \033[0m" 
        continue
        fi
        python3 Cgen.py -a > test.c 
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error: C generated failed \033[0m" 
        continue
        fi
        gcc -o Exe test.c -g 2>> test.log
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error: C compile failed \033[0m" 
        continue
        fi
        touch ${file%.sy}.in
        ./Exe < ${file%.sy}.in > this.out
        #res = $?
        #if test ${res} -eq 1 
        #then
        #echo -e "\033[31m C excuted failed\33[0m" 
        #fi
        echo $? >> this.out
        #echo "filename ${file##*/%.*}"
        if test 1 -eq 1 
        then
            diff -b -q this.out  ${file%.sy}.out
            if test $? -eq 1
            then
                rm std.out
                rm ../build/test.sy
                cp ${file%.sy}.out std.out
                cp ${file%.sy}.sy ../build/test.sy
                echo -e "\033[31m test $file failed \033[0m"
            else
                echo -e "\033[32m test $file passed \033[0m"
            fi
        else
            diff -b -q this.out  ${file%.sy}.out
            diff -b this.out  ${file%.sy}.out  >> test.log
            if test $? -eq 0
            then
                touch std.out
                rm std.out
                touch test.sy
                rm ../build/test.sy
                cp ${file%.sy}.out std.out
                cp ${file%.sy}.sy ../build/test.sy
                echo "//$file failesd" >> ../build/test.sy
                echo "test $file failed" >> test.log
            else
                echo -e "test $file passed" >> test.log
            fi
        fi
    done
elif test $1 == "-a"
    then

    let "i=0"
    let "sum=0"
    for file in ../testcase/*.sy
    do 
    
        touch testcase.sy
        rm testcase.sy
        touch this.out
        rm this.out
        cp ../testcase/${file##*/} testcase.sy 
        #echo "${file}"
        echo "testfile ${file##*/}:"
        echo "testfile ${file##*/}" > test.log
        ./../build/compiler -a -printIR ../testcase/${file##*/} > test.data 2>>test.log
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error:Ir generated failed \033[0m" 
        continue
        fi
        python3 Cgen.py -a > test.c 2> test.data
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error: generated failed \033[0m" 
        continue
        fi
        gcc -o Exe test.c -g 2> test.data
        if test $? -eq 1 
        then
        echo -e "\033[31m test $file error:C compile failed \033[0m" 
        continue
        fi
        touch ${file%.sy}.in
        timeout 8 ./Exe < ${file%.sy}.in > this.out
        #res = $?
        #if test ${res} -eq 1 
        #then
        #echo -e "\033[31m C excuted failed\33[0m" 
        #fi
        echo $? >> this.out
        let "i++"
        #echo "filename ${file##*/%.*}"
        if test 1 -eq 1 
        then
            diff -b -q this.out  ${file%.sy}.out
            if test $? -eq 1
            then
                rm std.out
                rm ../build/test.sy
                cp ${file%.sy}.out std.out
                cp ${file%.sy}.sy ../build/test.sy
                echo -e "\033[31m test $file failed \033[0m"
            else
                echo -e "\033[32m test $file passed \033[0m"
                let "sum++"
            fi
        else
            diff -b -q this.out  ${file%.sy}.out
            diff -b this.out  ${file%.sy}.out  >> test.log
            if test $? -eq 0
            then
                touch std.out
                rm std.out
                touch test.sy
                rm ../build/test.sy
                cp ${file%.sy}.out std.out
                cp ${file%.sy}.sy ../build/test.sy
                echo "//$file failesd" >> ../build/test.sy
                echo "test $file failed" >> test.log
            else
                echo -e "test $file passed" >> test.log
            fi
        fi
    done
    echo $sum / $i "passed"
elif test $1 = "-s"
    then
    #echo "into -s $2 $3 cond"
    file="../testcase/$2*.sy"
    touch testcase.sy
    rm testcase.sy
    touch this.out
    rm this.out
    cp ../testcase/${file##*/} testcase.sy 
    #echo "${file}"
    echo "testfile ${file##*/}:"
    echo "testfile ${file##*/}" 
    ./../build/compiler -a -printIR ../testcase/${file##*/}
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:Ir generated failed \033[0m" 
    fi
    python3 Cgen.py -a > test.c 
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:C generated failed \033[0m" 
    fi
    gcc -o Exe test.c -g
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:C compile failed \033[0m" 
    fi
    touch ${file%.sy}.in
    ./Exe < ${file%.sy}.in > this.out
    #res = $?
    #if test ${res} -eq 1 
    #then
    #echo -e "\033[31m C excuted failed\33[0m" 
    #fi
    echo $? >> this.out
    #echo "filename ${file##*/%.*}"
    if test 1 -eq 0 
    then
        diff -b -q this.out  ${file%.sy}.out
        diff -b this.out  ${file%.sy}.out  >> test.log
        if test $? -eq 0
        then
            touch std.out
            rm std.out
            touch test.sy
            rm ../build/test.sy
            cp ${file%.sy}.out std.out
            cp ${file%.sy}.sy ../build/test.sy
            echo "//$file failesd" >> ../build/test.sy
            echo "test $file failed" >> test.log
        else
            echo -e "test $file passed" >> test.log
        fi
    else
        diff -b -q this.out  ${file%.sy}.out
        if test $? -eq 1
        then
            rm std.out
            rm ../build/test.sy
            cp ${file%.sy}.out std.out
            cp ${file%.sy}.sy ../build/test.sy
            echo "//$file failed" >> ../build/test.sy
            echo -e "\033[31m test $file failed\033[0m"
        else
            rm std.out
            rm ../build/test.sy
            cp ${file%.sy}.out std.out
            cp ${file%.sy}.sy ../build/test.sy
            echo -e "\033[32m test $file passed\033[0m"
        fi
    fi
elif test $1 = "-d"
    then
    file="testcase.sy"
    touch this.out
    rm this.out
    echo "testfile $file:"
    ./../build/compiler -a -printIR $file
    cat $file
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:Ir generated failed \033[0m" 
    fi
    python3 Cgen.py -a > test.c 
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:C generated failed \033[0m" 
    fi
    gcc -o Exe test.c -g
    if test $? -eq 1 
    then
    echo -e "\033[31m test $file error:C compile failed \033[0m" 
    fi
    touch test.in
    ./Exe < test.in 
    echo $?
    #echo "filename ${file##*/%.*}"
    diff -b -q this.out  ${file%.sy}.out
    if test $? -eq 1
    then
        rm ../build/test.sy
        cp ${file%.sy}.sy ../build/test.sy
        echo "//$file failed" >> ../build/test.sy
        echo -e "\033[31m test $file failed\033[0m"
    else
        rm std.out
        rm ../build/test.sy
        cp ${file%.sy}.sy ../build/test.sy
        echo -e "\033[32m test $file passed\033[0m"
    fi
fi