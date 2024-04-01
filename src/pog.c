#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "include/pog.h"

// source files
#include "render.c"

#define pog_doc "A simple 2D library"
#define DO_POG_INIT(ret) if (!initialized) { \
	PyErr_SetString(PogInitError, "Pog not initialized!"); \
	return ret; \
}

static void
PogContext_call_handler(PogContext *self, const char* event) {
	PyObject *handler = PyDict_GetItemString(self->handlers, event);
	if (handler)
		PyObject_CallNoArgs(handler);
}

static PyObject*
PogContext_run(PogContext *self, PyObject *args) {
	PogContext_call_handler(self, "preloop");
	while (true) {
		SDL_PollEvent(&self->ev);
		if (self->ev.type == SDL_QUIT) {
			break;
		}

		SDL_RenderClear(self->renderer);
		PogContext_call_handler(self, "render");
		SDL_SetRenderDrawColor(self->renderer, 50, 50, 50, 255);
		SDL_RenderPresent(self->renderer);
	}
	PogContext_call_handler(self, "postloop");
	Py_RETURN_NONE;
}

static PyObject*
PogContext_print_handlers(PogContext *self, PyObject *args) {
	PyObject_Print(self->handlers, stdout, Py_PRINT_RAW);
	Py_RETURN_NONE;
}

static PyObject*
PogContext_add_handler(PogContext *self, PyObject *args) {
	DO_POG_INIT(NULL);

	char* event_type;
	PyObject *handler;
	
	if (!PyArg_ParseTuple(args, "sO", &event_type, &handler))
		return NULL;
	
	SDL_Log("Adding handler for %s\n", event_type);

	if (!PyCallable_Check(handler)) {
		PyErr_SetString(PyExc_TypeError, "Parameter two must be callable");
		return NULL;
	}
	SDL_Log("Handler is callable!\n");

	if (0 > PyDict_SetItemString(self->handlers, event_type, handler)) {
		PyErr_SetString(PyExc_Exception, "Unable to add handler");
		return NULL;
	}

	SDL_Log("Added handler for %s\n", event_type);
	Py_RETURN_NONE;
}

static int
PogContext_init(PogContext *self, PyObject *args, PyObject *kwds) {

	DO_POG_INIT(-1)

	static char* kwlist[] = { "title", "width", "height", NULL };
	char* title;
	int width, height;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "sii", kwlist,
		&title, &width, &height))
		return -1;
	
	self->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_SHOWN);

	if (self->window == NULL) {
		PyErr_SetString(PogInitError, SDL_GetError());
		return -1;
	} else {
		SDL_Log("Window [%s] [%dx%d] initialized properly\n", title, width, height);
	}
	self->renderer = SDL_CreateRenderer(self->window, -1, 0);
	self->handlers = PyDict_New();

	context = self;
	return 0;
}

static void
PogContext_dealloc(PogContext* self) {
	SDL_DestroyRenderer(self->renderer);
	SDL_DestroyWindow(self->window);
	Py_TYPE(self->handlers)->tp_free(self->handlers);
	Py_TYPE(self)->tp_free((PyObject*) self);
}

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
static PyMethodDef PogContext_methods[] = {
	{"run", PogContext_run, METH_NOARGS,
	"Run the main loop"},
	{"add_handler", PogContext_add_handler, METH_VARARGS,
	"Add event handlers"},
	{"print_handlers", PogContext_print_handlers, METH_NOARGS,
	"Print all event handlers"},
	{NULL}
};

static PyTypeObject PogContextType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "pog.Context",
	.tp_doc = PyDoc_STR("window context"),
	.tp_basicsize = sizeof(PogContext),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_new = PyType_GenericNew,
	.tp_init = (initproc) PogContext_init,
	.tp_dealloc = (destructor) PogContext_dealloc,
	.tp_methods = PogContext_methods
};

static PyObject*
init(PyObject *self, PyObject *args) {
	initialized = true;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	SDL_Log("We compiled against SDL version %u.%u.%u ...\n",
		compiled.major, compiled.minor, compiled.patch);
	SDL_Log("But we are linking against SDL version %u.%u.%u.\n",
		linked.major, linked.minor, linked.patch);
	Py_RETURN_NONE;
}

static PyObject*
quit(PyObject *self, PyObject *args) {
	initialized = false;
	SDL_Quit();
	Py_RETURN_NONE;
}

static PyMethodDef PogMethods[] = {
	{ "init", init, METH_NOARGS, "Initialize"},
	{ "quit", quit, METH_NOARGS, "Quit"},
	{ "draw_rectangle", draw_rectangle, METH_VARARGS, "Draw a rectangle"},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef pogmodule = {
	PyModuleDef_HEAD_INIT,
	.m_name = "pog",
	.m_doc = pog_doc,
	.m_size = -1,
	.m_methods = PogMethods
};

PyMODINIT_FUNC
PyInit_pog (void)
{
	PyObject *m;

	m = PyModule_Create(&pogmodule);
	if (m == NULL)
		return NULL;
	
	PogInitError = PyErr_NewException("pog.PogInitError", NULL, NULL);
	Py_XINCREF(PogInitError);
	if (PyModule_AddObject(m, "error", PogInitError) < 0) {
		Py_XDECREF(PogInitError);
		Py_CLEAR(PogInitError);
		Py_DECREF(m);
		return NULL;
	}

	if (PyType_Ready(&PogContextType) < 0)
		return NULL;

	Py_INCREF(&PogContextType);
	if (PyModule_AddObject(m, "Context", (PyObject *) &PogContextType) < 0) {
		Py_DECREF(&PogContextType);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}


int
main (int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL)
	{
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}

    /* Add a built-in module, before Py_Initialize */
    if (PyImport_AppendInittab("pog", PyInit_pog) == -1)
	{
		fprintf(stderr,
				 "Error: could not extend in-built modules table\n");
		exit(1);
	}

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required.
               If this step fails, it will be a fatal error. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
               import can be deferred until the embedded script
                              imports it. */
    PyObject *pmodule = PyImport_ImportModule("pog");
    if (!pmodule)
	{
		PyErr_Print();
		fprintf(stderr, "Error: could not import module 'pog'\n");
	}

	PyMem_RawFree(program);
    return 0;
}
