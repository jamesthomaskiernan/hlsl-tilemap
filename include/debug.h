#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <d3dcompiler.h>

void Debug(HRESULT error, const std::string &msg);

void Debug(ID3DBlob* blob, const std::string &msg);

void Debug(const std::string &msg);