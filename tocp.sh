#!/bin/bash

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] dir name : where to copy"
    echo " [0] -h       : print help"
}

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-h" ]; then
        printHelp
    elif [ $# -eq 1 ]; then
	cp info $1/.
	cp info.C $1/.
	cp temp_trigg_w_fit_spe_DCR_CT.sh $1/.
	cp ../txttoroot/txttoroot $1/.
	cp ../txttoroot/txttoroot.sh $1/.
    else
        printHelp
    fi
fi

#espeak "I have done"
