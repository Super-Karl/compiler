rm  test.log
if test $1 == "-a"
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
    diff this.out  ${file%.sy}.out >> test.log
fi