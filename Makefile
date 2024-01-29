CC = gcc
CFLAGS = -Wextra -s -Wno-cast-function-type -DOFFSET=100 -DEND=200 -fstack-protector -pie -ftrapv
LDFLAGS = -lssl -lcrypto -pthread -ldl

TARGET = main
SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/main.c $(SRCDIR)/infection.c $(SRCDIR)/utils.c $(SRCDIR)/selfPurge.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
DEPS = $(wildcard $(SRCDIR)/*.h)

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
