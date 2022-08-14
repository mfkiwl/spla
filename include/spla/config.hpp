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

#ifndef SPLA_CONFIG_HPP
#define SPLA_CONFIG_HPP

#ifdef SPLA_MSVC
    #ifdef SPLA_EXPORTS
        #define SPLA_API __declspec(dllexport)
    #else
        #define SPLA_API __declspec(dllimport)
    #endif
#else
    #define SPLA_API
#endif

namespace spla {

    /**
     * @addtogroup spla
     * @{
     */

    /**
     * @class Status
     * @brief Status of library operation execution
     */
    enum class Status : int {
        /** No error */
        Ok = 0,
        /** Some error occurred */
        Error,
        /** Library has no configured accelerator for computations */
        NoAcceleration,
        /** Accelerator platform not found */
        PlatformNotFound,
        /** Accelerator device not found */
        DeviceNotFound,
        /** Call of the function is not possible for some context */
        InvalidState,
        /** Passed invalid argument for some function */
        InvalidArgument,
        /** Some library feature is not implemented */
        NotImplemented
    };

    /**
     * @class AcceleratorType
     * @brief Types of supported accelerators for computations
     */
    enum class AcceleratorType {
        /** No acceleration to be used */
        None,
        /** OpenCL-based single device acceleration */
        OpenCL
    };

    /**
     * @brief Convert status value to string
     *
     * @param status Status value
     *
     * @return String value
     */
    static const char *to_string(Status status) {
#define STATUS_MAP(value) \
    case Status::value:   \
        return #value

        switch (status) {
            STATUS_MAP(Ok);
            STATUS_MAP(Error);
            STATUS_MAP(NoAcceleration);
            STATUS_MAP(PlatformNotFound);
            STATUS_MAP(DeviceNotFound);
            STATUS_MAP(InvalidState);
            STATUS_MAP(InvalidArgument);
            STATUS_MAP(NotImplemented);
            default:
                return "none";
        }
#undef STATUS_MAP
    }

    /**
     * @}
     */

}// namespace spla

#endif//SPLA_CONFIG_HPP
