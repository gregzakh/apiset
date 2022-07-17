#include <cstdio>
#include "apiset.hpp"

int main(void) {
  PAPI_SET_NAMESPACE aset{};
  UNICODE_STRING desc{};
  ULONG_PTR next = getapisethead(aset);

  auto entry = reinterpret_cast<PAPI_SET_NAMESPACE_ENTRY>(aset->EntryOffset + next);
  auto u_str = [&desc, next](const ULONG length, const ULONG offset) {
    desc.Length = length;
    desc.MaximumLength = static_cast<USHORT>(length + sizeof(WCHAR));
    desc.Buffer = reinterpret_cast<PWSTR>(offset + next);
  };

  for (ULONG i = 0; i < aset->Count; i++) {
    u_str(entry->NameLength, entry->NameOffset);
    printf("%58wZ.dll | %5s | ", &desc,
          (entry->Flags & API_SET_SCHEMA_ENTRY_FLAGS_SEALED) != 0 ? "true" : "false"
    );

    auto value = reinterpret_cast<PAPI_SET_VALUE_ENTRY>(entry->ValueOffset + next);
    for (ULONG j = 0; j < entry->ValueCount; j++) {
      u_str(value->ValueLength, value->ValueOffset);
      printf("%wZ", &desc);
      if ((j + 1) != entry->ValueCount) printf(", ");
      value++;
    }

    printf("\n");
    entry++;
  }

  return 0;
}
