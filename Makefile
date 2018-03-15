all:
	mkdir -p bin
	mkdir -p jail
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

fetch_sets:
	mkdir share/pavelock/sets/freebsd_11_1_aarch64
	cd share/pavelock/sets/freebsd_11_1_aarch64 && fetch http://www.mirrorservice.org/sites/ftp.freebsd.org/pub/FreeBSD/releases/arm64/aarch64/11.1-RELEASE/base.txz

	mkdir share/pavelock/sets/freebsd_11_1_amd64
	cd share/pavelock/sets/freebsd_11_1_amd64 && fetch http://www.mirrorservice.org/sites/ftp.freebsd.org/pub/FreeBSD/releases/amd64/11.1-RELEASE/base.txz

	mkdir share/pavelock/sets/freebsd_9_3_amd64
	cd share/pavelock/sets/freebsd_9_3_amd64 && fetch http://www.mirrorservice.org/sites/ftp.freebsd.org/pub/FreeBSD/releases/amd64/9.3-RELEASE/base.txz

	mkdir share/pavelock/sets/freebsd_11_1_i386
	cd share/pavelock/sets/freebsd_11_1_i386 && fetch http://www.mirrorservice.org/sites/ftp.freebsd.org/pub/FreeBSD/releases/i386/11.1-RELEASE/base.txz

	mkdir share/pavelock/sets/freebsd_9_3_i386
	cd share/pavelock/sets/freebsd_9_3_i386 && fetch http://www.mirrorservice.org/sites/ftp.freebsd.org/pub/FreeBSD/releases/i386/9.3-RELEASE/base.txz
