The problems are taken from the Assignments given by <b>Professor Piotr Gmytrasiewicz</b> while teaching Artificial Intelligence at University of Illinois at Chicago and uploaded after taking his consent.

The book which I used to study for this course is <b>Artificial Intelligence: A Modern Approach, 4th US edition</b>. The slides for the course can be accessed by clicking [here](http://aima.eecs.berkeley.edu/slides-pdf/)

The code for every program is written in C++. And can be run by using any [online compiler](https://www.onlinegdb.com/online_c++_compiler) and just typing in the input in the standard text input.


Sample Input for BFS, IDDFS, A-Star, IDA-Star: 1 0 2 4 5 7 3 8 9 6 11 12 13 10 14 15
where 0 represents the blank tile.


<b>Search Algorithms:</b>
The 15 puzzle is represented as a 2D matrix using vector of vectors. In this problem the cost for each move is 1.

And the four directions are traversed using dir_row[] and dir_col[] array.

A set is used to keep track of the visited states. isVisited() is used to check if the current state of the puzzle is visited and if it is not visited then mark the state as visited using the setVisited() function.

print_state() function is NOT used. It can be used to check the board configuration for debugging purpose.

expand() function is used to expand the next reachable states(board configurations) from the current state.


<b>Breadth First Search</b>:
In this uninformed search, nodes which are expanded are pushed to the end of queue. 
This gives optimal solution for 15 puzzle as the cost is 1 per step.
Space is a problem for this algorithm a lot of nodes are generated and stored in the queue.


<b>Iterative Deepening Depth First Search</b>:
This takes less memory than an Breadth First Search and a Depth First Search. 
In this algorithm we keep increasing the depth limit by 1 after each Depth First Search.
depth_limited_search() is function to search for a solution node till the depth limit l.
iterative_deepening_search() is a function where we increase the depth limit each time we are not able to find the goal state.


<b>A-Star</b>:
This is an informed search where we have used 2 heuristics(Manhattan Distance and Number of Misplaced Tiles) as a heuristic.
The values of f(n) is stored in the struct node each time we expand the node.
This algorithms gives an optimal solution by expanding all nodes with f(n) < C* and some nodes with f(n) = C*
I have used a <b>priority queue</b> for the fringe.
manhattan_distance() and heuristic() is used to calculate the heuristic.


<b>Iterative Deepening A-Star</b>:
[Here](https://en.wikipedia.org/wiki/Iterative_deepening_A*) is the wikipedia article that I used for this problem. Read the wiki page for explanation.
The memory usage is lower than the A-star algorithm.