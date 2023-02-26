#!/bin/bash

ls -lrt *.txt.root | awk {'print $9'} | sed 's/_/ /g' | awk {'print $1" "$4'} > temperature
ls -lrt *.txt.root | awk {'print $9'} | sed 's/_/ /g' | awk {'print $1" "1580" "1630'} > trigger_window
ls -lrt *.txt.root | awk {'print $9'} | sed 's/_/ /g' | awk {'print $1"  "0"  -999"'} > fit_DCR_single.pe
ls -lrt *.txt.root | awk {'print $9'} | sed 's/_/ /g' | awk {'print $1"  "0"  -999"'} > fit_CT_single.pe
