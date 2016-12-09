#include "SpaceCommandFactory.hpp"
namespace Space
{
const CommandGroupKey& CommandFactoryName(){
    static const CommandGroupKey key{"SpaceApplicationPlugin"};
    return key;
}
}

