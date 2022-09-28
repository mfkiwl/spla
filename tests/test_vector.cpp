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

#include "test_common.hpp"

#include <spla/spla.hpp>

TEST(vector, get_set_naive) {
    const spla::uint N    = 10;
    const int        X[N] = {1, 2, 3, 4, 5, -3, -3, 5, -8, 1};

    auto ivec = spla::make_vector(N, spla::INT);

    for (spla::uint i = 0; i < N; ++i) {
        ivec->set_int(i, X[i]);
    }

    for (spla::uint i = 0; i < N; ++i) {
        int x;
        ivec->get_int(i, x);
        EXPECT_EQ(x, X[i]);
    }
}

TEST(vector, get_set_reduce) {
    const spla::uint N    = 10;
    const int        X[N] = {1, 2, 3, 4, 5, -3, -3, 5, -8, 1};

    auto ivec = spla::make_vector(N, spla::INT);

    for (spla::uint i = 0; i < N; ++i) {
        ivec->set_int(i, X[i]);
        ivec->set_int(i, X[i]);
    }

    for (spla::uint i = 0; i < N; ++i) {
        int x;
        ivec->get_int(i, x);
        EXPECT_EQ(x, X[i]);
    }
}

SPLA_GTEST_MAIN_WITH_FINALIZE