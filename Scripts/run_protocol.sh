#!/bin/bash
helpFunction()
{
   echo ""
   echo -e "\t-p protocol name"
   echo -e "\t-s opt_share"
   echo -e "\t-c Pack bits?"
   echo -e "\t-d Datatype to use"
   exit 1 # Exit script after printing help
}

while getopts "p:s:c:d" opt
do
   case "$opt" in
      p ) protocol="$OPTARG" ;;
      s ) optshare="$OPTARG" ;;
      c ) compress="$OPTARG" ;;
      d ) datatype="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

./searchMaint.o 0 "$protocol" "$optshare" & ./searchMaint.o 1 "$protocol" "$opt_share" & ./searchMaint.o 2 "$protocol" "$opt_share"
