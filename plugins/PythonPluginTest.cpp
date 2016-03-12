// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "PythonPlugin.h"

#include <iostream>
#include <memory>
#include <string>

using namespace sigblocks;
using namespace std;

void test_function_call(PythonPlugin* pPyplugin);

void test_inline_source(PythonPlugin* pPyplugin);

void test_numpy(PythonPlugin* pPyplugin);

int main() {
    PythonPlugin pyplugin;
    // run multiple times to test mem-leak
    // and other errors!
    for (int i = 0; i < 1000000; ++i) {
        //test_function_call(&pyplugin);
        //test_function_call(&pyplugin);

        //test_inline_source(&pyplugin);
        //test_inline_source(&pyplugin);

        test_numpy(&pyplugin);
        //test_numpy(&pyplugin);
    }
}

void test_function_call(PythonPlugin* pPyplugin) {
    std::unique_ptr <PythonBaseResult> result;

    try {
        std::cout << "Running test_file.test_function : output {";
        result = pPyplugin->RunPythonCode("./", "test_file", "test_function");
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() <<
            //", result->mValue = " <<
            //dynamic_cast<PyStringResult*>(result.get())->mValue <<
            std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    }
    catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        std::cout << "Running py_function.multiply : output {";
        result = pPyplugin->RunPythonCode("./", "py_function", "multiply");
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() <<
            //dynamic_cast<PyLongResult*>(result.get())->mValue <<
            std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    }
    catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }
}

void test_inline_source(PythonPlugin* pPyplugin) {
    try {
        std::string source("print 'Hello World!'");
        std::unique_ptr <PythonRunnableCode> runnable(
                pPyplugin->ParsePythonSource(source));
        if (runnable.get() == 0) {
            return; // XXX error!
        }

        std::cout << "Running inline source [" << source << "] : output {";
        std::unique_ptr <PythonBaseResult> result(
                pPyplugin->RunPythonRunnableCode(runnable.get()));
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() << std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    }
    catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }
}

void test_numpy(PythonPlugin* pPyplugin) {
    std::unique_ptr <PythonBaseResult> result;

    try {
        std::cout << "Running numpy_test.myrange : output {";
        result = pPyplugin->RunPythonCode("./", "numpy_test", "myrange");
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() << std::endl;
            //(result.get())->operator<<(std::cout) << std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    }
    catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }
}
