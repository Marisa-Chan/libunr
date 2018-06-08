/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * UProperty.cpp - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UProperty.h"

u8 UProperty::PropertySizes[8] = { 1, 2, 4, 12, 16, 1, 2, 4 };

UProperty::UProperty()
{
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
  Value = 0;
}

UProperty::UProperty( int InNameIdx )
{
  NameIdx = InNameIdx;
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
  Value = 0;
}

UProperty::~UProperty()
{
  if ( LIKELY( PropNext ) )
    delete PropNext;
}

UProperty* UProperty::LoadProperty( FPackageFileIn& In )
{
}

FPropertyList::FPropertyList( UProperty* InProp )
  : Prop( InProp )
{
}

FPropertyList::~FPropertyList()
{
}

FPropertyHash::FPropertyHash()
{
  HashArray = NULL;
}

FPropertyHash::~FPropertyHash()
{
  if ( LIKELY( HashArray ) )
    delete HashArray;
}

void FPropertyHash::LoadPropertyList( FPackageFileIn& Ar )
{
  
//   int Name;
//   int PrevName;
//   const char* NameStr;
//   UProperty* Prop = NULL;
//   while( 1 )
//   {
//     // Get the name
//     Ar >> CINDEX( Name );
//     NameStr = Pkg->ResolveNameFromIdx( Name );
//     
//     // Finished when we hit None
//     if ( strncmp( NameStr, "None", 4 ) == 0 )
//       break;
//     
//     // Get info
//     u8 Info;
//     Ar >> Info;
//     EPropertyType Type = (EPropertyType)(Info & 0xF);
//     u8 Size = UProperty::PropertySizes[Info & 0x70];
//     bool Bit7 = Info & 0x80;
//     
//     if ( !Prop || PrevName != Name )
//     {
//       Prop = new UProperty( Name );
//     }
//     else
//     {
//       Prop->ArrayDim++;
//       if ( Prop->ArrayDim == 2 )
//         bCreateArray = true;
//     }
//     
//     switch( Type )
//     {
//       case PROP_Byte:
//         LoadByteProperty( Ar, Prop, bCreateArray );
//         break;
//       case PROP_Int:
//         LoadIntProperty( Ar, Prop, bCreateArray );
//         break;
//       case PROP_Bool:
//         Prop->Value = (u64)Bit7;
//         break;
//       case PROP_Float:
//         LoadFloatProperty( Ar, Prop, bCreateArray );
//         break;
//       case PROP_Object:
//         LoadObjectProperty( Ar, Prop, bCreateArray );
//         break;
//       case PROP_Name:
//         Ar >> CINDEX( *((int*)Prop->Value) );
//         break;
//       case PROP_String:
//         Logf( LOG_WARN, "UStringProperty serialization unimplemented." );
//         break;
//       case PROP_Class:
//         Logf( LOG_WARN, "UClassProperty serialization unimplemented." );
//         break;
//       case PROP_Array:
//         Logf( LOG_WARN, "UArrayProperty serialization unimplemented." );
//         break;
//       case PROP_Struct:
//         Logf( LOG_WARN, "UStructProperty serialization unimplemented.");
//         break;
//       case PROP_Vector:
//         Logf( LOG_WARN, "UVectorProperty serialization unimplemented.");
//         break;
//       case PROP_Rotator:
//         Logf( LOG_WARN, "URotatorProperty serialization unimplemented.");
//         break;
//       case PROP_Ascii:
//         Logf( LOG_WARN, "UAsciiStrPropertty serialization unimplemented.");
// //           Ar >> CINDEX( AsciiStr->Length );
// //           AsciiStr->Length++;
// //           
// //           AsciiStr->Value = xstl::Malloc( AsciiStr->Length );
// //           StrPtr = (char*)AsciiStr->Value;
// //           for (int i = 0; i < AsciiStr->Length; i++) {
// //             Ar.Read( &C, 1 );
// //             if (C == '\0' && i != (AsciiStr->Length - 1)) {
// //               Logf( LOG_WARN, "Written length does not match actual length!");
// //               AsciiStr->Length = 0;
// //               AsciiStr->Value  = NULL;
// //             }
// //             *StrPtr++ = C;
// //           }
//         break;
//       case PROP_Map:
//         Logf( LOG_WARN, "UMapProperty serialization unimplemented." );
//         break;
//       case PROP_FixArr:
//         Logf( LOG_WARN, "UFixedArrayProperty serialization unimplemented." );
//         break;
//       default:
//         Logf( LOG_WARN, "Bad property type!" );
//         return;
//     }
// 
//     PrevName = Name;
//     bCreateArray = false;
//   }
}
