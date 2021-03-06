// Copyright (c) 2014 Thomas Fussell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, WRISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "range_reference.hpp"
#include "worksheet.hpp"

namespace xlnt {
    
enum class major_order
{
    column,
    row
};

class cell_vector
{
public:
    cell_vector(worksheet ws, const range_reference &ref, major_order order = major_order::row);

    std::size_t num_cells() const;

    cell front();

    const cell front() const;

    cell back();

    const cell back() const;

    cell operator[](std::size_t column_index);

    const cell operator[](std::size_t column_index) const;

    cell get_cell(std::size_t column_index);

    const cell get_cell(std::size_t column_index) const;

    class iterator
    {
    public:
        iterator(worksheet ws, const cell_reference &start_cell, major_order order);
        ~iterator();

        bool operator==(const iterator &rhs);
        bool operator!=(const iterator &rhs) { return !(*this == rhs); }

        iterator operator++(int);
        iterator &operator++();

        cell operator*();

    private:
        worksheet ws_;
        cell_reference current_cell_;
        range_reference range_;
        major_order order_;
    };

    iterator begin();
    iterator end();

    class const_iterator
    {
    public:
        const_iterator(worksheet ws, const cell_reference &start_cell, major_order order);
        ~const_iterator();

        bool operator==(const const_iterator &rhs);
        bool operator!=(const const_iterator &rhs) { return !(*this == rhs); }

        const_iterator operator++(int);
        const_iterator &operator++();

        const cell operator*();

    private:
        worksheet ws_;
        cell_reference current_cell_;
        range_reference range_;
        major_order order_;
    };

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    worksheet ws_;
    range_reference ref_;
    major_order order_;
};

class range
{
public:
    range(worksheet ws, const range_reference &reference, major_order order = major_order::row);

    ~range();

    cell_vector operator[](std::size_t vector_index);

    const cell_vector operator[](std::size_t vector_index) const;

    bool operator==(const range &comparand) const;

    bool operator!=(const range &comparand) const { return !(*this == comparand); }

    cell_vector get_vector(std::size_t vector_index);

    const cell_vector get_vector(std::size_t vector_index) const;

    cell get_cell(const cell_reference &ref);

    const cell get_cell(const cell_reference &ref) const;

    range_reference get_reference() const;

    std::size_t length() const;

    class iterator
    {
    public:
        iterator(worksheet ws, const range_reference &start_cell, major_order order);
        ~iterator();

        bool operator==(const iterator &rhs);
        bool operator!=(const iterator &rhs) { return !(*this == rhs); }

        iterator operator++(int);
        iterator &operator++();

        cell_vector operator*();

    private:
        worksheet ws_;
        cell_reference current_cell_;
        range_reference range_;
        major_order order_;
    };

    iterator begin();
    iterator end();

    class const_iterator
    {
    public:
        const_iterator(worksheet ws, const range_reference &start_cell, major_order order);
        ~const_iterator();

        bool operator==(const const_iterator &rhs);
        bool operator!=(const const_iterator &rhs) { return !(*this == rhs); }

        const_iterator operator++(int);
        const_iterator &operator++();

        const cell_vector operator*();

    private:
        worksheet ws_;
        cell_reference current_cell_;
        range_reference range_;
        major_order order_;
    };

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    worksheet ws_;
    range_reference ref_;
    major_order order_;
};

} // namespace xlnt
