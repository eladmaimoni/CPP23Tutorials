
#include <catch2/catch.hpp>
#include <debugger_trace.hpp>
#include <vector>


class NoisyResource
{
private:
    uint8_t* _ptr{ nullptr };
    std::size_t _byteSize{0};

public:
    NoisyResource() = default;
    NoisyResource(std::size_t byteSize)
        :
        _ptr(new uint8_t[byteSize]),
        _byteSize(byteSize)

    {
        DEBUGGER_TRACE("NoisyResource::NoisyResource({})", _byteSize);
    }

    ~NoisyResource()
    {
        delete[] _ptr;
        DEBUGGER_TRACE("NoisyResource::~NoisyResource() deallocated {} bytes", _byteSize);
    }
 
    NoisyResource(const NoisyResource& that)
        :
        _ptr(new uint8_t[that._byteSize]), // allocate
        _byteSize(that._byteSize)
    {
        std::copy(that._ptr, that._ptr + _byteSize, _ptr); // assign
        DEBUGGER_TRACE("NoisyResource::NoisyResource(const NoisyResource& that)");    
    }

    NoisyResource& operator=(const NoisyResource& rhs)
    {
        // allocate
        auto ptr = new uint8_t[rhs._byteSize];

        // destruct
        delete _ptr;

        // assign
        _ptr = ptr;
        _byteSize = rhs._byteSize;
        std::copy(rhs._ptr, rhs._ptr + _byteSize, _ptr); // assign

        DEBUGGER_TRACE("NoisyResource::operator=(const NoisyResource& rhs)");

        return *this;
    }

    NoisyResource(NoisyResource&& that)
        :
        _ptr(that._ptr), 
        _byteSize(that._byteSize)
    {
        // transfer resource ownership
        that._ptr = nullptr;
        that._byteSize = 0;

        DEBUGGER_TRACE("NoisyResource::NoisyResource(NoisyResource&& that)");
    }

    NoisyResource& operator=(NoisyResource&& rhs)
    {
        _ptr = rhs._ptr;
        _byteSize = rhs._byteSize;

        // transfer resource ownership
        rhs._ptr = nullptr;
        rhs._byteSize = 0;

        DEBUGGER_TRACE("NoisyResource::operator=(NoisyResource&& rhs)");
        return *this;
    }
};


TEST_CASE("1 - basic raii", "[raii]")
{
    DEBUGGER_TRACE("start");

    {
        NoisyResource resource(1024);
        DEBUGGER_TRACE("created resource");
    }

    DEBUGGER_TRACE("done");

}

TEST_CASE("2 - copy constructor", "[raii]")
{
    DEBUGGER_TRACE("start");

    {
        NoisyResource resource1(1024);
        DEBUGGER_TRACE("created resource1");

        NoisyResource resource2(resource1);
        DEBUGGER_TRACE("created resource2");

    }

    DEBUGGER_TRACE("done");

}

TEST_CASE("3 - copy assignment", "[raii]")
{
    DEBUGGER_TRACE("start");
    NoisyResource resource2;
    {
        NoisyResource resource1(1024);
        DEBUGGER_TRACE("created resource1");

        resource2 = resource1;
        DEBUGGER_TRACE("assined to resource2");

    }

    DEBUGGER_TRACE("done");
}

TEST_CASE("4 - move constructor", "[raii]")
{
    DEBUGGER_TRACE("start");

    {
        NoisyResource resource1(1024);
        DEBUGGER_TRACE("created resource1");

        NoisyResource resource2(std::move(resource1));
        DEBUGGER_TRACE("created resource2");
    }

    DEBUGGER_TRACE("done");

}

TEST_CASE("5 - move assignment", "[raii]")
{
    DEBUGGER_TRACE("start");
    NoisyResource resource2;
    {
        NoisyResource resource1(1024);
        DEBUGGER_TRACE("created resource1");

        resource2 = std::move(resource1);
        DEBUGGER_TRACE("assigned to resource2");

    }

    DEBUGGER_TRACE("done");
}

TEST_CASE("6 - pure rvalue 1", "[raii]")
{
    DEBUGGER_TRACE("start");
    NoisyResource resource2;
    {
        resource2 = NoisyResource(1024); // assigning with pure rvalue, no need for move
        DEBUGGER_TRACE("assigned to resource2");
    }

    DEBUGGER_TRACE("done");
}

NoisyResource CreateNoisyResource(std::size_t byteSize)
{
    NoisyResource resource(byteSize);

    DEBUGGER_TRACE("CreateNoisyResource created a resource");

    return resource; // return by value
}

TEST_CASE("7 - pure rvalue 2", "[raii]")
{
    DEBUGGER_TRACE("start");
    NoisyResource resource2;
    {
        resource2 = CreateNoisyResource(1024); // assigning with pure rvalue, no need for move
        DEBUGGER_TRACE("assigned to resource2");
    }

    DEBUGGER_TRACE("done");
}

std::vector<NoisyResource> CreateNoisyResources(std::size_t count, std::size_t byteSize)
{
    std::vector<NoisyResource> resources;
    resources.reserve(count);


    for (auto i = 0u; i < count; ++i)
    {
        resources.emplace_back(byteSize);
    }

    DEBUGGER_TRACE("CreateNoisyResources created a vector of resources");

    return resources; // return by value
}

TEST_CASE("8 - return vector by value", "[raii]")
{
    DEBUGGER_TRACE("start");
  
    {       
        std::vector<NoisyResource> vec3;
        {
            auto vec1 = CreateNoisyResources(3, 1024);

            auto vec2 = vec1;

            vec3 = std::move(vec1);
        }
        DEBUGGER_TRACE("nested scope");

    }

    DEBUGGER_TRACE("done");
}

class SomeHeavyObject
{
    std::vector<int> _vec1;
    std::vector<int> _vec2;
    std::vector<int> _vec3;
    std::vector<int> _vec4;
public:
    SomeHeavyObject()
    {
        _vec1.resize(5120);
        _vec2.resize(5120);
        _vec3.resize(5120);
        _vec4.resize(5120);
    }
};

TEST_CASE("move only object - single scope ownership", "[raii]")
{
    std::unique_ptr< SomeHeavyObject> obj = std::unique_ptr<SomeHeavyObject>();

    DEBUGGER_TRACE("start");
    NoisyResource resource2;
    {
        resource2 = CreateNoisyResource(1024); // assigning with pure rvalue, no need for move
        DEBUGGER_TRACE("assigned to resource2");
    }

    DEBUGGER_TRACE("done");
}

