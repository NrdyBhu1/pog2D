#include <Python.h>
#include "include/pog.h"

static int
PogRect_init(PogRect *self, PyObject *args) 
{
	int x, y;
	int w, h;
	if (!PyArg_ParseTuple(args, "(ii)(ii)", &x, &y, &w, &h))
		return -1;
	self->rect = (SDL_Rect) { 
		.x = x,
		.y = y,
		.w = w,
		.h = h
	};
	return 0;
}

static PyObject* PogRect_getbounds(PogRect *self, PyObject *args)
{
	return Py_BuildValue("(ii)", self->rect.w, self->rect.h);
}

static PyObject* PogRect_getpos(PogRect *self, PyObject *args)
{
	return Py_BuildValue("(ii)", self->rect.x, self->rect.y);
}

static PyObject* PogRect_update(PogRect *self, PyObject *args)
{
	int x, y;
	if (!PyArg_ParseTuple(args, "ii", &x, &y))
		return NULL;

	self->rect.x = x;
	self->rect.y = y;
	Py_RETURN_NONE;
}

static void PogRect_dealloc(PogRect* self) 
{
	Py_TYPE(self)->tp_free((PyObject*) self);
}

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
