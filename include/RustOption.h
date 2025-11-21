
#ifndef RSTYPELIB_RUSTOPTION_H
#define RSTYPELIB_RUSTOPTION_H


#include <utility>
#include <new>

enum class OptionTag: bool{
    Some = true,
    None = false
};

namespace OptionCtorTag {
    struct StaticNone{};
}

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

    T* as_mut_ptr() const {
        if(static_cast<bool>(tag)){
            return &data;
        } else {
            return nullptr;
        }
    }

    T& unwrap_or(T& none_default) & {
        return is_some() ? data : none_default;
    }

    const T& unwrap_or(const T& none_default) const & {
        return is_some() ? data : none_default;
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

    template <class U>
    static Option Some(U&& value) {
        static_assert(
                std::is_constructible<T, U&&>::value,
                "T must be constructible from U&&"
            );

        Option opt;
        opt.tag = OptionTag::Some;
        new (&opt.data) T(std::forward<U>(value));
        return opt;
    }

    static Option None(){
        Option opt;
        opt.tag = OptionTag::None;
        return opt;
    }


    template<
        typename CtorTag,
        typename std::enable_if<
            std::is_same<CtorTag, OptionCtorTag::StaticNone>::value,
            int
        >::type = 0
    >
    explicit Option(CtorTag _ctor_tag) : tag(OptionTag::None) {}

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
    explicit Option(): tag(OptionTag::None){};
};


#endif //RSTYPELIB_RUSTOPTION_H
