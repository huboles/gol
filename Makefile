NAME = gol
VER ?= 0.2.2

SHELL = /bin/bash

DESTDIR ?=
ETCDIR ?= $(DESTDIR)/etc/$(NAME)
USRDIR ?= $(DESTDIR)/usr

PREFIX ?= $(USRDIR)
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
SHAREDIR ?= $(PREFIX)/share
INCLUDE ?= $(PREFIX)/include
MANDIR ?= $(SHAREDIR)/man
MAN1DIR ?= $(MANDIR)/man1
DOCDIR ?= $(PREFIX)/doc/$(NAME)

SRCDIR ?= ./src
OBJDIR ?= ./obj
BUILD ?= ./build

BIN ?= $(BUILD)/$(NAME)
SRC ?= $(wildcard $(SRCDIR)/*.c)
HDR ?= $(wildcard $(SRCDIR)/*.h)
OBJ ?= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
LIBNAME ?= lib$(NAME)
LIB ?= $(BUILD)/$(LIBNAME).so
STATIC ?= $(BUILD)/$(LIBNAME).a


TAR ?= $(BUILD)/$(NAME)-$(VER).tar.gz

MAN ?= $(SRCDIR)/$(NAME).1
MANPAGE ?= $(patsubst $(SRCDIR)/%.1,$(BUILD)/%.1.gz,$(MAN))
DOC ?= README LICENSE

ZIP = gzip
RM = rm -f

CFLAGS += -O2 -pipe
CPPFLAGS += -I$(SRCDIR) -I$(BUILD)
LDFLAGS +=
LDLIBS +=
WARNINGS ?= -Wall -Wextra -Wpedantic

FLAGS += $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $(LDFLAGS) $(LDLIBS)
DEBUGFLAGS += $(WARNINGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -ggdb -Og -pipe
RELEASEFLAGS = $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -O3 -pipe -DNDEBUG -DDONT_USE_VOL

all: $(BIN)

lib: $(LIB)

static: $(STATIC)

release: | $(BUILD)
	$(MAKE) clean
	$(CC) $(SRC) $(RELEASEFLAGS) -o $(BIN)-$(VER)
	$(MAKE) tar

install: $(BIN) $(MANPAGE) $(DOC)
	-mkdir -p $(BINDIR)
	cp $(BIN) $(BINDIR)
	chmod 755 $(BINDIR)/$(patsubst $(BUILD)/%,%,$(BIN))
	-mkdir -p $(MANDIR)
	cp $(MANPAGE) $(MANDIR)
	chmod 644 $(MANDIR)/$(patsubst $(BUILD)/%,%,$(MANPAGE))
	-mkdir -p $(DOCDIR)
	install -CDm 644 -t $(DOCDIR) $(DOC)

install_lib:
	-mkdir -p $(LIBDIR)
	cp $(LIB) $(LIBDIR)
	chmod 755 $(LIBDIR)/$(patsubst $(BUILD)/%,%,$(LIB))
	-mkdir -p $(INCLUDE)
	cp $(HDR) $(INCLUDE)
	chmod 644 $(INCLUDE)/$(patsubst $(SRCDIR)/%,%,$(HDR))

install_static:
	-mkdir -p $(LIBDIR)
	cp $(STATIC) $(LIBDIR)
	chmod 755 $(LIBDIR)/$(patsubst $(BUILD)/%,%,$(STATIC))
	-mkdir -p $(INCLUDE)
	cp $(HDR) $(INCLUDE)
	chmod 644 $(INCLUDE)/$(patsubst $(SRCDIR)/%,%,$(HDR))

uninstall:
	$(RM) -r $(BINDIR)/$(patsubst $(BUILD)/%,%,$(BIN))
	$(RM) -r $(MANDIR)/$(patsubst $(BUILD)/%,%,$(MANPAGE))
	$(RM) -r $(INCLUDE)/$(patsubst $(SRC)/%,%,$(HDR))
	$(RM) -r $(LIBDIR)/$(patsubst $(BUILD)/%,%,$(LIB))
	$(RM) -r $(LIBDIR)/$(patsubst $(BUILD)/%,%,$(STATIC))
	$(RM) -r $(DOCDIR)

tar: $(SRC) $(HDR) $(MAN) $(DOC) | $(BUILD)
	tar -I $(ZIP) -cvf $(TAR) $(SRC) $(HDR) $(MAN) $(DOC)

$(BIN): $(OBJ) | $(BUILD)
	$(CC) $(OBJ) $(FLAGS) -o $(BIN)

$(LIB): $(SRC) $(HDR) | $(BUILD)
	$(CC) $(SRC) $(FLAGS) -fPIC -shared -lc -o $(LIB)

$(STATIC): $(OBJ) | $(BUILD)
	$(AR) rcs $(STATIC) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDR) | $(OBJDIR)
	$(CC) -c $(FLAGS) $< -o $@

clean:
	$(RM) $(BUILD)/* $(OBJDIR)/*

debug: | $(BUILD)
	$(MAKE) clean
	$(CC) $(SRC) $(DEBUGFLAGS) -o $(BIN)-debug

$(MANPAGE): $(MAN) | $(BUILD)
	$(ZIP) -c $(MAN) > $(MANPAGE)

$(SRCDIR) $(OBJDIR) $(BUILD):
	mkdir $@

.PHONY: all release lib install_lib static install_static tar clean install uninstall debug
