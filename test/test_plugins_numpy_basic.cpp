// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/logging.h"
#include "../plugins/PythonPlugin.h"

#include "catch.hpp"

#include <cstdio>
#include <fstream>

using namespace sigblocks;

TEST_CASE("Testing plugins numpy basic inline test", "[plugin numpy basic]") {
    PythonPlugin pyplugin;
    try {
        std::string source("print 'Hello World!'");
        std::unique_ptr <PythonRunnableCode> runnable(
                pyplugin.ParsePythonSource(source));
        if (runnable.get() == 0) {
            return; // XXX error!
        }

        LOG_INFO("Running inline source [%s] : output {\n", source.c_str());
        std::unique_ptr <PythonBaseResult> result(
                pyplugin.RunPythonRunnableCode(runnable.get()));
        LOG_INFO("}\n");
        if (result.get() != 0) {
            LOG_INFO("%s\n", result->ToString().c_str());
        }
        else {
            LOG_INFO("No result returned!\n");
        }
    }
    catch (PyPluginTypeException& e) {
        LOG_ERROR("%s\n", e.what());
    }
}

TEST_CASE("Testing plugins numpy basic file test", "[plugin numpy basic]") {
    PythonPlugin pyplugin;

    // generate a temporary python file/module for testing
    std::string numpy_code = "import numpy as np\n"
            "\n"
            "def myrange():\n"
            "  x = np.arange(0, 10, np.float(1.0)).reshape(5,2)\n"
            "  return x\n"
            "  #return np.bool_(0)\n"
            "  c = np.longdouble(10)\n"
            "  print type(c)\n"
            "  return c";

    std::string tmp_name("tmp_11232_plugin_test");
    std::string tmp_filename(tmp_name);
    tmp_filename += std::string(".py");
    std::string tmp_filepath("./");
    std::string tmp_fullfilename = tmp_filepath;
    tmp_fullfilename += tmp_filename;
    std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
    ofs << numpy_code;
    ofs.close();

    std::unique_ptr <PythonBaseResult> result;

    try {
        LOG_INFO("Running numpy_test.myrange : output {");
        result = pyplugin.RunPythonCode(tmp_filepath, tmp_name, "myrange");
        LOG_INFO("} ");
        if (result.get() != 0) {
            LOG_INFO("%s\n", result->ToString().c_str());
            //(result.get())->operator<<(std::cout) << std::endl;
        }
        else {
            LOG_INFO("No result returned!\n");
        }
    }
    catch (PyPluginTypeException& e) {
        LOG_ERROR("%s\n", e.what());
    }

    // cleanup the test file created
    REQUIRE(remove(tmp_fullfilename.c_str()) == 0);
}

TEST_CASE("Testing plugins numpy basic hello world test", "[plugin numpy basic]") {
    PythonPlugin pyplugin;

    // generate a temporary python file/module for testing
    std::string numpy_code = "''' test '''\n"
            "def test_function():\n"
            "  print \"Hello World!\"\n"
            "  return \"Hello World!\"";

    std::string tmp_name("tmp_211232_plugin_test");
    std::string tmp_filename(tmp_name);
    tmp_filename += std::string(".py");
    std::string tmp_filepath("./");
    std::string tmp_fullfilename = tmp_filepath;
    tmp_fullfilename += tmp_filename;
    std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
    ofs << numpy_code;
    ofs.close();

    std::unique_ptr <PythonBaseResult> result;

    try {
        LOG_INFO("Running test_file.test_function : output {");
        result = pyplugin.RunPythonCode(tmp_filepath, tmp_name, "test_function");
        LOG_INFO("} ");
        if (result.get() != 0) {
            LOG_INFO("%s\n", result->ToString().c_str());
        }
        else {
            LOG_INFO("No result returned!\n");
        }
    }
    catch (PyPluginTypeException& e) {
        LOG_ERROR("%s\n", e.what());
    }

    // cleanup the test file created
    REQUIRE(remove(tmp_fullfilename.c_str()) == 0);
}

TEST_CASE("Testing plugins numpy basic multiply test test", "[plugin numpy basic]") {
    PythonPlugin pyplugin;

    // generate a temporary python file/module for testing
    std::string numpy_code = "''' Test Multiplication '''\n"
            "\n"
            "def multiply():\n"
            "    c = 12345*6789\n"
            "    print 'The result of 12345 x 6789 :', c\n"
            "    return c";

    std::string tmp_name("tmp_2321232_plugin_test");
    std::string tmp_filename(tmp_name);
    tmp_filename += std::string(".py");
    std::string tmp_filepath("./");
    std::string tmp_fullfilename = tmp_filepath;
    tmp_fullfilename += tmp_filename;
    std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
    ofs << numpy_code;
    ofs.close();

    std::unique_ptr <PythonBaseResult> result;

    try {
        LOG_INFO("Running py_function.multiply : output {");
        result = pyplugin.RunPythonCode(tmp_filepath, tmp_name, "multiply");
        LOG_INFO("} ");
        if (result.get() != 0) {
            LOG_INFO("%s\n", result->ToString().c_str());
        }
        else {
            LOG_INFO("No result returned!\n");
        }
    }
    catch (PyPluginTypeException& e) {
        LOG_ERROR("%s\n", e.what());
    }

    // cleanup the test file created
    REQUIRE(remove(tmp_fullfilename.c_str()) == 0);
}