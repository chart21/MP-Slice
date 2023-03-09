helpFunction()
{
   echo "Script to compile and run 6 mixed constellations of players in parallel"
   echo -e "\t-p Party number or all for running locally"
   echo -e "\t-a IP address of player 1 (if ip matches player_id can be empty)"
   echo -e "\t-b IP address of player 2 (if ip matches player_id can be empty)"
   echo -e "\t-c IP address of player 3 (if ip matches player_id can be empty)"
   echo -e "\t-d IP address of player 4 (if ip matches player_id can be empty)"

   exit 1 # Exit script after printing help
}

while getopts "p:a:b:" opt
do
   case "$opt" in
      p ) O_PARTY="$OPTARG" ;;
      a ) IP1="$OPTARG" ;;
      b ) IP2="$OPTARG" ;;
      c ) IP3="$OPTARG" ;;
      d ) IP4="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

comp="g++"

flags="-march=native -Ofast -std=c++2a -pthread -lssl -lcrypto"

O_IP1="127.0.0.1"
O_IP2="127.0.0.1"
O_IP3="127.0.0.1"
O_IP4="127.0.0.1"

if [ ! -z "$IP1" ];
then
O_IP1="$IP1"
fi

if [ ! -z "$IP2" ];
then
O_IP2="$IP2"
fi

if [ ! -z "$IP3" ];
then
O_IP3="$IP3"
fi

if [ ! -z "$IP4" ];
then
O_IP4="$IP4"
fi





# Compile all executables for P1
if [ "$O_PARTY" = "1" ] || [ "$O_PARTY" = "all" ]
echo "Compiling executables for P1 ..."
then
    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"6000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-2-3.o $flags
    
    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"38000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-3-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"8000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--2-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"9000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--2-3-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"10000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--3-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"11000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--3-2-1.o $flags

    

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"61000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"64000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"62000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--2-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"15000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--2-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"63000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--4-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"65000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--4-2-1.o $flags


    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"18000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"19000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--1-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"20000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--3-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"21000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--3-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"22000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--4-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"23000"/" config.h
    "$comp" tmain.cpp -o ./search-P1--4-3-1.o $flags
    
fi

# Compile all executables for P2
if [ "$O_PARTY" = "2" ] || [ "$O_PARTY" = "all" ]
echo "Compiling executables for P2 ..."
then
    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"6000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--1-2-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"38000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--1-3-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"8000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"9000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-3-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"10000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--3-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"11000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--3-2-1.o $flags


    
    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"61000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--1-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"64000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--1-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"62000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"15000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"63000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--4-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"65000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--4-2-1.o $flags



    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"24000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"25000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--2-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"26000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--3-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"27000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--3-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"28000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--4-2-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"29000"/" config.h
    "$comp" tmain.cpp -o ./search-P2--4-3-2.o $flags
fi

# Compile all executables for P3
if [ "$O_PARTY" = "3" ] || [ "$O_PARTY" = "all" ]
echo "Compiling executables for P3 ..."
then
    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"6000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--1-2-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"38000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--1-3-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"8000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--2-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"9000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--2-3-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"10000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"11000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-2-1.o $flags



    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"18000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--1-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"19000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--1-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"20000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"21000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"22000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--4-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"23000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--4-3-1.o $flags



    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"24000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--2-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"25000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--2-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"26000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"27000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--3-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"28000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--4-2-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"29000"/" config.h
    "$comp" tmain.cpp -o ./search-P3--4-3-2.o $flags
fi

# Compile all executables for P4
echo "Compiling executables for P4 ..."
if [ "$O_PARTY" = "4" ] || [ "$O_PARTY" = "all" ]
then
    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"61000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--1-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"64000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--1-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"62000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--2-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"15000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--2-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"63000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-1-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"65000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-2-1.o $flags



    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"18000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--1-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"19000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--1-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"20000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--3-1-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"21000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--3-4-1.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"22000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-1-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"23000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-3-1.o $flags



    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"24000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--2-3-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"25000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--2-4-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"2"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"26000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--3-2-4.o $flags

    sed -i -e "s/\(PARTY \).*/\1"1"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"27000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--3-4-2.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"28000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-2-3.o $flags

    sed -i -e "s/\(PARTY \).*/\1"0"/" config.h
    sed -i -e "s/\(BASE_PORT \).*/\1"29000"/" config.h
    "$comp" tmain.cpp -o ./search-P4--4-3-2.o $flags
fi

echo "Finished compiling, executing..."
# Run all executables for P1
if [ "$O_PARTY" = "1" ] || [ "$O_PARTY" = "all" ]
then
    ./search-P1--1-2-3.o $O_IP3 $O_IP2 &
    ./search-P1--1-3-2.o $O_IP2 $O_IP3 &
    ./search-P1--2-1-3.o $O_IP2 $O_IP3 &
    ./search-P1--2-3-1.o $O_IP3 $O_IP2 &
    ./search-P1--3-1-2.o $O_IP3 $O_IP2 &
    ./search-P1--3-2-1.o $O_IP2 $O_IP3 &

    ./search-P1--1-2-4.o $O_IP4 $O_IP2 &
    ./search-P1--1-4-2.o $O_IP2 $O_IP4 &
    ./search-P1--2-1-4.o $O_IP2 $O_IP4 &
    ./search-P1--2-4-1.o $O_IP4 $O_IP2 &
    ./search-P1--4-1-2.o $O_IP4 $O_IP2 &
    ./search-P1--4-2-1.o $O_IP2 $O_IP4 &

    ./search-P1--1-3-4.o $O_IP4 $O_IP3 &
    ./search-P1--1-4-3.o $O_IP3 $O_IP4 &
    ./search-P1--3-1-4.o $O_IP3 $O_IP4 &
    ./search-P1--3-4-1.o $O_IP4 $O_IP3 &
    ./search-P1--4-1-3.o $O_IP4 $O_IP3 &
    ./search-P1--4-3-1.o $O_IP3 $O_IP4 &
