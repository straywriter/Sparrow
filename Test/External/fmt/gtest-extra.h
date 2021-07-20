// Formatting library for C++ - custom Google Test assertions
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#pragma once

#ifndef FMT_GTEST_EXTRA_H_
#define FMT_GTEST_EXTRA_H_

#include <stdlib.h> // _invalid_parameter_handler

#include <string>

#include "fmt/os.h"
#include "gmock/gmock.h"

#define FMT_TEST_THROW_(statement, expected_exception, expected_message, fail)                                         \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_                                                                                      \
    if (::testing::AssertionResult gtest_ar = ::testing::AssertionSuccess())                                           \
    {                                                                                                                  \
        std::string gtest_expected_message = expected_message;                                                         \
        bool gtest_caught_expected = false;                                                                            \
        try                                                                                                            \
        {                                                                                                              \
            GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement);                                                 \
        }                                                                                                              \
        catch (expected_exception const &e)                                                                            \
        {                                                                                                              \
            if (gtest_expected_message != e.what())                                                                    \
            {                                                                                                          \
                gtest_ar << #statement " throws an exception with a different message.\n"                              \
                         << "Expected: " << gtest_expected_message << "\n"                                             \
                         << "  Actual: " << e.what();                                                                  \
                goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__);                                            \
            }                                                                                                          \
            gtest_caught_expected = true;                                                                              \
        }                                                                                                              \
        catch (...)                                                                                                    \
        {                                                                                                              \
            gtest_ar << "Expected: " #statement " throws an exception of type " #expected_exception                    \
                        ".\n  Actual: it throws a different type.";                                                    \
            goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__);                                                \
        }                                                                                                              \
        if (!gtest_caught_expected)                                                                                    \
        {                                                                                                              \
            gtest_ar << "Expected: " #statement " throws an exception of type " #expected_exception                    \
                        ".\n  Actual: it throws nothing.";                                                             \
            goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__);                                                \
        }                                                                                                              \
    }                                                                                                                  \
    else                                                                                                               \
        GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__) : fail(gtest_ar.failure_message())

// Tests that the statement throws the expected exception and the exception's
// what() method returns expected message.
#define EXPECT_THROW_MSG(statement, expected_exception, expected_message)                                              \
    FMT_TEST_THROW_(statement, expected_exception, expected_message, GTEST_NONFATAL_FAILURE_)

inline std::string system_error_message(int error_code, const std::string &message)
{
    auto ec = std::error_code(error_code, std::generic_category());
    return std::system_error(ec, message).what();
}

#define EXPECT_SYSTEM_ERROR(statement, error_code, message)                                                            \
    EXPECT_THROW_MSG(statement, std::system_error, system_error_message(error_code, message))

#if FMT_USE_FCNTL

// Captures file output by redirecting it to a pipe.
// The output it can handle is limited by the pipe capacity.
class output_redirect
{
  private:
    FILE *file_;
    fmt::file original_; // Original file passed to redirector.
    fmt::file read_end_; // Read end of the pipe where the output is redirected.

    void flush();
    void restore();

  public:
    explicit output_redirect(FILE *file);
    ~output_redirect() FMT_NOEXCEPT;

    output_redirect(const output_redirect &) = delete;
    void operator=(const output_redirect &) = delete;

    // Restores the original file, reads output from the pipe into a string
    // and returns it.
    std::string restore_and_read();
};

#define FMT_TEST_WRITE_(statement, expected_output, file, fail)                                                        \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_                                                                                      \
    if (::testing::AssertionResult gtest_ar = ::testing::AssertionSuccess())                                           \
    {                                                                                                                  \
        std::string gtest_expected_output = expected_output;                                                           \
        output_redirect gtest_redir(file);                                                                             \
        GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement);                                                     \
        std::string gtest_output = gtest_redir.restore_and_read();                                                     \
        if (gtest_output != gtest_expected_output)                                                                     \
        {                                                                                                              \
            gtest_ar << #statement " produces different output.\n"                                                     \
                     << "Expected: " << gtest_expected_output << "\n"                                                  \
                     << "  Actual: " << gtest_output;                                                                  \
            goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__);                                                \
        }                                                                                                              \
    }                                                                                                                  \
    else                                                                                                               \
        GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__) : fail(gtest_ar.failure_message())

