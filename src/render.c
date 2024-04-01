#include <Python.h>
#include "include/pog.h"

static PyObject*
draw_rectangle(PyObject *self, PyObject *args)
{
	int x, y; // position
	int r, g, b; // color
	int size_x, size_y; // size
	if(!PyArg_ParseTuple(args, "(ii)(iii)(ii)", &x, &y, &r, &g, &b, &size_x, &size_y))
		return NULL;
	SDL_Rect rect = (SDL_Rect){ .x = x, .y = y, .w = size_x, .h = size_y };

	SDL_SetRenderDrawColor(context->renderer, r, g, b, 255);
	SDL_RenderFillRect(context->renderer, &rect);
	Py_RETURN_NONE;
}
