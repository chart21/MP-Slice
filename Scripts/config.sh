#!/bin/bash

helpFunction()
{
   exit 1 # Exit script after printing help
}

while getopts "b:t:d:p:f:o:i:" opt
do
   case "$opt" in
      b ) base_port="$OPTARG" ;;
      l ) BITLENGTH="$OPTARG" ;;
      d ) DATATYPE="$OPTARG" ;;
      c ) compress="$OPTARG" ;;
      f ) function_name="$OPTARG" ;;
      n ) num_inputs="$OPTARG" ;;
      p ) protocol="$OPTARG" ;;
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

if [ ! -z "$base_port" ]
then
    sed -i -e "s/\(base_port \).*/\1$base_port/" DATATYPE.h
fi
if [ ! -z "$BITLENGTH" ]
then
    sed -i -e "s/\(BITLENGTH \).*/\1$BITLENGTH/" DATATYPE.h
fi
if [ ! -z "$num_inputs" ]
then
    sed -i -e "s/\(n \).*/\1$num_inputs/" DATATYPE.h
fi
if [ ! -z "$DATATYPE" ]
then
    sed -i -e "s/\(DATATYPE \).*/\1$DATATYPE/" DATATYPE.h
fi
if [ ! -z "$function_name" ]
then
    sed -i -e "s/\(performFunction \).*/\1$function_name/" DATATYPE.h
fi
if [ ! -z "$operator" ]
then
    sed -i -e "s/\(op \).*/\1$operator/" config.h
fi
if [ ! -z "$input_length" ]
then
    sed -i -e "s/\(INPUTSLENGTH \).*/\1\{$input_length\}/" DATATYPE.h
fi

