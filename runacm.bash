#!/bin/bash

make -f Makefileacm clean; make -f Makefileacm runacm;

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d             : default"
    echo " [0] -21.02.2023    : analize data from 21.02.2023"
    echo " [0] -23.02.2023    : analize data from 23.02.2023"
    echo " [0] -24.02.2023_01 : analize data from 24.02.2023_01"
    echo " [0] -24.02.2023_02 : analize data from 24.02.2023_02"
    echo " [0] -24.02.2023_03 : analize data from 24.02.2023_03"
    echo " [0] -24.02.2023_04 : analize data from 24.02.2023_04"
    echo " [0] -26.02.2023_01 : analize data from 26.02.2023_01"
    echo " [0] -26.02.2023_02 : analize data from 26.02.2023_02"
    echo " [0] -26.02.2023_03 : analize data from 26.02.2023_03"
    echo " [0] -26.02.2023_04 : analize data from 26.02.2023_04"
    echo " [0] -27.02.2023_03 : analize data from 27.02.2023_03"
    echo " [0] -h             : print help"
}

function ana {
    root_data_dir=$1
    #echo "$root_data_dir"
    rm -rf $root_data_dir/c1_DCR*pdf
    rm -rf $root_data_dir/c1_CT*pdf
    rm -rf $root_data_dir/log
    rm -rf merg_c1_DCR
    rm -rf merg_c1_CT
    ls -lrt $root_data_dir/*txt.root | awk '{print $9}' > $root_data_dir/rootFileList.list
    ./runacm 0 $root_data_dir $root_data_dir/rootFileList.list $root_data_dir/info $root_data_dir/MeasurementConditions.txt $root_data_dir/temperature $root_data_dir/trigger_window $root_data_dir/hist.root $root_data_dir/fit_DCR_single.pe $root_data_dir/fit_CT_single.pe | tee $root_data_dir/log
    #gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=$root_data_dir/c1_DCR.pdf `ls -lrt $root_data_dir/c1_DCR_*pdf | awk '{print $9}' | xargs`
    source merg_c1_DCR
    source merg_c1_CT
}
    
if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
        printHelp
    elif [ "$1" = "-21.02.2023" ]; then
	ana ../data/21.02.2023/
    elif [ "$1" = "-23.02.2023" ]; then
	ana ../data/23.02.2023/
    elif [ "$1" = "-24.02.2023_01" ]; then
	ana ../data/24.02.2023_01/
    elif [ "$1" = "-24.02.2023_02" ]; then
	ana ../data/24.02.2023_02/
    elif [ "$1" = "-24.02.2023_03" ]; then
	ana ../data/24.02.2023_03/
    elif [ "$1" = "-24.02.2023_04" ]; then
	ana ../data/24.02.2023_04/
    elif [ "$1" = "-26.02.2023_01" ]; then
	ana ../data/26.02.2023_01/
    elif [ "$1" = "-26.02.2023_02" ]; then
	ana ../data/26.02.2023_02/
    elif [ "$1" = "-26.02.2023_03" ]; then
	ana ../data/26.02.2023_03/
    elif [ "$1" = "-26.02.2023_04" ]; then
	ana ../data/26.02.2023_04/
    elif [ "$1" = "-27.02.2023_03" ]; then
	time ana ../data/27.02.2023_03/
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
