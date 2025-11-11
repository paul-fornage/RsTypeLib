//
// Created by pfornage on 11/11/25.
//

#ifndef RSTYPELIB_RUSTOPTION_H
#define RSTYPELIB_RUSTOPTION_H


#include <utility>
#include <new>

enum class OptionTag: bool{
    Some = true,
    None = false
};

template<typename T>
struct Option{


    OptionTag tag;
    union {
        T data;
    };

    inline bool is_some() const {
        return static_cast<bool>(tag);
    }

    const T* as_ptr() const {
        if(static_cast<bool>(tag)){
            return &data;
        } else {
            return nullptr;
        }
    }

    ~Option() {
        if (tag == OptionTag::Some) {
            data.~T();
        }
    }

    // Copy constructor
    Option(const Option& other) : tag(other.tag) {
        if (tag == OptionTag::Some) {
            new (&data) T(other.data);
        }
    }

    // Move constructor
    Option(Option&& other) noexcept : tag(other.tag) {
            if (tag == OptionTag::Some) {
                new (&data) T(std::move(other.data));
            }
    }

    // Copy assignment
    Option& operator=(const Option& other) {
        if (this != &other) {
            if (tag == OptionTag::Some) {
                data.~T();
            }
            tag = other.tag;
            if (tag == OptionTag::Some) {
                new (&data) T(other.data);
            }
        }
        return *this;
    }

    // Move assignment
    Option& operator=(Option&& other) noexcept {
        if (this != &other) {
            if (tag == OptionTag::Some) {
                data.~T();
            }
            tag = other.tag;
            if (tag == OptionTag::Some) {
                new (&data) T(std::move(other.data));
            }
        }
        return *this;
    }

    /// Moves the data
    T unwrap(){
        return std::move(data);
    }

    static Option<T> Some(T value){
        Option opt;
        opt.tag = OptionTag::Some;
        new (&opt.data) T(std::move(value));
        return opt;
    }

    static Option<T> None(){
        Option opt;
        opt.tag = OptionTag::None;
        return opt;
    }

    // Equality operator
    bool operator==(const Option& other) const {
        if (tag != other.tag) {
            return false;
        }
        if (tag == OptionTag::Some) {
            return data == other.data;
        }
        return true; // Both are None
    }

    // Inequality operator
    bool operator!=(const Option& other) const { return !operator==(other); }

private:
    explicit Option() = default;
};


#endif //RSTYPELIB_RUSTOPTION_H
