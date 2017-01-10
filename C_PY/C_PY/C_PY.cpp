// C_PY.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>

#include <Python.h>

//C/C++��ִ��python�ļ�
int exec_python_file()
{
    // ��ʼ��Python
    //��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
    //���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
    //����ģ������·���С��������û�з���ֵ�����ϵͳ
    //�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��

    Py_Initialize();

    // ����ʼ���Ƿ�ɹ�
    if (!Py_IsInitialized())
    {
        return -1;
    }

    // ��ӵ�ǰ·��
    //��������ַ�����ΪPython����ֱ�����У�����0
    //��ʾ�ɹ���-1��ʾ�д����ʱ���������Ϊ�ַ���
    //�����﷨����
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject *pName = NULL, *pModule = NULL, *pDict = NULL, *pFunc = NULL, *pArgs = NULL;

    // ������Ϊpytest�Ľű�
    pName = PyString_FromString("WeatherForecast");
    pModule = PyImport_Import(pName);
    if (!pModule)
    {
        printf("can't find WeatherForecast.py");
        getchar();
        return -1;
    }
    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        return -1;
    }

    // �ҳ�������Ϊadd�ĺ���
    pFunc = PyDict_GetItemString(pDict, "WeatherForecast");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        printf("can't find function [GetJson1]");
        getchar();
        return -1;
    }

    // ������ջ
//     pArgs = PyTuple_New(2);

    //  PyObject* Py_BuildValue(char *format, ...)
    //  ��C++�ı���ת����һ��Python���󡣵���Ҫ��
    //  C++���ݱ�����Pythonʱ���ͻ�ʹ������������˺���
    //  �е�����C��printf������ʽ��ͬ�����õĸ�ʽ��
    //  s ��ʾ�ַ�����
    //  i ��ʾ���ͱ�����
    //  f ��ʾ��������
    //  O ��ʾһ��Python����

//     PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
//     PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));

    // ����Python����
//     pArgs = PyTuple_New(1);
//     PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 2)); //

    PyObject* pRetVal = PyObject_CallObject(pFunc, pArgs);
    if (pRetVal == NULL)
    {
        return -1;
    }
    printf("function return value:%s\r\n", PyString_AsString(pRetVal));

    Py_DECREF(pName);
    if (pArgs)
    {
        Py_DECREF(pArgs);
    }
    Py_DECREF(pModule);

    // �ر�Python
    Py_Finalize();
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::cout << "C/C++ call python function:" << std::endl;
    exec_python_file();
    system("pause");
    return 0;
}