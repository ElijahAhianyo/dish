dish: 
	gcc -o dish -g -Wall -Wextra -Wpedantic dish.c main.c lex.c command.c compiler.c executer.c memory.c

clean:
	rm -rf dish