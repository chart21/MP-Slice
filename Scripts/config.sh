#!/bin/bash

helpFunction()
{
   exit 1 # Exit script after printing help
}

while getopts "b:a:d:c:f:n:s:i:l:p:o:u:" opt
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

      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

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
    sed -i -e "s/\(define n \).*/\1$NUM_INPUTS/" config.h
fi
if [ ! -z "$DATATYPE" ]
then
    sed -i -e "s/\(DATTYPE \).*/\1$DATATYPE/" config.h
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
        if [ "$LIVE" = "false" ] && [ "$INIT" = "true" ]; 
        then
            g++ -std=c++2a tmain.cpp -o ./search-P"$i"-INIT.o -msse4.1 -Ofast
            ./search-P"$i"-INIT.o
            sed -i -e "s/\(LIVE \).*/\1true/" config.h
            sed -i -e "s/\(INIT \).*/\1FALSE/" config.h
        fi
            g++ -std=c++2a tmain.cpp -o ./search-P"$i".o -msse4.1 -Ofast
    fi
done

