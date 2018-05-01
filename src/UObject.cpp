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
 * UObject.cpp - Generic object functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FMemory.h"
#include "UObject.h"
#include "UClass.h"
#include "UProperty.h"
#include "UPackage.h"

UClass* UObject::StaticAllocateClass( u32 Flags )
{
  return new UClass( Flags );
}

FArchive& operator>>( FArchive& Ar, UObject& Obj )
{
  Obj.LoadFromPackage( Ar );
  return Ar;
}

UObject::UObject()
{
}

UObject::~UObject()
{
}

bool UObject::ExportToFile()
{
  return false;
}

void UObject::LoadFromPackage( FArchive& Ar )
{
}

void UObject::SetPkgProperties( UPackage* InPkg, int InExpIdx, int InNameIdx )
{
  Pkg = InPkg;
  ExpIdx = InExpIdx;
  NameIdx = InNameIdx;
}

void UObject::ReadProperties( FArchive& Ar )
{
  char C;
  char* StrPtr;
  UAsciiStrProperty* AsciiStr;
  
  while( 1 )
  {
    // Get the name
    int Name;
    const char* NameStr;
    Ar >> CINDEX( Name );
    NameStr = Pkg->ResolveNameFromIdx( Name );
    
    // Finished when we hit None
    if ( strncmp( NameStr, "None", 4 ) == 0 )
      break;
    
    // Get info
    u8 Info;
    Ar >> Info;
    EPropertyType Type = (EPropertyType)(Info & 0xF);
    u8 Size = UProperty::PropertySizes[Info & 0x70];
    bool Bit7 = Info & 0x80;
    
    // Get property value
    UProperty* Prop;
    switch( Type )
    {
      case PROP_Byte:
        Prop = new UByteProperty();
        Ar >> *((u8*)Prop->Value);
        break;
      case PROP_Int:
        Prop = new UIntProperty();
        Ar >> *((int*)Prop->Value);
        break;
      case PROP_Bool:
        Prop = new UBoolProperty();
        Prop->Value = (void*)Bit7;
        break;
      case PROP_Float:
        Prop = new UFloatProperty();
        Ar >> *((float*)Prop->Value);
        break;
      case PROP_Object:
        Prop = new UObjectProperty();
        Ar >> CINDEX( *((int*)Prop->Value) );
        break;
      case PROP_Name:
        Prop = new UNameProperty();
        Ar >> CINDEX( *((int*)Prop->Value) );
        break;
      case PROP_String:
        Prop = new UStringProperty();
        printf("UStringProperty serialization unimplemented.\n");
        break;
      case PROP_Class:
        Prop = new UClassProperty();
        printf("UClassProperty serialization unimplemented.\n");
        break;
      case PROP_Array:
        Prop = new UArrayProperty();
        printf("UArrayProperty serialization unimplemented.\n");
        break;
      case PROP_Struct:
        Prop = new UStructProperty();
        printf("UStructProperty serialization unimplemented.\n");
        break;
      case PROP_Vector:
        Prop = new UVectorProperty();
        printf("UVectorProperty serialization unimplemented.\n");
        break;
      case PROP_Rotator:
        Prop = new URotatorProperty();
        printf("URotatorProperty serialization unimplemented.\n");
        break;
      case PROP_Ascii:
        Prop = new UAsciiStrProperty();
        AsciiStr = (UAsciiStrProperty*)Prop;
        
        Ar >> CINDEX( AsciiStr->Length );
        AsciiStr->Length++;
        
        AsciiStr->Value = FMemory::Malloc( AsciiStr->Length );
        StrPtr = (char*)AsciiStr->Value;
        for (int i = 0; i < AsciiStr->Length; i++) {
          Ar.Read( &C, 1 );
          if (C == '\0' && i != (AsciiStr->Length - 1)) {
            printf("Written length does not match actual length!\n");
            AsciiStr->Length = 0;
            AsciiStr->Value  = NULL;
          }
          *StrPtr++ = C;
        }
        break;
      case PROP_Map:
        Prop = new UMapProperty();
        printf("UMapProperty serialization unimplemented.\n");
        break;
      case PROP_FixArr:
        Prop = new UFixedArrayProperty();
        printf("UFixedArrayProperty serialization unimplemented.\n");
        break;
      default:
        printf("Bad property type!\n");
        return;
    }
  }
}
