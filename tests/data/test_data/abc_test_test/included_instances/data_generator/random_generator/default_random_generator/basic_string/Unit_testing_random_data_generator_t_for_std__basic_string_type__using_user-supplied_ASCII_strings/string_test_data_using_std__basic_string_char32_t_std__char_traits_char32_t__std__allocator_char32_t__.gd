#class std::tuple<unsigned __int64,unsigned __int64,class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> > >
(0,0,"")
(0,1,"")
# First rng() call is for size. Second is for value.
# Last char size = 1.
(1,1,"\x00000002")
(2,1,"")
(5,5,"\x00000006\x00000007\x00000008\x00000009\x0000000A")
# Size = 1000 % X = 4. 
(1000,5,"\x000003E9\x000003EA\x000003EB\x000003EC")
# Size 4. first 3 are last char32_t before 0xd800
(55294,5,"\x0000D7FF\x0000E000\x0000E001\x0000E002")
# Size = 7. 55296 = 0xE000, becomes EE 80 80. Next is EE 80 81. Final char8_t = 55298%128=2
(55295,7,"\x0000E000\x0000E001\x0000E002\x0000E003\x0000E004\x0000E005\x0000E006")
# 1000000%(13+1) = 8. 1000001=F4A41. In binary 0 1111 0100 1010 0100 0001. Beware many online calculators get conversion wrong.
(1000000,13,"\x000F4A41\x000F4A42\x000F4A43\x000F4A44\x000F4A45\x000F4A46\x000F4A47\x000F4A48")
# Size 10.
(1000000,14,"\x000F4A41\x000F4A42\x000F4A43\x000F4A44\x000F4A45\x000F4A46\x000F4A47\x000F4A48\x000F4A49\x000F4A4A")
# Size = 11. In this example size limits the last character created to 1 char16_t. It creates 47687, or BA47 in hex. 
(1000001,14,"\x000F4A42\x000F4A43\x000F4A44\x000F4A45\x000F4A46\x000F4A47\x000F4A48\x000F4A49\x000F4A4A\x000F4A4B\x000F4A4C")
# This one tests that the very last valid UTF32 character can be created. Size = 6. The last element has to be of size 1, so the limit is set to 65535. This sets the created value to 32771, or 0x8003
(1112062,10,"\x0010FFFF\x00000000\x00000001\x00000002\x00000003\x00000004")