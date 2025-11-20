#include <unity.h>
#include <RustOption.h>
#include <RustResult.h>
#include <RustNumberTypes.h>
#include <stdio.h>

static constexpr usize NUMBER_SIZE_LIMIT = 1000000;

enum class Error : u16{
    NullPtr,
    NumberTooBig,
    OptionWasNone
};

using FooResult = Result<i64, Error>;
FooResult foo(const Option<i64 const*>& opt);
void print_foo_result(const FooResult& result);

// using OptInt = Option<i64 const*>;

void run_basic_tests(){
    const i64 my_number = NUMBER_SIZE_LIMIT - 1;
    const i64 my_big_number = NUMBER_SIZE_LIMIT + 1;

    const auto none = Option<i64 const*>::None();
    const auto some_nullptr = Option<i64 const*>::Some(nullptr);
    const auto some_number = Option<i64 const*>::Some(&my_number);
    const auto some_big_number = Option<i64 const*>::Some(&my_big_number);

    const auto result_1 = foo(none);
    const auto result_2 = foo(some_nullptr);
    const auto result_3 = foo(some_number);
    const auto result_4 = foo(some_big_number);

    TEST_ASSERT(result_1 == FooResult::Err(Error::OptionWasNone));
    TEST_ASSERT(result_2 == FooResult::Err(Error::NullPtr));
    TEST_ASSERT(result_3 == FooResult::Ok(my_number));
    TEST_ASSERT(result_4 == FooResult::Err(Error::NumberTooBig));

    print_foo_result(result_1);
    print_foo_result(result_2);
    print_foo_result(result_3);
    print_foo_result(result_4);
}


FooResult foo(const Option<i64 const*>& opt){
    switch(opt.tag){
        case OptionTag::Some: {
            if(opt.data == nullptr){
                return FooResult::Err(Error::NullPtr);
            }
            const i64 number = *opt.data;
            if(number >= NUMBER_SIZE_LIMIT){
                return FooResult::Err(Error::NumberTooBig);
            }
            return FooResult::Ok(number);
        }
        case OptionTag::None: {
            return FooResult::Err(Error::OptionWasNone);
        }
    }
}



static const char* error_to_str(const Error error){
    switch(error){
        case Error::NullPtr: { return "NullPtr"; }
        case Error::NumberTooBig: { return "NumberTooBig"; }
        case Error::OptionWasNone: { return "OptionWasNone"; }
    }
}

void print_foo_result(const FooResult& result){
    switch(result.tag){
        case ResultTag::Ok: {
            printf("Ok: %ld\n", result.value);
            break;
        }
        case ResultTag::Err: {
            printf("Err: %s\n", error_to_str(result.error));
            break;
        }
    }
}



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(run_basic_tests);
    return UNITY_END();
}
