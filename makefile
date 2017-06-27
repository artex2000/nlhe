SRC=nlhe.c deck.c rand.c print.c input.c eval.c util.c stat.c

all: 
	gcc -o nlhe $(SRC)
