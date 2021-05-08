#pragma once

#include <cstddef> // size_t
#include <cstdint> // uint32_t

#define MAX_FILE_NAME 20U

enum class CmdType : uint8_t
{
    NoType   = 0x00U,
    Upload   = 0x01U,
    Download = 0x02U,
    NotFound = 0x03U,
    Ready    = 0x04U
};

struct Msg
{
    CmdType type;
};

struct MsgUpload
{
    CmdType type;
    size_t size;
    char name[MAX_FILE_NAME];
};

struct MsgDownload
{
    CmdType type;
    char name[MAX_FILE_NAME];
};
