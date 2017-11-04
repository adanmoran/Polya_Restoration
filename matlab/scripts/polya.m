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
    
    % Vector containing the total number of balls for each superurn row
    T = S * ones(k, 1);
    
    % nxk matrix of weightings for each row of the superurn matrix
    W = S ./ T;
    
    % Sample from the superurn using the weighting matrix to get the balls
    % to add
    B = matrix_sample(W)*Delta;
    
    % Add the balls to the original graph
    newV = V + B;
end
