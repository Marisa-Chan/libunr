/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * UScript.h - UnrealScript definitions
 * See "Script Format" from UT-Package-File-Format.pdf 
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

enum EScriptToken
{
  EX_LocalVariable      = 0x00, EX_InstanceVariable   = 0x01, EX_DefaultVariable    = 0x02,
  EX_Unk03              = 0x03, EX_Return             = 0x04, EX_Switch             = 0x05,
  EX_Jump               = 0x06, EX_JumpIfNot          = 0x07, EX_Stop               = 0x08,
  EX_Assert             = 0x09, EX_Case               = 0x0A, EX_Nothing            = 0x0B,
  EX_LabelTable         = 0x0C, EX_GotoLabel          = 0x0D, EX_EatString          = 0x0E,
  EX_Let                = 0x0F, EX_DynArrayElement    = 0x10, EX_New                = 0x11,
  EX_ClassContext       = 0x12, EX_MetaCast           = 0x13, EX_LetBool            = 0x14,
  EX_Unk15              = 0x15, EX_EndFunctionParms   = 0x16, EX_Self               = 0x17,
  EX_Skip               = 0x18, EX_Context            = 0x19, EX_ArrayElement       = 0x1A,
  EX_VirtualFunction    = 0x1B, EX_FinalFunction      = 0x1C, EX_IntConst           = 0x1D,
  EX_FloatConst         = 0x1E, EX_StringConst        = 0x1F, EX_ObjectConst        = 0x20,
  EX_NameConst          = 0x21, EX_RotationConst      = 0x22, EX_VectorConst        = 0x23, 
  EX_ByteConst          = 0x24, EX_IntZero            = 0x25, EX_IntOne             = 0x26,
  EX_True               = 0x27, EX_False              = 0x28, EX_NativeParm         = 0x29, 
  EX_NoObject           = 0x2A, EX_Unk2b              = 0x2B, EX_IntConstByte       = 0x2C,
  EX_BoolVariable       = 0x2D, EX_DynamicCast        = 0x2E, EX_Iterator           = 0x2F,
  EX_IteratorPop        = 0x30, EX_IteratorNext       = 0x31, EX_StructCmpEq        = 0x32,
  EX_StructCmpNe        = 0x33, EX_UnicodeStringConst = 0x34, EX_Unk35              = 0x35,
  EX_StructMember       = 0x36, EX_Unk37              = 0x37, EX_GlobalFunction     = 0x38,
  EX_RotatorToVector    = 0x39, EX_ByteToInt          = 0x3A, EX_ByteToBool         = 0x3B,
  EX_ByteToFloat        = 0x3C, EX_IntToByte          = 0x3D, EX_IntToBool          = 0x3E,
  EX_IntToFloat         = 0x3F, EX_BoolToByte         = 0x40, EX_BoolToInt          = 0x41,
  EX_BoolToFloat        = 0x42, EX_FloatToByte        = 0x43, EX_FloatToInt         = 0x44,
  EX_FloatToBool        = 0x45, EX_StringToName       = 0x46, EX_ObjectToBool       = 0x47, 
  EX_NameToBool         = 0x48, EX_StringToByte       = 0x49, EX_StringToInt        = 0x4A,
  EX_StringToBool       = 0x4B, EX_StringToFloat      = 0x4C, EX_StringToVector     = 0x4D,
  EX_StringToRotator    = 0x4E, EX_VectorToBool       = 0x4F, EX_VectorToRotator    = 0x50,
  EX_RotatorToBool      = 0x51, EX_ByteToString       = 0x52, EX_IntToString        = 0x53,
  EX_BoolToString       = 0x54, EX_FloatToString      = 0x55, EX_ObjectToString     = 0x56,
  EX_NameToString       = 0x57, EX_VectorToString     = 0x58, EX_RotatorToString    = 0x59,
  EX_Unk5a              = 0x5A, EX_Unk5b              = 0x5B, EX_Unk5c              = 0x5C,
  EX_Unk5d              = 0x5D, EX_Unk5e              = 0x5E, EX_Unk5f              = 0x5F,
  EX_ExtendedNative     = 0x60, EX_FirstNative        = 0x70,
};


