#class std::tuple<unsigned __int64,unsigned __int64,unsigned __int64,unsigned __int64>
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
# Checking a high value
(18446744073709551615, 2000, 0, 18446744073709551615, 1, 2000)
# Checking the maximum value, ensuring it returns the max possible.
(18446744073709551615, 18446744073709551615, 0, 18446744073709551615, 1, 18446744073709551615)
# And incrementing the rng returns 0.
(0, 18446744073709551615, 0, 18446744073709551615, 0, 0)
