/**********************************************************************************/
/* This file is part of spla project                                              */
/* https://github.com/JetBrains-Research/spla                                     */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2021 JetBrains-Research                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include "cl_kernel_builder.hpp"

namespace spla {

    CLKernelBuilder& CLKernelBuilder::add_define(const char* define, int value) {
        m_defines.emplace_back(define + std::string(" ") + std::to_string(value));
        return *this;
    }
    CLKernelBuilder& CLKernelBuilder::add_type(const char* define, const ref_ptr<Type>& type) {
        m_defines.emplace_back(define + std::string(" ") + type->get_cpp());
        return *this;
    }
    CLKernelBuilder& CLKernelBuilder::add_op(const char* name, const ref_ptr<OpBinary>& op) {
        m_functions.emplace_back(op->get_type_res()->get_cpp() + " " + name + " " + op->get_source());
        return *this;
    }
    CLKernelBuilder& CLKernelBuilder::add_op(const char* name, const ref_ptr<OpSelect>& op) {
        m_functions.emplace_back(std::string("bool ") + name + " " + op->get_source());
        return *this;
    }
    CLKernelBuilder& CLKernelBuilder::add_code(const char* source) {
        m_sources.emplace_back(source);
        return *this;
    }
    bool CLKernelBuilder::build() {
        std::stringstream builder;
        CLAccelerator*    acc = get_acc_cl();

        for (const auto& define : m_defines) {
            builder << "#define " << define << "\n";
        }
        for (const auto& function : m_functions) {
            builder << function << "\n";
        }
        for (const auto& source : m_sources) {
            builder << source;
        }

        m_source  = builder.str();
        m_program = cl::Program(acc->get_context(), m_source);

        if (m_program.build("-cl-std=CL1.2") != CL_SUCCESS) {
            LOG_MSG(Status::Error, "failed to build program: " << m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(acc->get_device()));
            LOG_MSG(Status::Error, "src\n" << m_source);
            return false;
        }

        return true;
    }

}// namespace spla