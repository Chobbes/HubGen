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

#include "pipes.h"

#include <stdio.h>
#include <stdlib.h>
#include <mux_pipe.h>
#include <muxparse.h>


/*
  Size of the pipes array, and the amount that we will increase the
  pipe array size by if there are too many pipes to fit in the
  array. This number is for the number of pipes in the array.
*/
static const int PIPE_CHUNK_SIZE = 256;


/*
  Arguments:
      pipe: The pipe we want to check.

      pipes: All of the previously parsed pipes.

      total_pipes: Number of pipes in the pipes array.

  Check if the pipe is valid - basically just checks for inputs being
  declared as outputs or vice versa. Prints warnings to stderr if
  there are problems with the pipe.
*/
static void pipe_warnings(MuxPipe pipe, MuxPipe *pipes, size_t total_pipes)
{
    if (pipe.out_pin == pipe.in_pin) {
	fprintf(stderr, "Can't have a pipe with the same input and output:");
	fprintf(stderr, " %d\n", pipe.out_pin);

	return;
    }

    for (int index = 0; index < total_pipes; ++index) {
	MuxPipe array_pipe = pipes[index];

	if (pipe.out_pin == array_pipe.in_pin) {
	    fprintf(stderr, "Output %d ", pipe.out_pin);
	    fprintf(stderr, "is an input for %d!\n", array_pipe.out_pin);

	    return;
	}
	else if (pipe.in_pin == array_pipe.out_pin) {
	    fprintf(stderr, "Input %d ", pipe.in_pin);
	    fprintf(stderr, "is an output for %d!\n", array_pipe.in_pin);

	    return;
	}
    }
}


MuxPipe * load_pipes(FILE *input_file, size_t *total_pipes)
{
    size_t pipe_count = 0;  /* Number of pipes in the file */
    size_t chunk_count = 1; /* We start with one pipe chunk sized array */
    MuxPipe *pipes = malloc(PIPE_CHUNK_SIZE * sizeof(MuxPipe));

    MuxPipe parsed_pipe;
    int parse_result = mux_parse_pipe(input_file, &parsed_pipe);

    while (0 == parse_result) {
	if (pipe_count + 1 > chunk_count * PIPE_CHUNK_SIZE) {
	    /* We are on the edge of a pipe chunk - need more pipes" */
	    ++chunk_count;
	    pipes = realloc(pipes, chunk_count * PIPE_CHUNK_SIZE);

	    if (NULL == pipes) {
		fprintf(stderr, "Memory allocation failure: ");
		fprintf(stderr, "Could not grow pipes array!\n");

		return NULL;
	    }
	}

	/* Print any necessary warnings */
	pipe_warnings(parsed_pipe, pipes, pipe_count);

	/* Add pipe to pipes array */
	pipes[pipe_count] = parsed_pipe;
	++pipe_count;

	/* Fetch the next pipe */
	parse_result = mux_parse_pipe(input_file, &parsed_pipe);
    }

    if (2 == parse_result) {
	return NULL;
    }

    *total_pipes = pipe_count;
    return pipes;
}
