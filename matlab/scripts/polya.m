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
    k = size(Delta,1);
    
    % Compute the superurn of each pixel. As a matrix, this is A*V
    S = A * V;
    
    % Pull a ball from the superurn and create a matrix, the same size as
    % V, which has row i equal to the row of Delta for the ball we pulled.
    % For example, if we pull a ball of type j, then the i'th row of this
    % new matrix is row j of Delta.
    balls_to_add = zeros(size(V));
    
    % Vector containing the total number of balls for each superurn row
    T = S * ones(k, 1);
    
    % nxk matrix of weightings for each row of the superurn matrix
    W = S ./ T;
    
    % Compute the balls to add by randomly sampling from the superurn
%     for i = 1:size(S, 1)
%         % Randomly draw a ball from the superurn
%         ball_type_drawn = randsample(k, 1, true, W(i,:));
%         % Add the number of balls according to the type we pulled
%         balls_to_add(i, :) = Delta(ball_type_drawn, :);
%     end
    
    % Sample from the superurn using the weighting matrix to get the balls
    % to add
    B = matrix_sample(W)*Delta;
    
    % Add the balls to the original graph
    newV = V + B;
end
