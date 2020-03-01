include config.mk

SRC = src/library.cpp src/lisp.cpp src/main.cpp src/reader.cpp
OBJ = $(SRC:src/%.cpp=%.o)

all: options solisp

options:
	@echo Solisp build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.cpp.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

solisp: ${OBJ}
	@cd src
	@echo LD $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f solisp ${OBJ} ${LIBOBJ} solisp-${VERSION}.tar.gz

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f solisp ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/solisp
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < docs/solisp.1 > ${DESTDIR}${MANPREFIX}/man1/solisp.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/solisp.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/solisp
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/solisp.1

.PHONY: all options clean dist install uninstall