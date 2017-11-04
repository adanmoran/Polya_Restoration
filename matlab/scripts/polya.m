%% Polya Contagion Model
% This model 
% V = Urn Matrix; A nxk matrix where n is the number of vertices, and k is
% the number of different types of balls
%
% A = Adjacency Matrix; a binary nxn matrix where a 1 represents 
% a connection between the vertices (i,j). Note that A must contain
% 1's along the diagonal so that the superurn can be correctly retrieved.
%
% Delta = a kxk matrix where the (i,j)th entry denotes the number of balls 
% of type j that will be added when a ball of type i is drawn.
function newV = polya(V, A, Delta)
    % How many types of balls we have
    NUM_TYPES = size(Delta,1);
    
    % Compute the superurn of each pixel. As a matrix, this is A*V
    superurn = A * V;
    
    % Pull a ball from the superurn and create a matrix, the same size as
    % V, which has row i equal to the row of Delta for the ball we pulled.
    % For example, if we pull a ball of type j, then the i'th row of this
    % new matrix is row j of Delta.
    balls_to_add = zeros(size(V));
    
    for i = 1:size(superurn, 1)
        % Compute total number of balls in this vertex
        total_balls = sum(superurn(i, :));
        % Decide the probability that you will draw each type of ball.
        weightings = superurn(i, :) / total_balls;
        % Randomly draw a ball from the superurn
        ball_type_drawn = randsample(NUM_TYPES, 1, true, weightings);
        % Add the number of balls according to the type we pulled
        balls_to_add(i, :) = Delta(ball_type_drawn, :);
    end
    
    % Add the balls to the original graph
    newV = V + balls_to_add;
end
