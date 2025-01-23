default:
	cc game.c -o game -Llib -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
