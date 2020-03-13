/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * TQueue.h - Template queue class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FMacro.h"

#include <queue>

template<class T> class TQueue
{
public:
  TQueue<T>()
  {
    Num = 0;
  }

  TQueue<T>( const TQueue<T>& Q ) 
  {
    if ( Q.Num )
    {
      for ( TQueueNode* Node = Q.FrontNode; Node != NULL; Node = Node->Next )
        Push( Node->Data );
    }
  }

  TQueue<T>( TQueue<T>&& Q )
  {
    FrontNode = Q.FrontNode;
    BackNode = Q.BackNode;
    Num = Q.Num;

    Q.Num = 0;
  }

  ~TQueue<T>()
  {
    while ( Num )
      Pop();
  }

 bool IsEmpty() const 
 { 
   return (Num == 0);
 }

 size_t Size() const    
 { 
   return Num;
 }

 T& Front()         
 { 
   return FrontNode->Data; 
 }

 T& Back()          
 { 
   return BackNode->Data; 
 }

 const T& Front() const 
 { 
   return FrontNode->Data;
 }

 const T& Back() const  
 { 
   return BackNode->Data;
 }

 void Pop()             
 { 
   if ( FrontNode == NULL )
     return;

   TQueueNode* OutNode = FrontNode;
   if ( FrontNode == BackNode )
   {
     FrontNode = NULL;
     BackNode = NULL;
   }
   else
   {
     FrontNode = FrontNode->Next;
   }

   Num--;
   delete OutNode;
 }

 void Push( const T& Value )
 { 
   TQueueNode* NewNode = new TQueueNode( Value );
   Num++;

   if ( FrontNode == NULL )
   {
     FrontNode = NewNode;
     BackNode = NewNode;
   }
   else if ( FrontNode == BackNode )
   {
     FrontNode->Next = NewNode;
     BackNode = NewNode;
   }
   else
   {
     BackNode->Next = NewNode;
     BackNode = NewNode;
   }
 }

 void Swap( TQueue<T>& x )
 { 
   TQueueNode* TmpFront = FrontNode;
   TQueueNode* TmpBack = BackNode;
   int TmpNum = Num;

   FrontNode = x.FrontNode;
   BackNode = x.BackNode;
   Num = x.Num;

   x.FrontNode = TmpFront;
   x.BackNode = TmpBack;
   x.Num = TmpNum;
 }

protected:
  struct TQueueNode
  {
    TQueueNode( const T& Value )
    {
      Data = Value;
      Next = NULL;
    }

    TQueueNode* Next;
    T Data;

  private:
    TQueueNode() {}
  };

  TQueueNode* FrontNode;
  TQueueNode* BackNode;
  int Num;
};

