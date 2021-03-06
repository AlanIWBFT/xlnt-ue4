#pragma once

#include <xlnt/cell/cell.hpp>
#include <xlnt/cell/comment.hpp>
#include <xlnt/cell/value.hpp>
#include <xlnt/common/types.hpp>
#include <xlnt/common/relationship.hpp>

namespace xlnt {

class style;

namespace detail {

struct worksheet_impl;
    
struct cell_impl
{
    cell_impl();
    cell_impl(worksheet_impl *parent, int column_index, int row_index);
    cell_impl(const cell_impl &rhs);
    cell_impl &operator=(const cell_impl &rhs);

    worksheet_impl *parent_;
    value value_;
    std::string formula_;
    relationship hyperlink_;
    column_t column_;
    row_t row_;
    style *style_;
    bool merged;
    bool is_date_;
    bool has_hyperlink_;
    comment comment_;
};
    
} // namespace detail
} // namespace xlnt