// Tests that the statement writes the expected output to file.
#define EXPECT_WRITE(file, statement, expected_output)                                                                 \
    FMT_TEST_WRITE_(statement, expected_output, file, GTEST_NONFATAL_FAILURE_)

#ifdef _MSC_VER

// Suppresses Windows assertions on invalid file descriptors, making
// POSIX functions return proper error codes instead of crashing on Windows.
class suppress_assert
{
  private:
    _invalid_parameter_handler original_handler_;
    int original_report_mode_;

    static void handle_invalid_parameter(const wchar_t *, const wchar_t *, const wchar_t *, unsigned, uintptr_t)
    {
    }

  public:
    suppress_assert()
        : original_handler_(_set_invalid_parameter_handler(handle_invalid_parameter)),
          original_report_mode_(_CrtSetReportMode(_CRT_ASSERT, 0))
    {
    }
    ~suppress_assert()
    {
        _set_invalid_parameter_handler(original_handler_);
        _CrtSetReportMode(_CRT_ASSERT, original_report_mode_);
    }
};

#define SUPPRESS_ASSERT(statement)                                                                                     \
    {                                                                                                                  \
        suppress_assert sa;                                                                                            \
        statement;                                                                                                     \
    }
#else
#define SUPPRESS_ASSERT(statement) statement
#endif // _MSC_VER

#define EXPECT_SYSTEM_ERROR_NOASSERT(statement, error_code, message)                                                   \
    EXPECT_SYSTEM_ERROR(SUPPRESS_ASSERT(statement), error_code, message)

// Attempts to read count characters from a file.
std::string read(fmt::file &f, size_t count);

#define EXPECT_READ(file, expected_content)                                                                            \
    EXPECT_EQ(expected_content, read(file, fmt::string_view(expected_content).size()))

#else
#define EXPECT_WRITE(file, statement, expected_output)                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        (void)(file);                                                                                                  \
        (void)(statement);                                                                                             \
        (void)(expected_output);                                                                                       \
        SUCCEED();                                                                                                     \
    } while (false)
#endif // FMT_USE_FCNTL

#endif // FMT_GTEST_EXTRA_H_

#include <gtest/gtest-spi.h>

#include <cstring>
#include <memory>
#include <stdexcept>

#include "fmt/os.h"
#include "util.h"

// Tests that assertion macros evaluate their arguments exactly once.
namespace
{
class single_evaluation_test : public ::testing::Test
{
  protected:
    single_evaluation_test()
    {
        p_ = s_;
        a_ = 0;
        b_ = 0;
    }

    static const char *const s_;
    static const char *p_;

    static int a_;
    static int b_;
};
} // namespace

const char *const single_evaluation_test::s_ = "01234";
const char *single_evaluation_test::p_;
int single_evaluation_test::a_;
int single_evaluation_test::b_;

void do_nothing()
{
}

FMT_NORETURN void throw_exception()
{
    throw std::runtime_error("test");
}

FMT_NORETURN void throw_system_error()
{
    throw fmt::system_error(EDOM, "test");
}

// Tests that when EXPECT_THROW_MSG fails, it evaluates its message argument
// exactly once.
TEST_F(single_evaluation_test, failed_expect_throw_msg)
{
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw_exception(), std::exception, p_++), "01234");
    EXPECT_EQ(s_ + 1, p_);
}

// Tests that when EXPECT_SYSTEM_ERROR fails, it evaluates its message argument
// exactly once.
TEST_F(single_evaluation_test, failed_expect_system_error)
{
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, p_++), "01234");
    EXPECT_EQ(s_ + 1, p_);
}

