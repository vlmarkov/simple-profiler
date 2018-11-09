#include <include/i-factory.hpp>
#include <include/i-profiler.hpp>


class PerfomanceFactory : public IFactory
{
    public:
        PerfomanceFactory()  = default;
        ~PerfomanceFactory() = default;

        std::shared_ptr<IProfiler> createProfiler(IFactoryType::type) override;
};
