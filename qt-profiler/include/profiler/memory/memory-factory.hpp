#include <include/i-factory.hpp>
#include <include/i-profiler.hpp>


class MemoryFactory : public IFactory
{
    public:
        MemoryFactory()  = default;
        ~MemoryFactory() = default;

        std::shared_ptr<IProfiler> createProfiler(const IFactoryType& type) override;
};
