#class std::tuple<unsigned __int64,unsigned char,unsigned char,unsigned char>
#rng_counter,rng_progress_to,lower_bound,upper_bound,rng_after_running,expected_result
# All of these have the same bounds, so only one element can be generated.
(0, 0, 50, 50, 0, 50)
(1, 0, 25, 25, 0, 25)
(0, 0, 115, 115, 0, 115)
(1, 0, 255, 255, 0, 255)
(0, 0, 115, 115, 0, 115)
# denominator of 1. rng returns 0.
(1, 0, 115, 114, 1, 114)
# denominator of 1. rnd() returns 1.
(1, 1, 115, 114, 1, 115)
# denominator of 1. rnd() returns 2, but we mod with 2 to get 0.
(1, 2, 115, 114, 1, 114)
# Checking the maximum value, ensuring it returns the max possible.
(1000, 255, 0, 255, 1, 255)
# And incrementing the rng returns 0.
(1000, 256, 0, 255, 1, 0)
