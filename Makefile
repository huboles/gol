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

BIN ?= $(BUILD)/$(NAME)
SRC ?= $(wildcard $(SRCDIR)/*.c) 
HDR ?= $(wildcard $(SRCDIR)/*.h)
OBJ ?= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

TAR ?= $(BUILD)/$(NAME)-$(VER).tar.gz

MAN ?= $(SRCDIR)/$(NAME).1
MANPAGE ?= $(patsubst $(SRCDIR)/%.1,$(BUILD)/%.1.gz,$(MAN))
DOC ?= README LICENSE

CC ?= gcc
ZIP ?= gzip
CFLAGS += -O2 -pipe
WARNINGS ?= -Wall -Wextra -Wpedantic
CPPFLAGS += -I $(SRCDIR)
LDFLAGS += -L $(SRCDIR)
LDLIBS +=
FLAGS ?= $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(LDFLAGS) $(LDLIBS)

all: $(BIN)

release: CFLAGS = -O3 -pipe -DNDEBUG -DDONT_USE_VOL
release: $(BIN)

install: $(BIN) $(MANPAGE) $(DOC)
	-mkdir -p $(BINDIR)
	cp $(BIN) $(BINDIR)
	chmod 755 $(BINDIR)/$(patsubst $(BUILD)/%,%,$(BIN))
	-mkdir -p $(MANDIR)
	cp $(MANPAGE) $(MANDIR)
	chmod 644 $(MANDIR)/$(patsubst $(BUILD)/%,%,$(MANPAGE))
	-mkdir -p $(DOCDIR)
	install -CDm 644 -t $(DOCDIR) $(DOC) 

uninstall:
	-rm -rf $(BINDIR)/$(BIN) $(MANDIR)/$(MANPAGE) $(DOCDIR)

tar: $(SRC) $(HDR) $(MAN) $(DOC)
	tar -I $(ZIP) -cvf $(TAR) $(SRC) $(HDR) $(MAN) $(DOC)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDR)  
	$(CC) -c $(FLAGS) $< -o $@

clean:
	-rm -f $(BUILD)/* $(OBJDIR)/*

debug: CFLAGS += -ggdb3 -Og
debug: $(BIN)

$(MANPAGE): $(MAN)
	$(ZIP) -c $(MAN) > $(MANPAGE)

$(SRCDIR) $(OBJDIR) $(BUILDIR):
	mkdir $@

.PHONY: all release clean install tar uninstall debug test
