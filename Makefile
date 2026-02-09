dish: 
	gcc -o dish -Wall -Wextra -Wpedantic dish.c main.c lex.c 

clean:
	rm -rf dish