#class std::tuple<class std::vector<unsigned int,class std::allocator<unsigned int> >,unsigned __int64,unsigned __int64>
# First seed = sum[1,2,3] = 6. Taking 6 values = [6,7,8,9,10,11]. Second rng value = sum, 51.
([1,2,3],6,51)
# First seed = 0. Taking 3 values = [0,1,2]=3. Second rng = 3.
([],3,3)
# Taking zero elements, always zero.
([],0,0)
([1],0,0)
#Taking one element, will always be that element.
([4294967295],1,4294967295)
# Taking 2 values will give 4294967295 and 0. New rng value = 4294967295.
([4294967295],2,4294967295)
# Taking 3 values will give [4294967295,0,1]. New rng value = 4294967296, as its based on size_t
([4294967295],3,4294967296)
