#include "DynamicLibrary.h"

#include <iostream>
#include <dlfcn.h>

DynamicLibrary::DynamicLibrary(const char *fileName)
{
    libHandle = dlopen(NULL, RTLD_NOW);
//    libHandle = dlopen(fileName, RTLD_NOW);
//    const char *strerr = dlerror();
    if (!libHandle)
        throw OpenLibFailedException();
}

DynamicLibrary::~DynamicLibrary()
{
    if (libHandle)
        dlclose(libHandle);
}

void *DynamicLibrary::getFunctionPtr(const char *name) const
{
    auto ret = (void *)dlsym(libHandle, name);
    if (ret == nullptr) {
        std::cerr << "Failed to get function " << name << std::endl;
    }
    return ret;
}

