#include "Ident.h"

Ident::Ident()
        : data{st_cMagic0,
               st_cMagic1,
               st_cMagic2,
               st_cMagic3,
               static_cast<uint8_t>(eElfIdentClass_t::ELFCLASS32),
               static_cast<uint8_t>(eElfIdentData_t::ELFDATA2LSB),
               static_cast<uint8_t>(eElfIdentVersion_t::EV_CURRENT),
               static_cast<uint8_t>(eElfIdentOsABI_t::ELFOSABI_LINUX),
               static_cast<uint8_t>(eElfIdentOsABIVersion_t::EIOSABI_UNSPECIFIED),
               0U,
               0U,
               0U,
               0U,
               0U,
               0U,
               0U} {
}
