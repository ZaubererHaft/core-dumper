/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details
 *
 * \author   l.kratzl
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>

using Half = uint16_t;
using Offs = uint32_t;
using Word = uint32_t;
using Addr = uint32_t;

static constexpr uint8_t st_cOffsetHalf = sizeof(Half);
static constexpr uint8_t st_cOffsetWord = sizeof(Word);
static constexpr uint8_t st_cOffsetOffs = sizeof(Offs);
static constexpr uint8_t st_cOffsetAddr = sizeof(Addr);

static constexpr Half st_cElfHeaderSize    = 16U * sizeof(uint8_t) + 2U * sizeof(Word) + 8U * sizeof(Half) + 1U * sizeof(Addr) + 2U * sizeof(Offs);
static constexpr Half st_cSectionHeaderSize = 8U * sizeof(Word) + 1U * sizeof(Addr) + 1U * sizeof(Offs);
static constexpr Half st_cProgramHeaderSize = 5U * sizeof(Word) + 2U * sizeof(Addr) + 1U * sizeof(Offs);



#endif // TYPES_H_

/**
 * \}
 * \}
 */
