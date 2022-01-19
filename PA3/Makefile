out : hasse.o
	gcc hasse.o ./ezdib/ezdib.c -o out -lm
hasse.o : hasse.c
	gcc -c hasse.c ./ezdib/ezdib.c -lm
clean :
	rm out *.o
