# Use of Hierarchical Inheritance
While creating the logic for movement of different pieces, we realized that the movement of Rook, bishop and Queen are very similar. That is, all three of them slide at a certain direction.
So to reduce code duplication in implementing their different movement logic we decided to store it all inone place and created a **SlidingPiece** class whose moment is just sliding at different directions.
Then the Rook, bishop and Queen inherit this functionality with the only difference being the direction in which they slide.
