#include "kae/exception.h"

#include <cstdlib>
#include <exception>

#include "kae/logger.h"
#include "kae/os.h"

namespace kae {

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

    kae::Logger logger{"terminate"};
    try {
        std::exception_ptr ex = std::current_exception();
        if (ex)
            std::rethrow_exception(ex);
        else
            logger(kae::LogLevel::fatal, "no active exception\n", trace);
    }
    catch (const kae::Exception& ex) {
        logger(kae::LogLevel::fatal, "fatal exception: {}\nfrom: {}\n{}", ex.what(), ex.from(), trace);
    }
    catch (const std::exception& ex) {
        logger(kae::LogLevel::fatal, "fatal exception: {}\n{}", ex.what(), trace);
    }
    catch (...) {
        logger(kae::LogLevel::fatal, "fatal exception of unknown type\n{}", trace);
    }

    std::abort();
}

}  // namespace kae
