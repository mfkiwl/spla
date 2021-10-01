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

#ifndef SPLA_SPLANODEPROCESSOR_HPP
#define SPLA_SPLANODEPROCESSOR_HPP

#include <expression/SplaExpressionContext.hpp>
#include <spla-cpp/SplaExpressionNode.hpp>
#include <spla-cpp/SplaRefCnt.hpp>

namespace spla {

    /**
     * @class NodeProcessor
     *
     * Interface to the expression node processor class.
     *
     * Node processor allows to implement custom node processing logic and
     * select required processor automatically in runtime. Selection is based
     * on the node content and global library setting.
     *
     * Single node operation may be supported in multiple node processors.
     * In runtime expression manager will select best matching processor for execution.
     *
     * @see ExpressionManager
     */
    class NodeProcessor : public RefCnt {
    public:
        /**
         *
         * @param nodeIdx
         * @param context
         *
         * @return
         */
        virtual bool Select(size_t nodeIdx, ExpressionContext &context) = 0;

        /**
         *
         * @param nodeIdx
         * @param context
         */
        virtual void Process(size_t nodeIdx, ExpressionContext &context) = 0;

        /** @return Type of the expression node operation, handled by this processor */
        virtual ExpressionNode::Operation GetOperationType() const = 0;
    };

}// namespace spla

#endif//SPLA_SPLANODEPROCESSOR_HPP