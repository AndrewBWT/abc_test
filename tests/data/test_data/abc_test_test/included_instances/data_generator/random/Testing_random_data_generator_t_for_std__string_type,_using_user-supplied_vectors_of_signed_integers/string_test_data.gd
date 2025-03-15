#class std::tuple<unsigned __int64,unsigned __int64,class std::vector<signed char,class std::allocator<signed char> > >
(0, 0, {})
(1, 0, {})
# Produces {0,1...}. Size of string is decided first, will come out as 0.
(0, 1, {})
# Produces {1..}. Size of string = (1 % (1+1)), character = 2. as lowest char is -128, comes out as -126.
(1,1,{-126})
# Produces {5,6...}. Size of string = 5 % (3+1)) = 1. character = 6. -128+6=-122.
(5,3,{-122})
# Produces {21,22,23...}. Size of string = 21 % (100+1) = 21. Produces -106,-105... -86.
(21,100,{-106,-105,-104,-103,-102,-101,-100,-99,-98,-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86})