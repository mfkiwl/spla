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

#include <spla/library.hpp>

#include <core/accelerator.hpp>
#include <core/logger.hpp>

#include <iostream>

#if defined(SPLA_BUILD_OPENCL)
    #include <opencl/cl_accelerator.hpp>
#endif

namespace spla {

    Library::Library() {
        // Setup logger (always present in the system)
        m_logger = std::make_unique<Logger>();
    }

    Library::~Library() = default;

    void Library::finalize() {
        LOG_MSG(Status::Ok, "finalize library state");

        if (m_accelerator) {
            LOG_MSG(Status::Ok, "release accelerator: " << m_accelerator->get_name());
            m_accelerator.reset();
        }

        m_logger.reset();
    }

    Status Library::set_accelerator(AcceleratorType accelerator) {
#if defined(SPLA_BUILD_OPENCL)
        if (accelerator == AcceleratorType::OpenCL) {
            m_accelerator = std::make_unique<CLAccelerator>();

            if (m_accelerator->init() != Status::Ok) {
                m_accelerator.reset();
                return Status::NoAcceleration;
            }

            return Status::Ok;
        }
#endif
        return Status::NoAcceleration;
    }

    Status Library::set_platform(int index) {
        return m_accelerator ? m_accelerator->set_platform(index) : Status::NoAcceleration;
    }

    Status Library::set_device(int index) {
        return m_accelerator ? m_accelerator->set_device(index) : Status::NoAcceleration;
    }

    Status Library::set_queues_count(int count) {
        return m_accelerator ? m_accelerator->set_queues_count(count) : Status::NoAcceleration;
    }

    Status Library::set_message_callback(MessageCallback callback) {
        m_logger->set_msg_callback(std::move(callback));
        LOG_MSG(Status::Ok, "set new message callback");
        return Status::Ok;
    }

    Status Library::set_default_callback() {
        auto callback = [](spla::Status status,
                           const std::string &msg,
                           const std::string &file,
                           const std::string &function,
                           int line) {
            std::stringstream to_output;

            to_output << "[" << file << ":" << line << "] "
                      << to_string(status) << ": " << msg << std::endl;

            if (status == Status::Ok)
                std::cout << to_output.str();
            else
                std::cerr << to_output.str();
        };
        return set_message_callback(callback);
    }

    class Accelerator *Library::get_accelerator() {
        return m_accelerator.get();
    }

    class Logger *Library::get_logger() {
        return m_logger.get();
    }

    Library *get_library() {
        static std::unique_ptr<Library> g_library;

        if (!g_library) {
            g_library = std::make_unique<Library>();

#if defined(SPLA_DEBUG)
            // In debug mode we automatically set default callback
            // to get log (error) messages from library start-up
            g_library->set_default_callback();
#endif
            // On init we by default attempt to setup OpenCL runtime
            // If setup is failed error is ignored and library fallbacks to CPU computations only
            g_library->set_accelerator(AcceleratorType::OpenCL);
        }

        return g_library.get();
    }

}// namespace spla