// Tests that assertion arguments are evaluated exactly once.
TEST_F(single_evaluation_test, exception_tests)
{
    // successful EXPECT_THROW_MSG
    EXPECT_THROW_MSG(
        { // NOLINT
            a_++;
            throw_exception();
        },
        std::exception, (b_++, "test"));
    EXPECT_EQ(1, a_);
    EXPECT_EQ(1, b_);

    // failed EXPECT_THROW_MSG, throws different type
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(
                                { // NOLINT
                                    a_++;
                                    throw_exception();
                                },
                                std::logic_error, (b_++, "test")),
                            "throws a different type");
    EXPECT_EQ(2, a_);
    EXPECT_EQ(2, b_);

    // failed EXPECT_THROW_MSG, throws an exception with different message
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(
                                { // NOLINT
                                    a_++;
                                    throw_exception();
                                },
                                std::exception, (b_++, "other")),
                            "throws an exception with a different message");
    EXPECT_EQ(3, a_);
    EXPECT_EQ(3, b_);

    // failed EXPECT_THROW_MSG, throws nothing
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(a_++, std::exception, (b_++, "test")), "throws nothing");
    EXPECT_EQ(4, a_);
    EXPECT_EQ(4, b_);
}

TEST_F(single_evaluation_test, system_error_tests)
{
    // successful EXPECT_SYSTEM_ERROR
    EXPECT_SYSTEM_ERROR(
        { // NOLINT
            a_++;
            throw_system_error();
        },
        EDOM, (b_++, "test"));
    EXPECT_EQ(1, a_);
    EXPECT_EQ(1, b_);

    // failed EXPECT_SYSTEM_ERROR, throws different type
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(
                                { // NOLINT
                                    a_++;
                                    throw_exception();
                                },
                                EDOM, (b_++, "test")),
                            "throws a different type");
    EXPECT_EQ(2, a_);
    EXPECT_EQ(2, b_);

    // failed EXPECT_SYSTEM_ERROR, throws an exception with different message
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(
                                { // NOLINT
                                    a_++;
                                    throw_system_error();
                                },
                                EDOM, (b_++, "other")),
                            "throws an exception with a different message");
    EXPECT_EQ(3, a_);
    EXPECT_EQ(3, b_);

    // failed EXPECT_SYSTEM_ERROR, throws nothing
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(a_++, EDOM, (b_++, "test")), "throws nothing");
    EXPECT_EQ(4, a_);
    EXPECT_EQ(4, b_);
}

#if FMT_USE_FCNTL
// Tests that when EXPECT_WRITE fails, it evaluates its message argument
// exactly once.
TEST_F(single_evaluation_test, failed_expect_write)
{
    EXPECT_NONFATAL_FAILURE(EXPECT_WRITE(stdout, std::printf("test"), p_++), "01234");
    EXPECT_EQ(s_ + 1, p_);
}

// Tests that assertion arguments are evaluated exactly once.
TEST_F(single_evaluation_test, write_tests)
{
    // successful EXPECT_WRITE
    EXPECT_WRITE(
        stdout,
        { // NOLINT
            a_++;
            std::printf("test");
        },
        (b_++, "test"));
    EXPECT_EQ(1, a_);
    EXPECT_EQ(1, b_);

    // failed EXPECT_WRITE
    EXPECT_NONFATAL_FAILURE(EXPECT_WRITE(
                                stdout,
                                { // NOLINT
                                    a_++;
                                    std::printf("test");
                                },
                                (b_++, "other")),
                            "Actual: test");
    EXPECT_EQ(2, a_);
    EXPECT_EQ(2, b_);
}

// Tests EXPECT_WRITE.
TEST(gtest_extra_test, expect_write)
{
    EXPECT_WRITE(stdout, do_nothing(), "");
    EXPECT_WRITE(stdout, std::printf("test"), "test");
    EXPECT_WRITE(stderr, std::fprintf(stderr, "test"), "test");
    EXPECT_NONFATAL_FAILURE(EXPECT_WRITE(stdout, std::printf("that"), "this"), "Expected: this\n"
                                                                               "  Actual: that");
}

TEST(gtest_extra_test, expect_write_streaming)
{
    EXPECT_WRITE(stdout, std::printf("test"), "test") << "unexpected failure";
    EXPECT_NONFATAL_FAILURE(EXPECT_WRITE(stdout, std::printf("test"), "other") << "expected failure",
                            "expected failure");
}

