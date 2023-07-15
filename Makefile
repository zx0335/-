all:
	arm-linux-gcc -o ./bin/pho ./src/* -I ./include -pthread