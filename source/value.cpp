#include <stdexcept>
#include <codecvt>

#include <xlnt/cell/value.hpp>
#include <xlnt/common/datetime.hpp>

namespace xlnt {

value value::error(const std::string &error_string)
{
    value v(error_string);
    v.type_ = type::error;
    return v;
}

value::value() : type_(type::null), numeric_value_(0)
{
}

value::value(value &&v)
{
    swap(*this, v);
}

value::value(const value &v)
{
    type_ = v.type_;
    numeric_value_ = v.numeric_value_;
    string_value_ = v.string_value_;
}
    
value::value(bool b) : type_(type::boolean), numeric_value_(b ? 1 : 0)
{
}

value::value(int i) : type_(type::numeric), numeric_value_(i)
{
}

value::value(double d) : type_(type::numeric), numeric_value_(d)
{
}

value::value(int64_t i) : type_(type::numeric), numeric_value_((long double)i)
{
}

value::value(const char *s) : value(std::string(s))
{
}

value::value(const std::string &s) : type_(type::string), string_value_(s)
{
}

value::value(const wchar_t *s) : value(std::wstring(s))
{
}

value::value(const std::wstring &s) : type_(type::string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	string_value_ = utf8_conv.to_bytes(s);
}

value &value::operator=(value other)
{
    swap(*this, other);
    return *this;
}

value &value::operator=(int i)
{
    return *this = value(i);
}

bool value::is(type t) const
{
    return type_ == t;
}
    
    template<>
    std::string value::get() const
    {
        if(type_ == type::string)
        {
            return string_value_;
        }
        
        throw std::runtime_error("not a string");
    }

template<>
double value::as() const
{
    switch(type_)
    {
    case type::boolean:
    case type::numeric:
        return (double)numeric_value_;
    case type::string:
        return std::stod(string_value_);
    case type::error:
        throw std::runtime_error("invalid");
    case type::null:
        return 0;
    }

    return 0;
}

template<>
int value::as() const
{
    switch(type_)
    {
    case type::boolean:
    case type::numeric:
        return (int)numeric_value_;
    case type::string:
        return std::stoi(string_value_);
    case type::error:
        throw std::runtime_error("invalid");
    case type::null:
        return 0;
    }

    return 0;
}

template<>
int64_t value::as() const
{
    switch(type_)
    {
    case type::boolean:
    case type::numeric:
        return (int64_t)numeric_value_;
    case type::string:
        return std::stoi(string_value_);
    case type::error:
        throw std::runtime_error("invalid");
    case type::null:
        return 0;
    }

    return 0;
}

template<>
bool value::as() const
{
    switch(type_)
    {
    case type::boolean:
    case type::numeric:
        return numeric_value_ != 0;
    case type::string:
        return !string_value_.empty();
    case type::error:
        throw std::runtime_error("invalid");
    case type::null:
        return false;
    }

    return false;
}

bool value::is_integral() const
{
    return type_ == type::numeric && (int64_t)numeric_value_ == numeric_value_;
}

template<>
std::string value::as() const
{
    return to_string();
}

value value::null()
{
    return value();
}

value::type value::get_type() const
{
    return type_;
}

std::string value::to_string() const
{
    switch(type_)
    {
    case type::boolean:
        return numeric_value_ != 0 ? "1" : "0";
    case type::numeric:
        return std::to_string(numeric_value_);
    case type::string:
    case type::error:
        return string_value_;
    case type::null:
        return "";
    }

    return "";
}

std::wstring value::to_wstring() const
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;

	switch (type_)
	{
	case type::boolean:
		return numeric_value_ != 0 ? L"1" : L"0";
	case type::numeric:
		return std::to_wstring(numeric_value_);
	case type::string:
	case type::error:
		return utf8_conv.from_bytes(string_value_);
	case type::null:
		return L"";
	}

	return L"";
}

bool value::operator==(bool value) const
{
    return type_ == type::boolean && (numeric_value_ != 0) == value;
}

bool value::operator==(int comparand) const
{
    return type_ == type::numeric && numeric_value_ == comparand;
}

bool value::operator==(double comparand) const
{
    return type_ == type::numeric && numeric_value_ == comparand;
}

bool value::operator==(const std::wstring &comparand) const
{
	if (type_ == type::string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
		return operator==(utf8_conv.to_bytes(comparand));
	}

	return false;
}

bool value::operator==(const std::string &comparand) const
{
    if(type_ == type::string)
    {
        return string_value_ == comparand;
    }

    return false;
}

bool value::operator==(const char *comparand) const
{
    return *this == std::string(comparand);
}

bool value::operator==(const wchar_t *comparand) const
{
	return *this == std::wstring(comparand);
}

bool value::operator==(const time &comparand) const
{
    if(type_ != type::numeric)
    {
        return false;
    }

    return time::from_number(numeric_value_) == comparand;
}

bool value::operator==(const date &comparand) const
{
    return type_ == type::numeric && comparand.to_number(calendar::windows_1900) == numeric_value_;
}

bool value::operator==(const datetime &comparand) const
{
    return type_ == type::numeric && comparand.to_number(calendar::windows_1900) == numeric_value_;
}

bool value::operator==(const timedelta &comparand) const
{
    return type_ == type::numeric && comparand.to_number() == numeric_value_;
}

bool value::operator==(const value &v) const
{
    if(type_ != v.type_) return false;
    if(type_ == type::string || type_ == type::error) return string_value_ == v.string_value_;
    if(type_ == type::numeric || type_ == type::boolean) return numeric_value_ == v.numeric_value_;
    return true;
}

/*
value &value::operator=(const time &value)
{
    d_->type_ = type::numeric;
    d_->numeric_value = value.to_number();
    d_->is_date_ = true;
    return *this;
}

value &value::operator=(const date &value)
{
    d_->type_ = type::numeric;
    auto base_year = worksheet(d_->parent_).get_parent().get_properties().excel_base_date;
    d_->numeric_value = value.to_number(base_year);
    d_->is_date_ = true;
    return *this;
}

value &value::operator=(const datetime &value)
{
    d_->type_ = type::numeric;
    auto base_year = worksheet(d_->parent_).get_parent().get_properties().excel_base_date;
    d_->numeric_value = value.to_number(base_year);
    d_->is_date_ = true;
    return *this;
}

value &value::operator=(const timedelta &value)
{
    d_->type_ = type::numeric;
    d_->numeric_value = value.to_number();
    d_->is_date_ = true;
    return *this;
}
*/

bool operator==(bool comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(int comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(const char *comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(const wchar_t *comparand, const xlnt::value &value)
{
	return value == comparand;
}

bool operator==(const std::string &comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(const std::wstring &comparand, const xlnt::value &value)
{
	return value == comparand;
}

bool operator==(const time &comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(const date &comparand, const xlnt::value &value)
{
    return value == comparand;
}

bool operator==(const datetime &comparand, const xlnt::value &value)
{
    return value == comparand;
}

void swap(value &left, value &right)
{
    using std::swap;
    swap(left.type_, right.type_);
    swap(left.string_value_, right.string_value_);
    swap(left.numeric_value_, right.numeric_value_);
}

} // namespace xlnt
