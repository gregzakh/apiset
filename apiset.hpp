#pragma once

#ifndef UNICODE
  #define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winternl.h> // PPEB

#pragma comment (lib, "ntdll.lib")

extern "C" PPEB RtlGetCurrentPeb();

constexpr ULONG API_SET_SCHEMA_ENTRY_FLAGS_SEALED = 1;

struct API_SET_NAMESPACE {
  ULONG Version;
  ULONG Size;
  ULONG Flags;
  ULONG Count;
  ULONG EntryOffset;
  ULONG HashOffset;
  ULONG HashFactor;
};

struct API_SET_NAMESPACE_ENTRY {
  ULONG Flags;
  ULONG NameOffset;
  ULONG NameLength;
  ULONG HashedLength;
  ULONG ValueOffset;
  ULONG ValueCount;
};

struct API_SET_VALUE_ENTRY {
  ULONG Flags;
  ULONG NameOffset;
  ULONG NameLength;
  ULONG ValueOffset;
  ULONG ValueLength;
};

using PAPI_SET_NAMESPACE = API_SET_NAMESPACE*;
using PAPI_SET_NAMESPACE_ENTRY = API_SET_NAMESPACE_ENTRY*;
using PAPI_SET_VALUE_ENTRY = API_SET_VALUE_ENTRY*;

auto getapisethead(PAPI_SET_NAMESPACE& api) -> ULONG_PTR {
  api = static_cast<PAPI_SET_NAMESPACE>((RtlGetCurrentPeb())->Reserved9[0]);
  return reinterpret_cast<ULONG_PTR>(api);
}
