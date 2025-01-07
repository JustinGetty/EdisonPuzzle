# EdisonPuzzle
I made this in response to Luigi Mangione's (healthcare CEO Brian Thompson's murderer) version of this. He posted his to github and it was incredibly slow. I was able to represent the grid using 64 bit ints and using bitwise operations to check whether a cell is free or not (0 or 1). I did this to try and hurt any legacy he thought he was leaving behind. His vs Mine are below, and mine is 500x faster (1.188 seconds aka 1,188 ms vs 2.3 ms):

<img width="837" alt="Screenshot 2025-01-07 at 4 21 59 PM" src="https://github.com/user-attachments/assets/428818e1-1030-40ac-aae8-b1ead7f246cf" />

his version for reference, and I removed the visualizer when I ran his:
https://github.com/lnmangione/Edison-Puzzle
