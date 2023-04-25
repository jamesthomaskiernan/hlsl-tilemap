#include "debug.h"
#include "comdef.h"
#include "application.h"


void Debug(HRESULT error, const std::string &msg)
{
    if (FAILED(error))
    {
        _com_error comErr = _com_error(error);

        std::cout << std::endl;
        std::cout << "ERROR: " << msg << std::endl;
        std::cout << "HRESULT: " << comErr.ErrorMessage() << std::endl;
        std::cout << std::endl;

        exit(1);
    }
}

void Debug(ID3DBlob* blob, const std::string &msg)
{
    if (blob)
    {
        std::cout << std::endl;
        std::cout << "ERROR: " << msg << std::endl;
        std::cout << "BLOB: " << ((char*)blob->GetBufferPointer());
        std::cout << std::endl;

        exit(1);
    }
}

void Debug(const std::string &msg)
{
    std::cout << std::endl;
    std::cout << "ERROR: " << msg << std::endl;
    std::cout << std::endl;
    
    exit(1);
}