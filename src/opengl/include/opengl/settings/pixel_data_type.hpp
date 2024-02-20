#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_PIXEL_DATA_TYPE_HPP_
#define ENGINE_OPENGL_SETTINGS_PIXEL_DATA_TYPE_HPP_

namespace Engine::OpenGL::Settings
{
    enum class EPixelDataType
    {
        Byte                       = 0x1400,
        UnsignedByte               = 0x1401,
        Bitmap                     = 0x1A00,
        Short                      = 0x1402,
        UnsignedShort              = 0x1403,
        Int                        = 0x1404,
        UnsignedInt                = 0x1405,
        Float                      = 0x1406,
        UnsignedByte_3_3_2         = 0x8032,
        UnsignedByte_2_3_3_Rev     = 0x8362,
        UnsignedShort_5_6_5        = 0x8363,
        UnsignedShort_5_6_5_Rev    = 0x8364,
        UnsignedShort_4_4_4_4      = 0x8033,
        UnsignedShort_4_4_4_4_Rev  = 0x8365,
        UnsignedShort_5_5_5_1      = 0x8034,
        UnsignedShort_1_5_5_5_Rev  = 0x8366,
        UnsignedInt_8_8_8_8        = 0x8035,
        UnsignedInt_8_8_8_8_Rev    = 0x8367,
        UnsignedInt_10_10_10_2     = 0x8036,
        UnsignedInt_2_10_10_10_Rev = 0x8368
    };
}

#endif // ENGINE_OPENGL_SETTINGS_PIXEL_DATA_TYPE_HPP_
