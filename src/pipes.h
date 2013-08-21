/* Copyright (C) 2013 Calvin Beck

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#ifndef PIPES_H
#define PIPES_H

#include <stdio.h>
#include <stdlib.h>
#include <mux_pipe.h>


/*
  Arguments:
      input_file: File pointer for a ".mux" file.

      total_pipes: Location to store a size_t value for the number of
      pipes read.

  Returns a pointer to an array of MuxPipe structures containing all
  of the pipes from the file pointer in order.

  The file should probably have just been opened with fopen() prior to
  calling this function, and if all goes well this function will read
  to the end of the file. This function will return NULL on an error,
  and will print error messages to stderr.

  Note that you will have to free this memory later.

 */

MuxPipe * load_pipes(FILE *input_file, size_t *total_pipes);

#endif
