# Testing
Since chess is a game with rigid rules, it is very important that all the rules of chess are followed by the program with no expection.
This, it turns out is quite difficult as testing each positon will take a lot of time and we may miss some issue just because of human mistakes.

To assist with testing, we use **Perft**, (performance test, move path enumeration). It is a debugging function that walks through the move generation tree and counts all the leaf nodes of a certian depth.
In simpler terms, it is a function that can gives us the total number of positions that can be reached starting
from a chess board position upto certain depth.
It is at this point, we found out that our chess move generation has tremendious speed issues when going at larger depths. So we limited our testing a depth of 4 which takes around 2 mins to calculate. We also skipped some perft optimization such as hashing and bulk-counting
A much more indepth view of perft can be obtained from the chess programming wiki: https://www.chessprogramming.org/Perft

The number of moves obtained were compared with the infromation provided in the wiki: https://www.chessprogramming.org/Perft_Results and we were able to get a bird's eye veiw of all the bugs.
For a much more granular debugging purpose, we used **Stockfish**(a powerful and open source chess engine). 
We created a custrom chess position, and ran perft test on both our program and stockfish and compared the results. With this, we were able to find and solve many bugs.