// Tests that the compiler will not complain about unreachable code in the
// EXPECT_THROW_MSG macro.
TEST(gtest_extra_test, expect_throw_no_unreachable_code_warning)
{
    int n = 0;
    using std::runtime_error;
    EXPECT_THROW_MSG(throw runtime_error(""), runtime_error, "");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(n++, runtime_error, ""), "");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw 1, runtime_error, ""), "");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw runtime_error("a"), runtime_error, "b"), "");
}

// Tests that the compiler will not complain about unreachable code in the
// EXPECT_SYSTEM_ERROR macro.
TEST(gtest_extra_test, expect_system_error_no_unreachable_code_warning)
{
    int n = 0;
    EXPECT_SYSTEM_ERROR(throw fmt::system_error(EDOM, "test"), EDOM, "test");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(n++, EDOM, ""), "");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw 1, EDOM, ""), "");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw fmt::system_error(EDOM, "aaa"), EDOM, "bbb"), "");
}

TEST(gtest_extra_test, expect_throw_behaves_like_single_statement)
{
    if (::testing::internal::AlwaysFalse())
        EXPECT_THROW_MSG(do_nothing(), std::exception, "");

    if (::testing::internal::AlwaysTrue())
        EXPECT_THROW_MSG(throw_exception(), std::exception, "test");
    else
        do_nothing();
}

TEST(gtest_extra_test, expect_system_error_behaves_like_single_statement)
{
    if (::testing::internal::AlwaysFalse())
        EXPECT_SYSTEM_ERROR(do_nothing(), EDOM, "");

    if (::testing::internal::AlwaysTrue())
        EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, "test");
    else
        do_nothing();
}

TEST(gtest_extra_test, expect_write_behaves_like_single_statement)
{
    if (::testing::internal::AlwaysFalse())
        EXPECT_WRITE(stdout, std::printf("x"), "x");

    if (::testing::internal::AlwaysTrue())
        EXPECT_WRITE(stdout, std::printf("x"), "x");
    else
        do_nothing();
}

// Tests EXPECT_THROW_MSG.
TEST(gtest_extra_test, expect_throw_msg)
{
    EXPECT_THROW_MSG(throw_exception(), std::exception, "test");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw_exception(), std::logic_error, "test"),
                            "Expected: throw_exception() throws an exception of "
                            "type std::logic_error.\n  Actual: it throws a different type.");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(do_nothing(), std::exception, "test"),
                            "Expected: do_nothing() throws an exception of type std::exception.\n"
                            "  Actual: it throws nothing.");
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw_exception(), std::exception, "other"),
                            "throw_exception() throws an exception with a different message.\n"
                            "Expected: other\n"
                            "  Actual: test");
}

// Tests EXPECT_SYSTEM_ERROR.
TEST(gtest_extra_test, expect_system_error)
{
    EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, "test");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw_exception(), EDOM, "test"),
                            "Expected: throw_exception() throws an exception of "
                            "type std::system_error.\n  Actual: it throws a different type.");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(do_nothing(), EDOM, "test"),
                            "Expected: do_nothing() throws an exception of type std::system_error.\n"
                            "  Actual: it throws nothing.");
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, "other"),
                            fmt::format("throw_system_error() throws an exception with a different message.\n"
                                        "Expected: {}\n"
                                        "  Actual: {}",
                                        system_error_message(EDOM, "other"), system_error_message(EDOM, "test")));
}

TEST(gtest_extra_test, expect_throw_msg_streaming)
{
    EXPECT_THROW_MSG(throw_exception(), std::exception, "test") << "unexpected failure";
    EXPECT_NONFATAL_FAILURE(EXPECT_THROW_MSG(throw_exception(), std::exception, "other") << "expected failure",
                            "expected failure");
}

TEST(gtest_extra_test, expect_system_error_streaming)
{
    EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, "test") << "unexpected failure";
    EXPECT_NONFATAL_FAILURE(EXPECT_SYSTEM_ERROR(throw_system_error(), EDOM, "other") << "expected failure",
                            "expected failure");
}

#if FMT_USE_FCNTL

using fmt::buffered_file;
using fmt::file;

