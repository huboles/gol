NAME = gol
VER ?= 0.2.2

SHELL = /bin/bash

DESTDIR ?= 

ETCDIR ?= $(DESTDIR)/etc/$(NAME)
USRDIR ?= $(DESTDIR)/usr
BINDIR ?= $(USRDIR)/bin
LIBDIR ?= $(USRDIR)/lib
MANDIR ?= $(USRDIR)/share/man/man1
DOCDIR ?= $(USRDIR)/share/doc/$(NAME)

SRCDIR ?= ./src
OBJDIR ?= ./obj
BUILD ?= ./build
TESTDIR ?= ./tests
TESTBIN ?= $(TESTDIR)/bin

BIN ?= $(BUILD)/$(NAME)
SRC ?= $(wildcard $(SRCDIR)/*.c) 
HDR ?= $(wildcard $(SRCDIR)/*.h)
OBJ ?= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
TEST ?= $(wildcard $(TESTDIR)/*.c)
TESTS ?= $(patsubst $(TESTDIR)/%.c, $(TESTBIN)/%, $(TEST))

TAR ?= $(BUILD)/$(NAME)-$(VERSION).tar.gz
TARSUM ?= $(BUILD)/$(NAME)-$(VERSION)-tar.hash
BINSUM ?= $(BUILD)/$(NAME)-$(VERSION).hash

MAN ?= $(SRCDIR)/$(NAME).1
MANPAGE ?= $(BUILD)/$(MAN).gz
DOC ?= README LICENSE

CC ?= gcc
AR ?= tar
HASH ?= sha256sum
CFLAGS += -O2 -pipe
WARNINGS ?= -Wall -Wextra -Wpedantic
CPPFLAGS += -I $(SRCDIR)
LDFLAGS += -L $(SRCDIR)
LDLIBS +=
FLAGS ?= $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(LDFLAGS) $(LDLIBS)

all: $(BIN)

release: CFLAGS = -O3 -pipe -DNDEBUG -DDONT_USE_VOL
release: clean
release: tar
release: $(BIN)
	$(HASH) $(BIN) > $(BINSUM)

install: $(BIN) $(MAN) $(DOC)
	-mkdir -p $(BINDIR)
	install -CDm 755 -t $(BINDIR) $(BIN)
	-mkdir -p $(MANDIR)
	install -CDm 644 -t $(MANDIR) $(MANPAGE) 
	-mkdir -p $(DOCDIR)
	install -CDm 644 -t $(DOCDIR) $(DOC) 

uninstall:
	-rm -rf $(BINDIR)/$(BIN) $(MANDIR)/$(MANPAGE) $(DOCDIR)

tar: $(SRC) $(HDR) $(MAN) $(DOC)
	-rm $(TAR) $(HASHSUM)
	$(AR) -c -f - $(SRC) $(HDR) $(MAN) $(DOC) | gzip > $(TAR)
	$(HASH) $(TAR) > $(TARSUM)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDR)  
	$(CC) -c $(FLAGS) $< -o $@

clean:
	-rm -f $(BUILD)/* $(OBJDIR)/*

debug: CFLAGS += -ggdb3 -Og
debug: $(BIN)

$(TESTBIN)/%: $(TESTDIR)/%.c
	$(CC) $(FLAGS) $< $(filter-out $(OBJDIR)/$(NAME).o,$(OBJ)) -o $@

test: $(BIN) $(TESTBIN) $(TESTS)
	for test in $(TESTS); do ./$$test; done

$(SRCDIR) $(OBJDIR) $(BUILDIR) $(TESTDIR) $(TESTBIN):
	mkdir $@

.PHONY: all release clean install tar uninstall debug test
