# EdisonPuzzle
I made this in response to Luigi Mangione's (healthcare CEO Brian Thompson's murderer) version of this. He posted his to github and it was incredibly slow. I was able to represent the grid using 64 bit unsigned ints and using bitwise operations to check whether a cell is free or not (0 or 1). I did this to try and hurt any legacy he thought he was leaving behind. His vs Mine are below, and mine is 500x faster (1.074 seconds aka 1,074 ms vs 2.2 ms):

<img width="724" alt="Screenshot 2025-01-07 at 4 31 54 PM" src="https://github.com/user-attachments/assets/d4429321-a89a-4cac-92cb-029bc1356d73" />

his version for reference, and I removed the visualizer when I ran his:
https://github.com/lnmangione/Edison-Puzzle
