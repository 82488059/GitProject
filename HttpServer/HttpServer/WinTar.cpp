#include "stdafx.h"
#include "WinTar.h"

#include <Python.h>

CWinTar::CWinTar()
{
}


CWinTar::~CWinTar()
{
}

bool CWinTar::UnPackTar(const CString& szFileName, const CString& szPath)
{
    // TODO:  在此添加控件通知处理程序代码
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        return false;
    }
    //     PyObject * pModule = NULL;
    //     PyObject * pFunc = NULL;
    //     PyObject* pName = NULL;

    int r = PyRun_SimpleString("import sys");
    r = PyRun_SimpleString("sys.path.append('./recv/')");

    PyObject *pName = PyString_FromString("MyTar");
    PyObject * pModule = PyImport_Import(pName);
    if (!pModule)
    {
        return false;
    }
    PyObject* pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        return false;
    }
    PyObject* pFunc = PyDict_GetItemString(pDict, "utar");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        return false;
    }
    PyObject* pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", szFileName));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", szFileName.Left (szFileName.GetLength ()-4)));

    PyObject* pReturn = NULL;

    pReturn = PyEval_CallObject(pFunc, pArgs);

    int result;
    PyArg_Parse(pReturn, "i", &result);

    Py_Finalize();

    return true;
}
bool CWinTar::PackTar(const CString & szFileName)
{
    // TODO:  在此添加控件通知处理程序代码
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        return false;
    }
    //     PyObject * pModule = NULL;
    //     PyObject * pFunc = NULL;
    //     PyObject* pName = NULL;

    int r = PyRun_SimpleString("import sys");
    r = PyRun_SimpleString("sys.path.append('./recv/')");

    PyObject *pName = PyString_FromString("MyTar");
    PyObject * pModule = PyImport_Import(pName);
    if (!pModule)
    {
        return false;
    }
    PyObject* pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        return false;
    }
    PyObject* pFunc = PyDict_GetItemString(pDict, "tar_one");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        return false;
    }
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", szFileName));

    PyObject* pReturn = NULL;

    pReturn = PyEval_CallObject(pFunc, pArgs);

    int result;
    PyArg_Parse(pReturn, "i", &result);

    Py_Finalize();

    return true;
}
