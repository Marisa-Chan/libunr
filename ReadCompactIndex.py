import sys

def ReadCompactIndex(argv):
  negate = False
  Value = 0

  bytes = argv[1:]
  print(bytes)
  
  if len(bytes) == 0 or len(bytes) > 5:
    print("Invalid")
    return
  
  for i in range(0, 5):
    bytes[i] = "0x" + bytes[i]
    x = int(bytes[i], 16)
    if i == 0:
      if (x & 0x80) > 0:
        negate = True
      Value |= (x & 0x3f)
      if (x & 0x40) == 0:
        break
      
    elif i == 4:
      Value |= (x & 0x1f) << (6 + (3 * 7))
      
    else:
      Value |= (x & 0x7f) << (6 + ((i - 1) * 7))
      if (x & 0x80) == 0:
        break
      
  if negate:
    Value = -Value
    
  print(Value)
  
if __name__ == '__main__':
  ReadCompactIndex(sys.argv)
  
  
