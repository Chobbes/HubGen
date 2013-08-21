# Copyright (C) 2013 Calvin Beck

# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# You may need to change the includes to suit your needs.
MUXPARSE_DIR = ~/repos/work/muxparse/src/
MUXDUINO_DIR = ~/arduino-ua/libraries-osx/muxduino/

CFLAGS += -I $(MUXDUINO_DIR) -I $(MUXPARSE_DIR)
LDFLAGS += -L $(MUXPARSE_DIR) -lmuxparse

hubgen : hubgen.o pipes.o
	$(CC) $^ $(LDFLAGS) -o $@

hubgen.o : hubgen.c pipes.h
	$(CC) -c $< $(CFLAGS)

pipes.o : pipes.c pipes.h
	$(CC) -c $< $(CFLAGS)

%.o : %.c
	$(CC) -c $< $(CFLAGS)

clean:
	$(RM) hubgen
	$(RM) hubgen.o
	$(RM) pipes.o

.PHONY: clean
