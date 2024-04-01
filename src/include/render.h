#include <stdlib.h>
#include <SDL2/SDL.h>
#ifndef Py_POG_RENDER_H
#define Py_POG_RENDER_H

typedef struct {
	PyObject_HEAD
	SDL_Rect rect;
} PogRect;

static int PogRect_init(PogRect *self, PyObject *args);
static void PogRect_dealloc(PogRect* self);
static PyObject* PogRect_getbounds(PogRect *self, PyObject *args);
static PyObject* PogRect_getpos(PogRect *self, PyObject *args);
static PyObject* PogRect_update(PogRect *self, PyObject *args);

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
static PyMethodDef PogRect_methods[] = {
	{ "getbounds", PogRect_getbounds, METH_NOARGS,
		"Get bounds of rectangle"},
	{ "getpos", PogRect_getpos, METH_NOARGS,
		"Get position of rectangle"},
	{ "update", PogRect_update, METH_VARARGS,
		"Update the position of rectangle"},
	{NULL}
};

static PyTypeObject PogRectType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "pog.Rect",
	.tp_doc = PyDoc_STR("Rectangle"),
	.tp_basicsize = sizeof(PogRect),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_new = PyType_GenericNew,
	.tp_init = (initproc) PogRect_init,
	.tp_dealloc = (destructor) PogRect_dealloc,
	.tp_methods = PogRect_methods
};

static PyObject* draw_rectangle(PyObject *self, PyObject *args);

#endif // Py_POG_RENDER_H
