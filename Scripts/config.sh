#!/bin/bash

helpFunction()
{
   exit 1 # Exit script after printing help
}

while getopts "b:a:d:c:f:n:s:i:l:p:o:u:g:x:" opt
do
   case "$opt" in
      b ) BASE_PORT="$OPTARG" ;;
      a ) BITLENGTH="$OPTARG" ;;
      d ) DATTYPE="$OPTARG" ;;
      c ) COMPRESS="$OPTARG" ;;
      f ) FUNCTION="$OPTARG" ;;
      n ) NUM_INPUTS="$OPTARG" ;;
      s ) PROTOCOL="$OPTARG" ;;
      i ) INIT="$OPTARG" ;;
      l ) LIVE="$OPTARG" ;;
      p ) PARTY="$OPTARG" ;;
      o ) OPT_SHARE="$OPTARG" ;;
      u ) NUM_PLAYERS="$OPTARG" ;;
      g ) GNU_OPTIONS="$OPTARG" ;;
      x ) COMPILER="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

comp="g++"
if [ ! -z "$COMPILER" ]
then
comp="$COMPILER"
fi

flags="-msse4.1 -Ofast -std=c++2a"

if [ ! -z "$GNU_OPTIONS" ]
then
flags="$GNU_OPTIONS"
fi
# Print helpFunction in case parameters are empty
# if [ -z "$parameterA" ] || [ -z "$parameterB" ] || [ -z "$parameterC" ]
# then
#    echo "Some or all of the parameters are empty";
#    helpFunction
# fi

# Begin script in case all parameters are correct

if [ ! -z "$BASE_PORT" ]
then
    sed -i -e "s/\(base_port \).*/\1$BASE_PORT/" config.h
fi
if [ ! -z "$BITLENGTH" ]
then
    sed -i -e "s/\(BITLENGTH \).*/\1$BITLENGTH/" config.h
fi
if [ ! -z "$NUM_INPUTS" ]
then
    sed -i -e "s/\(NUM_INPUTS \).*/\1$NUM_INPUTS/" config.h
fi
if [ ! -z "$DATTYPE" ]
then
    sed -i -e "s/\(DATTYPE \).*/\1$DATTYPE/" config.h
fi
if [ ! -z "$FUNCTION" ]
then
    sed -i -e "s/\(FUNCTION \).*/\1$FUNCTION/" config.h
fi
if [ ! -z "$COMPRESS" ]
then
    sed -i -e "s/\(COMPRESS \).*/\1$COMPRESS/" config.h
fi
if [ ! -z "$LIVE" ]
then
    sed -i -e "s/\(LIVE \).*/\1$LIVE/" config.h
fi
if [ ! -z "$INIT" ]
then
    sed -i -e "s/\(INIT \).*/\1$INIT/" config.h
fi
if [ ! -z "$OPT_SHARE" ]
then
    sed -i -e "s/\(OPT_SHARE \).*/\1$OPT_SHARE/" config.h
fi
if [ ! -z "$PROTOCOL" ]
then
    sed -i -e "s/\(PROTOCOL \).*/\1$PROTOCOL/" config.h
fi
if [ ! -z "$PARTY" ]
then
    sed -i -e "s/\(PARTY \).*/\1$PARTY/" config.h
fi
if [ ! -z "$NUM_PLAYERS" ]
then
    sed -i -e "s/\(num_players \).*/\1$NUM_PLAYERS/" config.h
fi

for i in {0..2}
do
    if [ "$i" = "$PARTY" ] || [ "$PARTY" = "all" ];
    then
        if [ "$PARTY" = "all" ];
        then        
            sed -i -e "s/\(PARTY \).*/\1"$i"/" config.h
        fi
        if [ "$LIVE" = "0" ] && [ "$INIT" = "1" ]; 
        then
            sed -i -e "s/\(LIVE \).*/\10/" config.h
            sed -i -e "s/\(INIT \).*/\11/" config.h
            echo "Compiling INIT executable for P-$i ..."
            "$comp" tmain.cpp -o ./search-P"$i"-INIT.o $flags
            ./search-P"$i"-INIT.o
            sed -i -e "s/\(LIVE \).*/\11/" config.h
            sed -i -e "s/\(INIT \).*/\10/" config.h
        fi
            echo "Compiling executable for P-$i ..."
            "$comp" tmain.cpp -o ./search-P"$i".o $flags
    fi
done
echo "Finished compiling"
