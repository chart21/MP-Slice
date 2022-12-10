# MP-Slice: Seucre Mulitparty Computaiton Framework supporting Bitslicing and V-Slicing

## Usage
First configure the program using config.sh. Then run the player executable

### Configuaration
Run ./Scripts/config.sh with your desired parameters. 

```
  Script to configure and compile executables for a run.
  Only arguments you want to change have to be set.
  -n Number of elements
  -b base_port: Needs to be the same for all players for successful networking (e.g. 6000)
  -d Datatype used for slicing: 1(bool),8(char),64(uint64),128(SSE),256(AVX),512(AVX512)
  -p Player ID (0/1/2). Use all for compilling all platers
  -f Name of the function to execute
  -c Pack Bool in Char before sending? (0/1). Only used with -d 1
  -s MPC Protocol (1(Sharemind),2(Replicated),3(Astra),4(OEC DUP),5(OEC REP),6(TTP))
  -i Initialize circuit seperatly (0) or at runtime (1)?
  -l Include the Online Phase in this executable  (0/1)?
  -e Compile circuit with Preprocessing phase before online phase (0/1) (only with -d 4 -i 1 -l 1)?
  -o Use additional assumptions to optimize the sharing phase? (0/1)
  -u Number of players in total
  -g Compile flags (other than standard)
  -x Compiler (g++/clang++/..)
```

The following configuration compiles an executable for P2, 1024 inputs, sliced 256 times in an AVX variable, using Protocol OEC-DUP. The executable contains an initialization phase and an online phase.
> ./Scripts/config.sh -p 2 -n 1024 -d 256 -s 4 -l 1 -i 1 -e 0 

The following configuration uses the previous configuration but adds a preprocessing phase before the online phase (only supported with OEC-DUP and OPT_Share currently).
> ./Scripts/config.sh -p 2 -e 1 -o 1

The following configuration uses the previous configuration but compiles an executable for all players. Useful when running the parties on the same host.
> ./Scripts/config.sh -p all

The following configuration compiles 6 executables for player 2 (all player combinations) and runs all executables in parallel.
> ./split-roles.sh -p 2 -a IP_P0 -b IP_P1

### Execution
Execute P0 executable. (Assuming TTP is set to external via -t 0)
> ./search-P0.o IP_P0 IP_P2

Execute P1 executable.
> ./search-P1.o IP_P0 IP_P2


Execute all players locally on one host (requires previous config.sh with -p all):
> ./Scripts/run_protocol.sh -f search

Programs ran successful if bit at index 7 is 1 and all others are 0.
