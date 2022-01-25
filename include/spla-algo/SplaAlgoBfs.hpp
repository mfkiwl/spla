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

#ifndef SPLA_SPLAALGOBFS_HPP
#define SPLA_SPLAALGOBFS_HPP

#include <spla-algo/SplaAlgoCommon.hpp>
#include <spla-cpp/SplaMatrix.hpp>
#include <spla-cpp/SplaVector.hpp>

namespace spla {

    /**
     * @addtogroup Algorithm
     * @{
     */

    /**
     * @brief Breadth-first search algorithm
     *
     * @param[out] v Vector where to store levels of the reached vertices
     * @param A Input adjacency matrix of the graph; must be n x n
     * @param s Index of the source vertex to begin bfs
     * @param descriptor Algorithm descriptor
     */
    SPLA_API void Bfs(RefPtr<Vector> &v, const RefPtr<Matrix> &A, Index s, const AlgoDescriptor &descriptor = {});

    /**
     * @brief Breadth-first search algorithm
     *
     * @note Naive cpu reference algo implementation for correctness only
     *
     * @param[out] v Vector where to store levels of the reached vertices
     * @param A Input adjacency matrix of the graph; must be n x n
     * @param s Index of the source vertex to begin bfs
     */
    SPLA_API void Bfs(RefPtr<HostVector> &v, const RefPtr<HostMatrix> &A, Index s);

    /**
     * @}
     */

}// namespace spla

#endif//SPLA_SPLAALGOBFS_HPP