TEST(output_redirect_test, scoped_redirect)
{
    file read_end, write_end;
    file::pipe(read_end, write_end);
    {
        buffered_file file(write_end.fdopen("w"));
        std::fprintf(file.get(), "[[[");
        {
            output_redirect redir(file.get());
            std::fprintf(file.get(), "censored");
        }
        std::fprintf(file.get(), "]]]");
    }
    EXPECT_READ(read_end, "[[[]]]");
}

// Test that output_redirect handles errors in flush correctly.
TEST(output_redirect_test, flush_error_in_ctor)
{
    file read_end, write_end;
    file::pipe(read_end, write_end);
    int write_fd = write_end.descriptor();
    file write_copy = write_end.dup(write_fd);
    buffered_file f = write_end.fdopen("w");
    // Put a character in a file buffer.
    EXPECT_EQ('x', fputc('x', f.get()));
    FMT_POSIX(close(write_fd));
    std::unique_ptr<output_redirect> redir{nullptr};
    EXPECT_SYSTEM_ERROR_NOASSERT(redir.reset(new output_redirect(f.get())), EBADF, "cannot flush stream");
    redir.reset(nullptr);
    write_copy.dup2(write_fd); // "undo" close or dtor will fail
}

TEST(output_redirect_test, dup_error_in_ctor)
{
    buffered_file f = open_buffered_file();
    int fd = (f.fileno)();
    file copy = file::dup(fd);
    FMT_POSIX(close(fd));
    std::unique_ptr<output_redirect> redir{nullptr};
    EXPECT_SYSTEM_ERROR_NOASSERT(redir.reset(new output_redirect(f.get())), EBADF,
                                 fmt::format("cannot duplicate file descriptor {}", fd));
    copy.dup2(fd); // "undo" close or dtor will fail
}

TEST(output_redirect_test, restore_and_read)
{
    file read_end, write_end;
    file::pipe(read_end, write_end);
    buffered_file file(write_end.fdopen("w"));
    std::fprintf(file.get(), "[[[");
    output_redirect redir(file.get());
    std::fprintf(file.get(), "censored");
    EXPECT_EQ("censored", redir.restore_and_read());
    EXPECT_EQ("", redir.restore_and_read());
    std::fprintf(file.get(), "]]]");
    file = buffered_file();
    EXPECT_READ(read_end, "[[[]]]");
}

// Test that OutputRedirect handles errors in flush correctly.
TEST(output_redirect_test, flush_error_in_restore_and_read)
{
    file read_end, write_end;
    file::pipe(read_end, write_end);
    int write_fd = write_end.descriptor();
    file write_copy = write_end.dup(write_fd);
    buffered_file f = write_end.fdopen("w");
    output_redirect redir(f.get());
    // Put a character in a file buffer.
    EXPECT_EQ('x', fputc('x', f.get()));
    FMT_POSIX(close(write_fd));
    EXPECT_SYSTEM_ERROR_NOASSERT(redir.restore_and_read(), EBADF, "cannot flush stream");
    write_copy.dup2(write_fd); // "undo" close or dtor will fail
}

TEST(output_redirect_test, error_in_dtor)
{
    file read_end, write_end;
    file::pipe(read_end, write_end);
    int write_fd = write_end.descriptor();
    file write_copy = write_end.dup(write_fd);
    buffered_file f = write_end.fdopen("w");
    std::unique_ptr<output_redirect> redir(new output_redirect(f.get()));
    // Put a character in a file buffer.
    EXPECT_EQ('x', fputc('x', f.get()));
    EXPECT_WRITE(
        stderr,
        {
            // The close function must be called inside EXPECT_WRITE,
            // otherwise the system may recycle closed file descriptor when
            // redirecting the output in EXPECT_STDERR and the second close
            // will break output redirection.
            FMT_POSIX(close(write_fd));
            SUPPRESS_ASSERT(redir.reset(nullptr));
        },
        system_error_message(EBADF, "cannot flush stream"));
    write_copy.dup2(write_fd); // "undo" close or dtor of buffered_file will fail
}

#endif // FMT_USE_FCNTL

#endif // FMT_USE_FCNTL