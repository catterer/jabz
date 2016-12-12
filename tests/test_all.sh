#!/bin/bash

function run_test {
    NAME=$1
    cat $NAME.in  | ../jabz  | hexdump -C > tmp
    RES=`diff tmp $NAME.out | wc -l`
    if [[ "$RES" -ne "0" ]]
    then
        echo "Failed    $NAME"
        cp tmp $NAME.failed
    else
        echo "OK        $NAME"
    fi
    rm tmp
}  

run_test 1
run_test num
run_test ext
