#!/bin/bash
helpFunction()
{
   echo "Script to compile and run 6 mixed constellations of players in parallel"
   echo -e "\t-p Party number or all for running locally"
   echo -e "\t-a IP address of lower index player "
   echo -e "\t-b IP address of higher index player "
   exit 1 # Exit script after printing help
}

while getopts "p:a:b:" opt
do
   case "$opt" in
      p ) PARTY="$OPTARG" ;;
      a ) IP1="$OPTARG" ;;
      b ) IP2="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

comp="g++"

flags="-march=native -Ofast -std=c++2a -pthread -lssl -lcrypto"


for i in {0..2}
    do
    for j in {0..2}
        do
            for z in {0..1}
                do
                    let "s = (1-z)*(j+i)%3+z*(3+2-i-j)%3"
                    # s= $(((1-z)*(j+i)%3+z*(2-i-j)%3))
                    echo $s
            if [ "$i" = "$PARTY" ] || [ "$PARTY" = "all" ];
            then
                sed -i -e "s/\(PARTY \).*/\1"$s"/" config.h
                sed -i -e "s/\(BASE_PORT \).*/\1"$((6000 + (j+z*3) * 1000))"/" config.h
                if [ "$LIVE" = "0" ] && [ "$INIT" = "1" ]; 
                then
                    sed -i -e "s/\(LIVE \).*/\10/" config.h
                    sed -i -e "s/\(INIT \).*/\11/" config.h
                    echo "Compiling INIT executable for P-"$s"-"$i"-"$z" ..."
                    "$comp" tmain.cpp -o ./search-P"$s"-"$i"-"$z"-INIT.o $flags
                    ./search-P"$j"-"$i"-"$z"-INIT.o
                    sed -i -e "s/\(LIVE \).*/\11/" config.h
                    sed -i -e "s/\(INIT \).*/\10/" config.h
                fi
                    echo "Compiling executable for P-"$s"-"$i"-"$z" ..."
                    "$comp" tmain.cpp -o ./search-P"$s"-"$i"-"$z".o $flags
            fi
        done
    done
done
echo "Finished compiling"
echo "Executing"

        if [ "$PARTY" = "all" ];
        then
        ./search-P0-0-0.o  & ./search-P0-1-1.o  &./search-P0-2-0.o  &./search-P0-0-1.o  &./search-P0-1-0.o  &./search-P0-2-1.o  &./search-P1-0-0.o  &./search-P1-1-1.o  &./search-P1-2-0.o  &./search-P1-0-1.o  &./search-P1-1-0.o  &./search-P1-2-1.o  &./search-P2-0-0.o  &./search-P2-1-1.o  &./search-P2-2-0.o  &./search-P2-0-1.o  &./search-P2-1-0.o  &./search-P2-2-1.o  
        elif [ "$PARTY" = "0" ];
        then
./search-P0-"$PARTY"-0.o "$IP1" "$IP2" & ./search-P0-"$PARTY"-1.o "$IP2" "$IP1" & ./search-P1-"$PARTY"-0.o "$IP2" "$IP1" & ./search-P1-"$PARTY"-1.o "$IP1" "$IP2" &./search-P2-"$PARTY"-0.o "$IP1" "$IP2" &./search-P2-"$PARTY"-1.o "$IP2" "$IP1"
        elif [ "$PARTY" = "1" ];
        then
            ./search-P0-"$PARTY"-0.o "$IP2" "$IP1" & ./search-P0-"$PARTY"-1.o "$IP1" "$IP2" & ./search-P1-"$PARTY"-0.o "$IP1" "$IP2" &./search-P1-"$PARTY"-1.o "$IP2" "$IP1" &./search-P2-"$PARTY"-0.o "$IP2" "$IP1" &./search-P2-"$PARTY"-1.o "$IP1" "$IP2"
        elif [ "$PARTY" = "2" ];
        then
            ./search-P0-"$PARTY"-0.o "$IP1" "$IP2" & ./search-P0-"$PARTY"-1.o "$IP2" "$IP1" & ./search-P1-"$PARTY"-0.o "$IP2" "$IP1" &./search-P1-"$PARTY"-1.o "$IP1" "$IP2" &./search-P2-"$PARTY"-0.o "$IP1" "$IP2" &./search-P2-"$PARTY"-1.o "$IP2" "$IP1"
        fi


