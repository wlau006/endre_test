# endre_test

emulating endre in code

Setup a basic program to parse the file generated by the script i made last week, 
next step is to create a way of easily swapping between redundancy elimination algorithms
We will probably only use chunk match for the destination actual redundancy detection protocol to keep things simple
There will be no actual file/data transfer (unless it seems necessary)

encode, store the fingerprint or whatever, also store corresponding string (unordered map keyed by marker)?

obviously need a way of determining which chunk of memory exactly was modified and being able to identify and replace said chunk of memory in the destination


the most confusing thing becomes of course how exactly I will measure the performance of each since

obviously runtime, overall reduction in bytes "sent" since i will probably not actually send them (unless I should, at which point i need to draft up a quick socket program to actually transfer bytes),
and then maybe memory overhead (which i'm not sure how i'll measure?)
