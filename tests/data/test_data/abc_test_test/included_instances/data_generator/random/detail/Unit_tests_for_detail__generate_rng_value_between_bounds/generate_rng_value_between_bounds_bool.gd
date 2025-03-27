#class std::tuple<unsigned __int64,unsigned __int64,bool,bool,unsigned __int64,bool>
#rng_counter,rng_progress_to,lower_bound,upper_bound,rng_after_running,expected_result
# All of these have the same bounds, so only one element can be generated.
(0, 0, true, true, 0, true)
(1, 0, true, true, 0, true)
(0, 0, false, false, 0, false)
(1, 0, false, false, 0, false)
# denominator(min(difference,counter)=0. result=lower()=false.
(0, 0, true, false, 0, false)
# denominator(min(difference,counter)=1. result=lower()+rng(=0)%denominator=false.
(1, 0, true, false, 1, false)
(1, 1, true, false, 1, true)
# Pattern should repeat for higher rng_counters.
(2, 0, true, false, 1, false)
(2, 1, true, false, 1, true)
(500, 0, true, false, 1, false)
(1000, 1, true, false, 1, true)