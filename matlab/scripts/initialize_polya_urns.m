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
%
% Returns:
% urns - a length(adjacency)x2 vector where (i,1) is the number of
% black balls and (i,2) is the number of white balls in the urn
% related to pixel i.
function urns = initialize_polya_urns(image, initial_balls, numBallTypes)

    % Default number of balls for a polya process is 2
    % numBallTypes = 2;
    
%     if ~islogical(image)
%         % Create as many ball types as can fit in the numerical type of
%         % this image (e.g. 255 for 'uint8').
%         
%         % Get the numerical type of the image
%         classname = class(image); 
%         % Get the max size of that type, plus one for zero
%         numBallTypes = double(intmax(classname)) + 1; 
%     end

    % Convert the image into a column vector along its rows. 
    % This should match the length of the adjacency matrix.
    imageCols = reshape(image', [], 1);
    % Convert the image columns into a list of indices for urn columns
    urnCols = double(imageCols) + 1;
    % Get the number of elements
    numCols = length(imageCols);
    % Now get the row elements
    rows = 1:numCols;
    % Create the list of initial balls
    balls = initial_balls * ones(numCols,1);
    % Now build the urn matrix as a sparse matrix
    urns = sparse(rows, urnCols, balls, numCols, numBallTypes);
    
    % The state at time 0 is the initial superurn
    %urns = adjacency * urns;
end