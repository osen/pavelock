all:
	mkdir -p bin
	c++ -g src/*.cpp -obin/pavelock

plsh:
	echo '#include <stdlib.h>' > plsh.c
	echo "int main(){system(\"$(PWD)/bin/pavelock\");}" >> plsh.c
	cc plsh.c -oplsh
	su root -c 'cp plsh /tmp/plsh && chmod u+s /tmp/plsh'
	rm plsh.c
	rm plsh

clean:
	rm bin/pavelock
