#!/bin/sh

while [ 1 ]; do
    dname=$(uuidgen)
    mkdir $dname
    cd $dname
    ../../../_build/2048-ai cli expectimax 
    cd ..
    python mana_files.py $dname/moves.out $dname/full.out
    rm -rf $dname
done
