import sys

prime = 1099511628211
basis = 14695981039346656037

collide = 0

def Fnv1a(data):
  value = basis

  for i in range(0, len(data)):
    value ^= ord(data[i])
    value &= 0xffffffffffffffff
    value *= prime
    value &= 0xffffffffffffffff

  return value
  
def NumCollisions(strings):
  hashes = []
  for i in range(0, len(strings)):
    hashes.append(Fnv1a(strings[i]))
    print(strings[i] + " : " + str(hashes[i]))
    for j in range(0, len(hashes)):
      if (hashes[i] == hashes[j]) and i != j:
        print("collision")
        collide += 1
  
if __name__ == '__main__':
  NumCollisions(sys.argv[1:])
  print("num collisions : " + str(collide))
