# Makefile for eu
# Copyright (C) 2006-2025 Arturo Bianchi <the.craftsman@rinux.it>
#
# RCS ID: $Id: Makefile,v 1.2 2006/04/13 11:03:53 IW0DER Exp root $
#
# $Log: Makefile,v $
# Revision 1.2  2006/04/13 11:03:53  IW0DER
# fix so_open error, extract query code from main code,
# use strtok for read more name from single line.
#
# Revision 1.1  2006/04/08 10:57:04  IW0DER
# First version - DAS 1.0 compatible
#


#
# Package version
#
AUTHOR		= IW0DER
PACKAGE		= eu
VERSION		= 1.3


#
# Paths
#
BIN_DIR		= /usr/bin
ETC_DIR		= /etc/eu
VAR_DIR		= /var/eu
MAN_DIR		= /usr/man


#
# Files
#
CFILES  = eu.c query.c unique.c
DFILES  =
EFILES  = $(DFILES) eu
HFILES  = eu.h query.h unique.h common.h whenmake.h
MFILES  = eu.1 
RFILES  = .gitignore AUTHORS CHANGES COPYING INSTALL README TODO Makefile eu.lsm.in
SUBDIR  = doc
TARGET  = $(EFILES)


#
# Compiler flags
#
CFLAGS  = -Wall -Wstrict-prototypes -O2 -pedantic -DUSE_RCS
#CFLAGS  = -Wall -Wstrict-prototypes -O2 -pedantic -D_REENTRANT -DDEBUG


#
# Library flags
#
LDFLAGS   =
LIBS      =


#
# Prg
#
CC      = gcc
CP	= cp -f
DATE    = date
INSTALL = install
LD      = gcc
MD5SUM  = md5sum
MV      = mv -f
RM      = rm -f
SED     = sed
STRIP   = strip --strip-unneeded
TAR     = tar



#
# Object rules
#
.c.o:
	$(CC) $(CFLAGS) -c $<


#
# Default rule
#
all: .depend whenmake.c $(TARGET)

.SILENT: .depend whenmake.c



#
# Strip
#
strip: $(EFILES)
	$(STRIP) $^



#
# Print
#
print.ps:
	a2ps -MA4 --column=1 -toc *.[ch] -o print.ps

print.pdf: print.ps
	ps2pdf13 -sPAPERSIZE=a4 $^

print: print.pdf


size:
	@wc *.[ch]



#
# Install
#
install: installbin installman

installbin: all
	$(INSTALL) -m  555 -s -o bin  -g bin   eu	 	$(BIN_DIR)

installdir:
	$(INSTALL) -m  755    -o root -g root  -d		$(ETC_DIR)
	$(INSTALL) -m  777    -o root -g root  -d		$(VAR_DIR)

installconf: installdir
	$(INSTALL) -m  640    -o root -g root  etc/*.conf	$(ETC_DIR)

installman:
	$(INSTALL) -m  644    -o bin  -g bin   eu.1		$(MAN_DIR)/man1


#
# Depend 
#
.depend: Makefile
	echo "Verify source's dependences..."
	$(CC) $(CFLAGS) -M $(CFILES) >.depend

depend:
	$(CC) $(CFLAGS) -M $(CFILES) >.depend
	@echo "The following error is _normal_ please redo your \"make\""
	@echo " "
	@exit 1


#
# Cleaning
#
clean:
	$(RM) *.o *~ .*~ core print.ps

distclean: clean
	$(RM) $(EFILES) print.pdf last_change

whenmakeclean:
	$(RM) whenmake.c

clobber: distclean 
	$(RM) .depend *.orig $(PACKAGE).tgz


#
# Package save
#
./TAR:
	$(INSTALL) -m 750 -o root -g root -d ./TAR

$(PACKAGE).tgz: $(RFILES) $(CFILES) $(HFILES) $(MFILES) $(SUBDIR)
	$(TAR) czf $(PACKAGE).tgz $(RFILES) $(CFILES) $(HFILES) $(MFILES) $(SUBDIR)

$(PACKAGE).lsm: $(PACKAGE).tgz
	$(SED) -e s/SIZE/$(shell expr `wc -c $(PACKAGE).tgz | cut -d\  -f1` / 1024)/ -e s/VERSION/$(VERSION)/g -e s/DATE/$(shell date +%Y%m%d)/ -e s/PACKAGE/$(PACKAGE)/g < $(PACKAGE).lsm.in > $(PACKAGE).lsm

tar: clobber ./TAR $(PACKAGE).lsm
	$(MV) $(PACKAGE).tgz $(PACKAGE)-$(VERSION).tgz
	$(CP) $(PACKAGE).lsm $(PACKAGE)-$(VERSION).lsm
	$(MD5SUM) $(PACKAGE)-$(VERSION).tgz > $(PACKAGE)-$(VERSION).tgz.md5
	$(MV) $(PACKAGE)-$(VERSION).* ./TAR



#
# RCS & TAGS
#
./rcsnote:
	@echo
	@echo "Insert version note on ./rcsnote"
	@echo
	@exit 1

./RCS:
	$(INSTALL) -m 750    -o root -g root -d                 ./RCS

ci: ./RCS ./rcsnote
	ci -l -q -m"`cat ./rcsnote`" -w$(AUTHOR) $(RFILES) $(CFILES) $(HFILES) $(MFILES)
	$(RM) ./rcsnote

tags: $(CFILES)
	ctags $(CFILES)



#
# Process 
#
restart: installbin
	@echo
	@echo Before restart:
	@(ps -ax|grep log|grep -v grep)
	@(for i in $(DFILES); do kill `ps -ax|grep $$i|grep -v grep|cut -b 1-5`; $(SBIN_DIR)/$$i; done)
	@echo
	@echo After restart:
	@(ps -ax|grep log|grep -v grep)


#
# Executables
#
eu: eu.o query.o unique.o whenmake.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "*** $@ is done!"
	@echo



#
# when make
#
whenmake: whenmakeclean whenmake.c

whenmake.c: Makefile
	echo Create $@
	echo "char package[]  = \"$(PACKAGE)\";" > $@
	echo "char version[]  = \"$(VERSION)\";" >> $@
	echo "char software[] = \"$(PACKAGE) $(VERSION) by $(AUTHOR)\";" >> $@
	echo "char compiled[] = \"`$(DATE)`\";" >> $@
	echo "char etc_path[] = \"$(ETC_DIR)\";" >> $@
	echo "char var_path[] = \"$(VAR_DIR)\";" >> $@


# Generate the file "last_change" which contains the date of change
# of the most recently modified source code file
#
last_change: $(CFILES) $(HFILES)
	grep '$$Id' $(CFILES) $(HFILES) | sort +4 | tail -1 | awk -F\$$ '{print $$2}' | awk '{print $$4,$$5}' > last_change


#
# Objects
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif

#
# End
