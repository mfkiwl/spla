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

#ifndef SPLA_SPLAUTILS_HPP
#define SPLA_SPLAUTILS_HPP

#include <fstream>
#include <istream>
#include <sstream>
#include <type_traits>
#include <vector>

#include <spla-cpp/SplaConfig.hpp>

namespace spla {

    /**
     * @addtogroup API
     * @{
     */

    /**
     * @brief A matrix loader of .mtx format
     *
     * @tparam Value A type of target matrix value
     */
    template<typename Value>
    class SPLA_API MatrixLoader {
    public:
        static constexpr bool HasValue = !std::is_same_v<Value, void>;
        using ValueCollectionType = std::conditional_t<std::is_same_v<Value, void>, nullptr_t, std::vector<Value>>;

    public:
        MatrixLoader() = default;

        template<typename FileValue>
        MatrixLoader &Load(std::istream &is) {
            static_assert(!(std::is_same_v<FileValue, void> && !std::is_same_v<Value, void>) );
            std::string line;
            std::size_t lineN = 0;
            while (std::getline(is, line)) {
                ++lineN;
                if (line[0] != '%') break;
            }
            Size nnz;
            std::stringstream headerLineStream(line);
            headerLineStream >> mNrows >> mNcols >> nnz;

            if constexpr (HasValue) {
                mVals.reserve(nnz);
            }
            mRows.reserve(nnz);
            mCols.reserve(nnz);

            while (std::getline(is, line)) {
                ++lineN;
                std::stringstream lineStream(line);
                Index i, j;
                lineStream >> i >> j;
                mRows.push_back(i);
                mCols.push_back(j);

                if (!(0 < i && i <= mNrows)) {
                    throw std::logic_error("Row index is out of bounds on the line " + std::to_string(lineN));
                }

                if (!(0 < j && j <= mNcols)) {
                    throw std::logic_error("Column index is out of bounds on the line " + std::to_string(lineN));
                }

                if constexpr (!std::is_same_v<FileValue, void>) {
                    FileValue value;
                    lineStream >> value;
                    if constexpr (HasValue) {
                        mVals.push_back(static_cast<Value>(value));
                    }
                }
            }

            if (mRows.size() != nnz) {
                throw std::logic_error("Number of non zero values is not valid");
            }

            return *this;
        }

        template<typename FileValue>
        MatrixLoader &Load(const std::string &filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::invalid_argument("Could not open '" + filename + "' to read matrix");
            }
            return Load<FileValue>(file);
        }

        [[nodiscard]] Size GetNrows() const { return mNrows; }
        [[nodiscard]] Size GetNcols() const { return mNcols; }
        [[nodiscard]] Size GetNnvals() const { return mRows.size(); }
        [[nodiscard]] static constexpr Size GetElementSize() { return sizeof(Value); }

        [[nodiscard]] const std::vector<Index> &GetRowIndices() { return mRows; }
        [[nodiscard]] const std::vector<Index> &GetColIndices() { return mCols; }

        [[nodiscard]] std::conditional_t<HasValue, const ValueCollectionType &, nullptr_t> GetValues() const {
            return mVals;
        }

    private:
        ValueCollectionType mVals{};
        std::vector<Index> mRows{};
        std::vector<Index> mCols{};

        Size mNrows{};
        Size mNcols{};
    };

    /**
     * @}
     */

}// namespace spla

#endif//SPLA_SPLAUTILS_HPP