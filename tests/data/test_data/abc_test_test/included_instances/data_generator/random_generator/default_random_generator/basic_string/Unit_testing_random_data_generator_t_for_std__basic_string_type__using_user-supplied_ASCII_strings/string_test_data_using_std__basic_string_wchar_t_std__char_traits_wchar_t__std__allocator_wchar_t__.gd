#class std::tuple<unsigned __int64,unsigned __int64,class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> > >
(0,0,"")
(0,1,"")
# First rng() call is for size. Second is for value.
# Last char size = 1.
(1,1,"\x0002")
(2,1,"")
(5,5,"\x0006\x0007\x0008\x0009\x000A")
# Size = 1000 % X = 4. 
(1000,5,"\x03E9\x03EA\x03EB\x03EC")
# Size 4. first 3 are last char32_t before 0xd800. last bit is a single valid char8_t character. rng is 55296%128=0
(55294,5,"\xD7FF\xE000\xE001\xE002")
# Size = 7. 55296 = 0xE000, becomes EE 80 80. Next is EE 80 81. Final char8_t = 55298%128=2
(55295,7,"\xE000\xE001\xE002\xE003\xE004\xE005\xE006")
# 1000000%(13+1) = 8. 1000001=F4A41. In binary 0 1111 0100 1010 0100 0001. Beware many online calculators get conversion wrong.
(1000000,13,"\xDB92\xDE41\xDB92\xDE42\xDB92\xDE43\xDB92\xDE44")
# Size 10.
(1000000,14,"\xDB92\xDE41\xDB92\xDE42\xDB92\xDE43\xDB92\xDE44\xDB92\xDE45")
# Size = 11. In this example size limits the last character created to 1 char16_t. It creates 47687, or BA47 in hex. 
(1000001,14,"\xDB92\xDE42\xDB92\xDE43\xDB92\xDE44\xDB92\xDE45\xDB92\xDE46\xBA47")
# This one tests that the very last valid UTF32 character can be created. Size = 6. The last element has to be of size 1, so the limit is set to 65535. This sets the created value to 32771, or 0x8003
(1112062,10,"\xDBFF\xDFFF\x0000\x0001\x0002\x8003")