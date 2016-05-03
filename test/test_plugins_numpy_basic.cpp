// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#include "../common/logging.h"
#include "../plugins/PythonPlugin.h"

#include "catch.hpp"

#include <cstdio>
#include <fstream>

using namespace signalblocks;

// This test case is not testing anything, while generating all the test python
// code so that the rest of the test cases can use them.
// It appears that the python interpreter (once loaded) do not have
// a clear view when new python modules are added dynamically.
// This results in failure of test cases and it reports that the import
// for the said module (file) failed although the file was dynamically
// generated within that test case.
//
// This is a workaround of the issue where all the python code is
// made available at the start so that once the python interpreter
// is loaded it has the complete visibility and can run the test
// modules correctly. Note that the first instance of PythonPlugin
// will trigger initialization of the python interpreter hence all
// the python test modules must be available before that.
//
TEST_CASE("Testing plugins while generating python modules", "[plugin numpy basic]") {
    {
        // generate a temporary python file/module for testing
        std::string numpy_code = "import numpy as np\n"
                "\n"
                "def myrange():\n"
                "    x = np.arange(0, 10, np.float(1.0)).reshape(5,2)\n"
                "    return x\n"
                "    #return np.bool_(0)\n"
                "    c = np.longdouble(10)\n"
                "    print(type(c))\n"
                "    return c";

        std::string tmp_name("tmp_11232_plugin_test");
        std::string tmp_filename(tmp_name);
        tmp_filename += std::string(".py");
        std::string tmp_filepath("./");
        std::string tmp_fullfilename = tmp_filepath;
        tmp_fullfilename += tmp_filename;
        {
            std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
            ofs << numpy_code;
            ofs.close();
        }
    }

    {
        // generate a temporary python file/module for testing
        std::string numpy_code = "''' test '''\n"
                "def test_function():\n"
                "  print(\"Hello World!\")\n"
                "  return \"Hello World!\"";

        std::string tmp_name("tmp_211232_plugin_test");
        std::string tmp_filename(tmp_name);
        tmp_filename += std::string(".py");
        std::string tmp_filepath("./");
        std::string tmp_fullfilename = tmp_filepath;
        tmp_fullfilename += tmp_filename;
        {
            std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
            ofs << numpy_code;
            ofs.close();
        }
    }

    {
        // generate a temporary python file/module for testing
        std::string numpy_code = "''' Test Multiplication '''\n"
                "\n"
                "def multiply():\n"
                "    c = 12345*6789\n"
                "    print('The result of 12345 x 6789 :', c)\n"
                "    return c";

        std::string tmp_name("tmp_2321232_plugin_test");
        std::string tmp_filename(tmp_name);
        tmp_filename += std::string(".py");
        std::string tmp_filepath("./");
        std::string tmp_fullfilename = tmp_filepath;
        tmp_fullfilename += tmp_filename;
        {
            std::ofstream ofs(tmp_fullfilename.c_str(), std::ofstream::out);
            ofs << numpy_code;
            ofs.close();
        }
        LOG_DEBUG("Generated file=%s\n", tmp_fullfilename.c_str());
    }
}

TEST_CASE("Testing plugins numpy basic inline test", "[plugin numpy basic]") {
    PythonPlugin pyplugin;
    try {
        std::string source("print('Hello World!')");
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
    std::unique_ptr <PythonBaseResult> result;

    PythonPlugin pyplugin;
    try {
        LOG_INFO("Running numpy_test.myrange : output {");
        result = pyplugin.RunPythonCode("./", "tmp_11232_plugin_test", "myrange");
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
    REQUIRE(remove("./tmp_11232_plugin_test.py") == 0);
}

TEST_CASE("Testing plugins numpy basic hello world test", "[plugin numpy basic]") {
    std::unique_ptr <PythonBaseResult> result;

    PythonPlugin pyplugin;
    try {
        LOG_INFO("Running test_file.test_function : output {");
        result = pyplugin.RunPythonCode("./", "tmp_211232_plugin_test", "test_function");
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
    REQUIRE(remove("./tmp_211232_plugin_test.py") == 0);
}

TEST_CASE("Testing plugins numpy basic multiply test test", "[plugin numpy basic]") {
    std::unique_ptr <PythonBaseResult> result;

    PythonPlugin pyplugin;
    try {
        LOG_INFO("Running py_function.multiply : output {");
        result = pyplugin.RunPythonCode("./", "tmp_2321232_plugin_test", "multiply");
        LOG_INFO("} ");
    }
    catch (PyPluginTypeException& e) {
        LOG_ERROR("%s. Try rerunning again to allow reloading of module.\n", e.what());
        // lets rerun again (see below)
    }
    try {
        if (!result) {
            LOG_INFO("Running py_function.multiply : output {");
            result = pyplugin.RunPythonCode("./", "tmp_2321232_plugin_test", "multiply");
            LOG_INFO("} ");
        }
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

    REQUIRE(remove("./tmp_2321232_plugin_test.py") == 0);
}