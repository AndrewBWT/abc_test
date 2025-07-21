#class std::tuple<unsigned __int64,unsigned __int64,class std::basic_string<char8_t,struct std::char_traits<char8_t>,class std::allocator<char8_t> > >
(0,0,"")
(0,1,"")
# First rng() call is for size. Second is for value.
# Last char size = 1.
(1,1,"\x02")
(2,1,"")
(5,5,"\x06\x07\x08\x09\x0A")
# Size = 1000 % X = 4. 
(1000,5,"\xCF\xA9\xCF\xAA")
# 4 char8ts. first 3 are last char32_t before 0xd800. last bit is a single valid char8_t character. rng is 55296%128=0
(55294,5,"\xED\x9F\xBF\x00")
# Size = 7. 55296 = 0xE000, becomes EE 80 80. Next is EE 80 81. Final char8_t = 55298%128=2
(55295,7,"\xEE\x80\x80\xEE\x80\x81\x02")
# 1000000%(13+1) = 8. 1000001=F4A41. In binary 0 1111 0100 1010 0100 0001. Beware many online calculators get conversion wrong.
(1000000,13,"\xF3\xB4\xA9\x81\xF3\xB4\xA9\x82")
# In this example size limits the last character created to 2 bytes. It creates 579, or 243 in hex. 
(1000000,14,"\xF3\xB4\xA9\x81\xF3\xB4\xA9\x82\xC9\x83")
# In this example size limits the last character created to 3 bytes. It creates 47684, or BA44 in hex. 
(1000001,14,"\xF3\xB4\xA9\x82\xF3\xB4\xA9\x83\xEB\xA9\x84")
# This one tests that the very last valid UTF32 character can be created.
(1112062,10,"\xF4\x8F\xBF\xBF\x00\x01")