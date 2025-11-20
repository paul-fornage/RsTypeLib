//
// Created by pfornage on 11/11/25.
//

#ifndef RSTYPELIB_RUSTRESULT_H
#define RSTYPELIB_RUSTRESULT_H

#include <utility>
#include <new>

enum class ResultTag : bool{
    Ok = true,
    Err = false
};

template<typename T, typename E>
struct Result {
public:
    union{
        T value;
        E error;
    };
    ResultTag tag;

private:
    explicit Result() : tag(ResultTag::Err) {}
public:
    ~Result()  {
        if (tag == ResultTag::Ok) {
            value.~T();
        } else {
            error.~E();
        }
    }

    // Copy constructor
    Result(const Result& other) : tag(other.tag) {
        if (tag == ResultTag::Ok) {
            new (&value) T(other.value);
        } else {
            new (&error) E(other.error);
        }
    }

    // Copy assignment operator
    Result& operator=(const Result& other) {
        if (this != &other) {
            // Destroy current object
            if (tag == ResultTag::Ok) {
                value.~T();
            } else {
                error.~E();
            }


            tag = other.tag;
            if (tag == ResultTag::Ok) {
                new (&value) T(other.value);
            } else {
                new (&error) E(other.error);
            }
        }
        return *this;
    }

    // Move constructor
    Result(Result&& other) noexcept : tag(other.tag) {
            if (tag == ResultTag::Ok) {
                new (&value) T(std::move(other.value));
            } else {
                new (&error) E(std::move(other.error));
            }
    }

    // Move assignment operator
    Result& operator=(Result&& other) noexcept {
        if (this != &other) {
            // Destroy current object
            if (tag == ResultTag::Ok) {
                value.~T();
            } else {
                error.~E();
            }

            // Move from other
            tag = other.tag;
            if (tag == ResultTag::Ok) {
                new (&value) T(std::move(other.value));
            } else {
                new (&error) E(std::move(other.error));
            }
        }
        return *this;
    }

    /**
     * Constructor for success case. Moves the value into the result.
     * @param value
     * @return
     */
    static Result Ok(T value) {
        Result res;
        res.tag = ResultTag::Ok;
        new (&res.value) T(std::move(value));
        return res;
    }

    // Constructor for error case
    static Result Err(E error) {
        Result res;
        res.tag = ResultTag::Err;
        new (&res.error) E(std::move(error));
        return res;
    }

    // you will get garbage if you call this on an Err result
    T unwrap() {
        return std::move(value);
    }

    // you will get garbage if you call this on an Ok result
    E unwrap_err() {
        return std::move(error);
    }

    const T* get_value() const {
        if(tag == ResultTag::Ok){
            return &value;
        } else {
            return nullptr;
        }
    }

    const E* get_error() const {
        if(tag == ResultTag::Ok){
            return nullptr;
        } else {
            return &error;
        }
    }

    bool is_ok() const {
        return tag == ResultTag::Ok;
    }

    bool is_err() const {
        return tag == ResultTag::Err;
    }

    // Equality operator
    bool operator==(const Result& other) const {
        if (tag != other.tag) {
            return false;
        }
        if (tag == ResultTag::Ok) {
            return value == other.value;
        } else {
            return error == other.error;
        }
    }

    // Inequality operator
    bool operator!=(const Result& other) const {
        return !operator==(other);
    }

};



#endif //RSTYPELIB_RUSTRESULT_H