fi
# Run all executables for P2
if [ "$O_PARTY" = "2" ] || [ "$O_PARTY" = "all" ]
then
    ./search-P2--1-2-3.o $O_IP1 $O_IP3 &
    ./search-P2--1-3-2.o $O_IP3 $O_IP1 &
    ./search-P2--2-1-3.o $O_IP3 $O_IP1 &
    ./search-P2--2-3-1.o $O_IP1 $O_IP3 &
    ./search-P2--3-1-2.o $O_IP1 $O_IP3 &
    ./search-P2--3-2-1.o $O_IP3 $O_IP1 &

    ./search-P2--1-2-4.o $O_IP1 $O_IP4 &
    ./search-P2--1-4-2.o $O_IP4 $O_IP1 &
    ./search-P2--2-1-4.o $O_IP4 $O_IP1 &
    ./search-P2--2-4-1.o $O_IP1 $O_IP4 &
    ./search-P2--4-1-2.o $O_IP1 $O_IP4 &
    ./search-P2--4-2-1.o $O_IP4 $O_IP1 &
   
    ./search-P2--2-3-4.o $O_IP4 $O_IP3 &
    ./search-P2--2-4-3.o $O_IP3 $O_IP4 &
    ./search-P2--3-2-4.o $O_IP3 $O_IP4 &
    ./search-P2--3-4-2.o $O_IP4 $O_IP3 &
    ./search-P2--4-2-3.o $O_IP4 $O_IP3 &
    ./search-P2--4-3-2.o $O_IP3 $O_IP4 &
fi
# Run all executables for P3
if [ "$O_PARTY" = "3" ] || [ "$O_PARTY" = "all" ]
then
    ./search-P3--1-2-3.o $O_IP2 $O_IP1 &
    ./search-P3--1-3-2.o $O_IP1 $O_IP2 &
    ./search-P3--2-1-3.o $O_IP1 $O_IP2 &
    ./search-P3--2-3-1.o $O_IP2 $O_IP1 &
    ./search-P3--3-1-2.o $O_IP2 $O_IP1 &
    ./search-P3--3-2-1.o $O_IP1 $O_IP2 &

    ./search-P3--1-3-4.o $O_IP1 $O_IP4 &
    ./search-P3--1-4-3.o $O_IP4 $O_IP1 &
    ./search-P3--3-1-4.o $O_IP4 $O_IP1 &
    ./search-P3--3-4-1.o $O_IP1 $O_IP4 &
    ./search-P3--4-1-3.o $O_IP1 $O_IP4 &
    ./search-P3--4-3-1.o $O_IP4 $O_IP1 &

    ./search-P3--2-3-4.o $O_IP2 $O_IP4 &
    ./search-P3--2-4-3.o $O_IP4 $O_IP2 &
    ./search-P3--3-2-4.o $O_IP4 $O_IP2 &
    ./search-P3--3-4-2.o $O_IP2 $O_IP4 &
    ./search-P3--4-2-3.o $O_IP2 $O_IP4 &
    ./search-P3--4-3-2.o $O_IP4 $O_IP2 &
fi
# Run all executables for P4
if [ "$O_PARTY" = "4" ] || [ "$O_PARTY" = "all" ]
then
    ./search-P4--1-2-4.o $O_IP2 $O_IP1 &
    ./search-P4--1-4-2.o $O_IP1 $O_IP2 &
    ./search-P4--2-1-4.o $O_IP1 $O_IP2 &
    ./search-P4--2-4-1.o $O_IP2 $O_IP1 &
    ./search-P4--4-1-2.o $O_IP2 $O_IP1 &
    ./search-P4--4-2-1.o $O_IP1 $O_IP2 &

    ./search-P4--1-3-4.o $O_IP3 $O_IP1 &
    ./search-P4--1-4-3.o $O_IP1 $O_IP3 &
    ./search-P4--3-1-4.o $O_IP1 $O_IP3 &
    ./search-P4--3-4-1.o $O_IP3 $O_IP1 &
    ./search-P4--4-1-3.o $O_IP3 $O_IP1 &
    ./search-P4--4-3-1.o $O_IP1 $O_IP3 &

    ./search-P4--2-3-4.o $O_IP3 $O_IP2 &
    ./search-P4--2-4-3.o $O_IP2 $O_IP3 &
    ./search-P4--3-2-4.o $O_IP2 $O_IP3 &
    ./search-P4--3-4-2.o $O_IP3 $O_IP2 &
    ./search-P4--4-2-3.o $O_IP3 $O_IP2 &
    ./search-P4--4-3-2.o $O_IP2 $O_IP3 &
fi

