#include "utils/exception.h"

#include <cstdlib>
#include <exception>

#include "utils/logger.h"
#include "utils/os.h"

namespace utils {

Exception::Exception(std::string&& from, std::string_view what) :
    std::runtime_error{what.data()},
    from_{std::make_shared<const std::string>(std::move(from))}
{}

const std::string& Exception::from() const
{
    return *from_;
}


void terminate() noexcept
{
    std::string trace;
    std::vector<StackFrame> frames = stacktrace();
    for (size_t i = 0; i < frames.size(); ++i) {
        const StackFrame& f = frames[i];
        trace += fmt::format("\n#{:02d}  {}  {} ({})", i, f.module, f.function, f.address);
        if (!f.file.empty()) {
            trace += fmt::format("  {}", f.file);
            if (f.line)
                trace += fmt::format(":{}", f.line);
        }
    }

    utils::Logger logger{"terminate"};
    try {
        std::exception_ptr ex = std::current_exception();
        if (ex)
            std::rethrow_exception(ex);
        else
            logger(utils::LogLevel::fatal, "no active exception\n", trace);
    }
    catch (const utils::Exception& ex) {
        logger(utils::LogLevel::fatal, "fatal exception: {}\nfrom: {}\n{}", ex.what(), ex.from(), trace);
    }
    catch (const std::exception& ex) {
        logger(utils::LogLevel::fatal, "fatal exception: {}\n{}", ex.what(), trace);
    }
    catch (...) {
        logger(utils::LogLevel::fatal, "fatal exception of unknown type\n{}", trace);
    }

    std::abort();
}

}  // namespace utils
