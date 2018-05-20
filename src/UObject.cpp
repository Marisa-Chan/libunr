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

//#include "FLog.h"
#include "Memory.h"
#include "UObject.h"
#include "UClass.h"
#include "UProperty.h"
#include "UPackage.h"

UClass* UObject::StaticAllocateClass( u32 Flags )
{
  return new UClass( Flags );
}

FPackageFileIn& operator>>( FPackageFileIn& Ar, UObject& Obj )
{
  Obj.LoadFromPackage( Ar );
  return Ar;
}

FPackageFileOut& operator<<( FPackageFileOut& Ar, UObject& Obj )
{
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

void UObject::LoadFromPackage( FPackageFileIn& Ar )
{
}

void UObject::SetPkgProperties( UPackage* InPkg, int InExpIdx, int InNameIdx )
{
  Pkg = InPkg;
  ExpIdx = InExpIdx;
  NameIdx = InNameIdx;
}

void UObject::ReadProperties( FPackageFileIn& Ar )
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
        //Log->Print( LOG_WARN, "UStringProperty serialization unimplemented." );
        break;
      case PROP_Class:
        Prop = new UClassProperty();
        //Log->Print( LOG_WARN, "UClassProperty serialization unimplemented." );
        break;
      case PROP_Array:
        Prop = new UArrayProperty();
        //Log->Print( LOG_WARN, "UArrayProperty serialization unimplemented." );
        break;
      case PROP_Struct:
        Prop = new UStructProperty();
        //Log->Print( LOG_WARN, "UStructProperty serialization unimplemented.");
        break;
      case PROP_Vector:
        Prop = new UVectorProperty();
        //Log->Print( LOG_WARN, "UVectorProperty serialization unimplemented.");
        break;
      case PROP_Rotator:
        Prop = new URotatorProperty();
        //Log->Print( LOG_WARN, "URotatorProperty serialization unimplemented.");
        break;
      case PROP_Ascii:
        Prop = new UAsciiStrProperty();
        AsciiStr = (UAsciiStrProperty*)Prop;
        
        Ar >> CINDEX( AsciiStr->Length );
        AsciiStr->Length++;
        
        AsciiStr->Value = xstl::Malloc( AsciiStr->Length );
        StrPtr = (char*)AsciiStr->Value;
        for (int i = 0; i < AsciiStr->Length; i++) {
          Ar.Read( &C, 1 );
          if (C == '\0' && i != (AsciiStr->Length - 1)) {
            //Log->Print( LOG_WARN, "Written length does not match actual length!");
            AsciiStr->Length = 0;
            AsciiStr->Value  = NULL;
          }
          *StrPtr++ = C;
        }
        break;
      case PROP_Map:
        Prop = new UMapProperty();
        //Log->Print( LOG_WARN, "UMapProperty serialization unimplemented." );
        break;
      case PROP_FixArr:
        Prop = new UFixedArrayProperty();
        //Log->Print( LOG_WARN, "UFixedArrayProperty serialization unimplemented." );
        break;
      default:
        //Log->Print( LOG_WARN, "Bad property type!" );
        return;
    }
  }
}
