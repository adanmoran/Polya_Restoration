%% Initialize Polya Urns
% Take the superurn of each pixel as its initial state.
% Each pixel will have initial_balls black balls if it is black,
% or initial_balls white balls if it is white.
%
% Inputs:
% image - the black-and-white image for which you're determining the vector
% of vertices.
% initial_balls - the number of balls to start with at time -1 (before the
% superurn is calculated)
% num_ball_types - the number of ball types to quantize the input image
% into
%
% Returns:
% urns - a length(adjacency) x num_ball_types vector where (i,1) is the 
% number of black balls and (i,k) is the number of the k-th ball type in 
% the urn related to pixel i.
function urns = initialize_polya_urns(image, initial_balls, num_ball_types)

    % Convert the image into a column vector along its rows. 
    % This should match the length of the adjacency matrix.
    imageCols = reshape(image', [], 1);
    % Convert the image columns into a list of indices for urn columns
    urnCols = double(imageCols) + 1;
    % Get the number of elements
    numRows = length(imageCols);
    % Now get the row elements
    rows = 1:numRows;
    % Create the list of initial balls
    balls = initial_balls * ones(numRows,1);
    % Now build the urn matrix as a sparse matrix
    urns = sparse(rows, urnCols, balls, numRows, num_ball_types);
    
    % The state at time 0 is the initial superurn
    %urns = adjacency * urns;
end